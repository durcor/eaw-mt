// sim/tests/firing_spawn_test.cpp — b3 host gate for the projectile create+init restructure.
//
// Proves two things (firing_body_lift_scope.md §5, b3 row):
//   A. The Phase-A payload builder is a faithful, deterministic function of the resolved inputs — the
//      3825b0 branch logic (owner-vs-template damage/lifetime/speed, charge scale, vis frame, guided vs
//      ballistic) reproduces hand-computed expectations, with the binary's FP grouping.
//   B. THE DRAIN GATE (schema §9): a projectile SpawnCommand carrying the §3 payload survives the
//      Phase-A→buffer→Phase-B path with N-shard ≡ serial determinism. M firers each emit one projectile
//      spawn; for every N ∈ {1,2,4,8} × {round-robin, contiguous} partition × shuffled within-shard
//      processing order, the drained (assigned-id, applied ProjectileRecord) sequence is BIT-IDENTICAL
//      to the serial drain. This is the in-host analogue of the later in-game DTWA-structural oracle:
//      the create restructure does not change WHICH projectile gets WHICH init, nor the append-id order.
#include "../firing_spawn.h"
#include "../sim_parallel.h"
#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

using namespace sim;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

// ── Deterministic per-firer input generator (varies every branch across the population) ─────────────
static ProjectileFiringInputs make_firer(uint32_t i) {
    ProjectileFiringInputs in;
    in.requester     = reinterpret_cast<void*>(static_cast<uintptr_t>(0x10000u + i));
    in.template_id   = 200u + (i % 7u);
    in.team          = static_cast<int32_t>(i % 3u);
    in.spawn_pos     = { float(i), float(i) * 0.5f, -float(i) };
    in.flags         = i & 3u;

    in.firer_id      = 50000u + i;
    in.target        = reinterpret_cast<void*>(static_cast<uintptr_t>(0x20000u + (i * 7u)));
    in.target_sub_id = (i & 1u) ? int32_t(i % 5u) : -1;

    // damage: alternate owner-override vs template; every 3rd shot charges.
    in.owner_damage    = (i & 1u) ? float(10 + i % 13) : 0.0f;
    in.template_damage = float(5 + i % 9);
    in.apply_charge    = (i % 3u) == 0u;
    in.charge_mod      = 1.5f + float(i % 4) * 0.25f;

    // lifetime: alternate owner vs template.
    in.owner_lifetime    = (i % 4u == 0u) ? 0u : (300u + i);
    in.template_lifetime = 500u + i;

    // muzzle speed accumulation.
    in.base_speed      = (i & 1u) ? float(40 + i % 7) : 0.0f;
    in.template_speed  = float(30 + i % 5);
    in.target_extent   = float(i % 6) * 0.5f;
    in.add_subextent   = (i % 5u) == 0u;
    in.target_subextent= float(i % 4) * 0.25f;
    in.aim_z           = float(i % 11) - 5.0f;
    in.aimpoint_z      = float(i % 9) - 3.0f;

    // visibility frame.
    in.has_vis_frame = (i % 2u) == 0u;
    in.vis_base      = int32_t(1000 + i);
    in.vis_offset    = int32_t(i % 17);
    in.shroud        = reinterpret_cast<void*>(static_cast<uintptr_t>(0x30000u + i));

    // launch geometry: every 6th is a guided missile.
    in.guided = (i % 6u) == 0u;
    in.launch_dir = { float(i) * 0.1f, float(i) * -0.2f, float(i) * 0.3f };
    // a non-trivial target rotation (a yaw) + delta for guided cases
    float a = float(i % 8) * 0.3f;
    in.target_rot[0] =  std::cos(a); in.target_rot[1] = -std::sin(a); in.target_rot[2] = 0;
    in.target_rot[3] =  std::sin(a); in.target_rot[4] =  std::cos(a); in.target_rot[5] = 0;
    in.target_rot[6] = 0;            in.target_rot[7] = 0;            in.target_rot[8] = 1;
    in.guided_delta  = { float(i % 3) - 1.0f, float(i % 5) - 2.0f, float(i % 7) - 3.0f };
    return in;
}

// ── Recording WorldApply: append id (I1) + apply the projectile record (the §3 applier). ────────────
struct FiringWorld : WorldApply {
    uint32_t next_id;
    std::vector<std::pair<uint32_t, ProjectileRecord>> created;
    explicit FiringWorld(uint32_t first) : next_id(first) {}
    uint32_t create_object(const SpawnCommand& c) override {
        uint32_t id = next_id++;
        ProjectileRecord rec;
        if (c.projectile.present) firing_apply_projectile_record(rec, c.projectile);
        created.emplace_back(id, rec);
        return id;
    }
    void apply_signal(const Command&) override {}
};

static const uint32_t M        = 240;     // firers
static const uint32_t FIRST_ID = 800000;

// Build the canonical firer list in visitation-rank order (rank = index here). Each firer fires once.
static std::vector<ProjectileFiringInputs> build_firers() {
    std::vector<ProjectileFiringInputs> v;
    for (uint32_t i = 0; i < M; ++i) v.push_back(make_firer(i));
    return v;
}

// Run the tick: partition firers across `nshards`, optionally shuffle within-shard processing, emit each
// firer's projectile spawn into its shard buffer (tagged with its rank), then drain canonically.
//   partition: 0 = round-robin (i%N), 1 = contiguous blocks.
static FiringWorld run_tick(const std::vector<ProjectileFiringInputs>& firers, int nshards,
                            int partition, bool shuffle, std::mt19937& prng) {
    std::vector<ShardBuffer> shards(nshards);
    // assign each firer (by rank) to a shard
    std::vector<std::vector<uint32_t>> owned(nshards);
    for (uint32_t r = 0; r < firers.size(); ++r) {
        int s = (partition == 0) ? int(r % uint32_t(nshards))
                                 : int((uint64_t(r) * nshards) / firers.size());
        owned[s].push_back(r);
    }
    for (int s = 0; s < nshards; ++s) {
        std::vector<uint32_t> order = owned[s];
        if (shuffle) std::shuffle(order.begin(), order.end(), prng);
        for (uint32_t r : order) {
            shards[s].begin_entity(/*rank*/ r, /*gom*/ 0);
            shards[s].emit_spawn(firing_make_spawn(firers[r]));
        }
    }
    std::vector<ShardBuffer*> ptrs;
    for (auto& sb : shards) ptrs.push_back(&sb);
    FiringWorld w(FIRST_ID);
    drain_parallel(ptrs, w);
    return w;
}

static bool same(const FiringWorld& a, const FiringWorld& b) {
    if (a.created.size() != b.created.size()) return false;
    for (size_t i = 0; i < a.created.size(); ++i)
        if (a.created[i].first != b.created[i].first || !(a.created[i].second == b.created[i].second))
            return false;
    return true;
}

int main() {
    // ── A. Phase-A builder fidelity (branch + FP grouping) ──────────────────────────────────────────
    {
        // ballistic, owner-damage override, charged, owner lifetime, base speed + extents + |Δz|.
        ProjectileFiringInputs in;
        in.owner_damage = 12.0f; in.template_damage = 5.0f; in.apply_charge = true; in.charge_mod = 2.0f;
        in.owner_lifetime = 700; in.template_lifetime = 500;
        in.base_speed = 40.0f; in.template_speed = 30.0f;
        in.target_extent = 3.0f; in.add_subextent = true; in.target_subextent = 1.0f;
        in.aim_z = 5.0f; in.aimpoint_z = 2.0f;                       // |Δz| = 3
        in.has_vis_frame = true; in.vis_base = 1000; in.vis_offset = 7;
        in.guided = false; in.launch_dir = {0.1f, 0.2f, 0.3f};
        ProjectileInit p = firing_build_projectile_init(in);
        CHECK(p.present);
        CHECK(p.damage == 2.0f * 12.0f);                            // charge * owner
        CHECK(p.lifetime == 700u);                                  // owner nonzero
        // muzzle = |Δz| + (subextent + (extent + base)) = 3 + (1 + (3 + 40)) = 47
        CHECK(p.muzzle_speed == (std::fabs(5.0f - 2.0f) + (1.0f + (3.0f + 40.0f))));
        CHECK(p.vis_frame == 1007);
        CHECK(!p.guided && p.launch_dir[0] == 0.1f && p.launch_dir[2] == 0.3f);
    }
    {
        // template fallbacks: owner_damage<=0 ⇒ template; owner_lifetime==0 ⇒ template; base<=0 ⇒ template.
        ProjectileFiringInputs in;
        in.owner_damage = 0.0f; in.template_damage = 9.0f; in.apply_charge = false;
        in.owner_lifetime = 0; in.template_lifetime = 333;
        in.base_speed = 0.0f; in.template_speed = 25.0f;
        in.target_extent = 0.0f; in.add_subextent = false; in.aim_z = 0.0f; in.aimpoint_z = 0.0f;
        in.has_vis_frame = false;
        ProjectileInit p = firing_build_projectile_init(in);
        CHECK(p.damage == 9.0f);
        CHECK(p.lifetime == 333u);
        CHECK(p.muzzle_speed == 25.0f);
        CHECK(p.vis_frame == 0);                                    // not written when !has_vis_frame
    }
    {
        // guided: lead = rot · delta with identity rot ⇒ lead == delta; launch_dir stays zero.
        ProjectileFiringInputs in;
        in.guided = true;
        in.guided_delta = {1.5f, -2.5f, 3.5f};
        // identity rot is the default
        ProjectileInit p = firing_build_projectile_init(in);
        CHECK(p.guided);
        CHECK(p.guided_lead[0] == 1.5f && p.guided_lead[1] == -2.5f && p.guided_lead[2] == 3.5f);
        CHECK(p.launch_dir[0] == 0.0f && p.launch_dir[1] == 0.0f && p.launch_dir[2] == 0.0f);
    }

    // ── B. The drain gate: N-shard ≡ serial across partitions + shuffles ────────────────────────────
    std::vector<ProjectileFiringInputs> firers = build_firers();
    std::mt19937 prng(0xF1817000u);

    FiringWorld serial = run_tick(firers, /*N*/1, /*partition*/0, /*shuffle*/false, prng);
    CHECK(serial.created.size() == M);
    // ids are the contiguous append run FIRST_ID, +1, +2, … in rank order, and each created record
    // equals the applier output for that rank's firer (order preserved).
    for (uint32_t i = 0; i < M; ++i) {
        CHECK(serial.created[i].first == FIRST_ID + i);
        ProjectileRecord expect;
        firing_apply_projectile_record(expect, firing_build_projectile_init(firers[i]));
        CHECK(serial.created[i].second == expect);
    }

    for (int nshards : {1, 2, 4, 8}) {
        for (int partition : {0, 1}) {
            for (int rep = 0; rep < 3; ++rep) {
                FiringWorld par = run_tick(firers, nshards, partition, /*shuffle*/true, prng);
                CHECK(same(par, serial));
            }
        }
    }

    // ── B-negative control: keying the drain by ascending requester-id (instead of visitation rank)
    //    would REORDER the appends ⇒ a different id→record assignment. We emulate the wrong key by
    //    re-ranking firers in a different order and confirming the created stream DIFFERS. ────────────
    {
        std::vector<ShardBuffer> shards(1);
        // emit in REVERSE rank order but tag with a CONSTANT rank-by-emit-order (the "wrong" key):
        for (uint32_t k = 0; k < M; ++k) {
            uint32_t r = M - 1 - k;                     // process newest-first…
            shards[0].begin_entity(/*rank=*/k, 0);      // …but mis-tag rank by emit order (wrong)
            shards[0].emit_spawn(firing_make_spawn(firers[r]));
        }
        std::vector<ShardBuffer*> ptrs{ &shards[0] };
        FiringWorld wrong(FIRST_ID);
        drain_parallel(ptrs, wrong);
        CHECK(!same(wrong, serial));                    // wrong key ⇒ divergent id→record assignment
    }

    if (g_fail == 0) std::printf("firing_spawn_test: ALL PASS\n");
    else             std::printf("firing_spawn_test: %d FAILURE(S)\n", g_fail);
    return g_fail ? 1 : 0;
}
