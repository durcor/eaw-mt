// sim/tests/parallel_fire_test.cpp — host determinism gate for the PARALLEL-FIRE-PASS architecture
// (firing_body_lift_scope.md §8 b4-revisit + a2_integration_scope.md §1).
//
// The b4-revisit reframed firing as a SEPARATE READ-ONLY PHASE after the cheap-mass settles, with all
// writes deferred. This test assembles that two-parallel-phase tick from the built pieces (ShardBuffer +
// drain_parallel + the b3 firing_spawn payload) and proves the load-bearing correctness claim:
//
//   for each GOM (fixed order):
//     PHASE A ∥  cheap-mass — each object writes its OWN settled position (disjoint by rank)
//     ── barrier ──
//     PHASE B ∥  fire pass — each firing object reads its target's NOW-SETTLED position (LIVE, but stable
//                because Phase A finished and wrote only own-state) and BUFFERS a b3 projectile
//                SpawnCommand (payload encodes the settled read) + a Class-2b signal + an SFX cue
//     ── barrier ──
//     PHASE C ·  drain_parallel in canonical (gom,rank,seq) order
//
// CLAIM 1 (determinism): the whole pipeline is BIT-IDENTICAL N-shard ≡ 1-shard for N∈{1,2,4,8} ×
//   {round-robin, contiguous} partitions × SHUFFLED processing order in BOTH phases independently. This is
//   the proof that the read-only fire phase needs no snapshot and no predicate lift: Phase B reads
//   live-but-stable state, writes only its own shard buffer, so its output depends only on (id, settled
//   world), never on shard/order. (Simulated-parallel via shuffle-invariance: disjoint-shard + shuffle ≡
//   serial ⟹ any thread interleaving is safe — the same technique as shard_scheduler_test.)
//
// CLAIM 2 (honest retrofit delta, a2_integration_scope §3 / a2.0b): the two-phase tick is NOT stock-
//   identical. A stock-interleaved tick (settle+fire per object in one pass) makes a firing object read
//   each target at whatever update-state its rank had reached — a mix of pre/post-move. The two-phase
//   split makes every fire read the fully-settled positions. We DEMONSTRATE the difference (expected, by
//   design — validated by determinism + lockstep, not stock-equivalence).
#include "../firing_spawn.h"
#include "../sim_parallel.h"
#include <cstdio>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

using namespace sim;
using eaw::vec3;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

static const uint32_t TICK     = 4242;
static const int      NGOM     = 2;
static const uint32_t PER_GOM  = 200;
static const uint32_t FIRST_ID = 900000;

struct Obj {
    uint32_t id;            // obj+0x50 (stable; drives partition)
    uint32_t rank;          // Pass-A visitation rank (canonical drain key; head-insert => descending id)
    uint32_t target_rank;   // a neighbor in the same GOM (the unit it fires at)
    bool     firer;         // ~every 3rd object fires this tick
};

// Phase-A cheap-mass: settle the object's OWN position deterministically in (id, tick).
static vec3 phaseA_pos(uint32_t id, uint32_t tick) {
    float a = float((id * 2654435761u + tick * 40503u) & 0xffffu) * 0.001f;
    return { float(id % 97) + a, float(id % 89) - a, float(tick % 71) + a * 0.5f };
}

// Phase-B fire: build the b3 projectile inputs from the firer's own id + the target's SETTLED position.
// The settled read is encoded into the payload (launch_dir) so the drained ProjectileRecord reflects it —
// i.e. the test's comparison is sensitive to WHICH position Phase B read.
static ProjectileFiringInputs fire_inputs(const Obj& f, const vec3& tgt_settled) {
    ProjectileFiringInputs in;
    in.requester     = reinterpret_cast<void*>(static_cast<uintptr_t>(0x100000u + f.id));
    in.template_id   = 300u + (f.id % 5u);
    in.team          = static_cast<int32_t>(f.id % 3u);
    in.firer_id      = f.id;
    in.target        = reinterpret_cast<void*>(static_cast<uintptr_t>(0x200000u + f.target_rank));
    in.target_sub_id = static_cast<int32_t>(f.id % 7u);
    in.owner_damage    = (f.id & 1u) ? float(8 + f.id % 11) : 0.0f;
    in.template_damage = float(4 + f.id % 6);
    in.owner_lifetime    = (f.id % 4u == 0u) ? 0u : (200u + f.id);
    in.template_lifetime = 400u + f.id;
    in.base_speed      = float(20 + f.id % 9);
    in.template_speed  = float(15 + f.id % 4);
    // the settled target position drives the launch direction (ballistic) — the phase-A→phase-B dependency
    in.guided     = false;
    in.launch_dir = tgt_settled;
    in.aim_z      = tgt_settled.z;
    in.aimpoint_z = 0.0f;
    return in;
}

// Recording WorldApply: append id (I1) + apply the projectile record + record the Class-2b / SFX streams.
struct FireWorld : WorldApply {
    uint32_t next_id;
    struct Spawn { uint32_t id; ProjectileRecord rec; };
    std::vector<Spawn> spawns;
    std::vector<std::pair<uint32_t,uint32_t>> signals;   // (emitter-id, sig_id)
    std::vector<std::pair<uint32_t,uint32_t>> sfx;        // (emitter-id, sfx_id)
    explicit FireWorld(uint32_t first) : next_id(first) {}
    static uint32_t idof(const void* p){ return uint32_t(reinterpret_cast<uintptr_t>(p)); }
    uint32_t create_object(const SpawnCommand& c) override {
        uint32_t id = next_id++;
        ProjectileRecord rec;
        if (c.projectile.present) firing_apply_projectile_record(rec, c.projectile);
        spawns.push_back({id, rec});
        return id;
    }
    void apply_signal(const Command& c) override { signals.push_back({idof(c.emitter), c.sig_id}); }
    void flush_sfx(const SfxCommand& c) override { sfx.push_back({idof(c.emitter), c.sfx_id}); }
    bool operator==(const FireWorld& o) const {
        if (spawns.size()!=o.spawns.size()||signals!=o.signals||sfx!=o.sfx) return false;
        for (size_t i=0;i<spawns.size();++i)
            if (spawns[i].id!=o.spawns[i].id || !(spawns[i].rec==o.spawns[i].rec)) return false;
        return true;
    }
};

static int shard_of(uint32_t rank, int n, int partition, uint32_t per_gom) {
    return (partition==0) ? int(rank % uint32_t(n)) : int((uint64_t(rank)*n)/per_gom);
}

// The assembled two-phase tick. Phase A settles positions (disjoint by rank); barrier; Phase B fires from
// the settled positions into shard buffers; barrier; drain. Processing order in each phase is shuffled to
// simulate arbitrary thread completion.
static FireWorld run_two_phase(const std::vector<std::vector<Obj>>& goms, int nshards, int partition,
                               bool shuffle, std::mt19937& prng) {
    FireWorld w(FIRST_ID);
    for (uint32_t gi = 0; gi < goms.size(); ++gi) {
        const auto& objs = goms[gi];
        std::vector<vec3> settled(objs.size());

        std::vector<size_t> ord(objs.size());
        std::iota(ord.begin(), ord.end(), 0u);
        if (shuffle) std::shuffle(ord.begin(), ord.end(), prng);

        // PHASE A ∥ — each object writes its OWN settled slot (disjoint index ⇒ shuffle-invariant).
        for (size_t i : ord) settled[objs[i].rank] = phaseA_pos(objs[i].id, TICK);

        // ── barrier ──
        std::vector<ShardBuffer> shards(nshards);
        std::vector<size_t> ordB(objs.size());
        std::iota(ordB.begin(), ordB.end(), 0u);
        if (shuffle) std::shuffle(ordB.begin(), ordB.end(), prng);

        // PHASE B ∥ — fire from the SETTLED positions; buffer spawn + signal + sfx.
        for (size_t i : ordB) {
            const Obj& o = objs[i];
            if (!o.firer) continue;
            ShardBuffer& buf = shards[shard_of(o.rank, nshards, partition, PER_GOM)];
            buf.begin_entity(o.rank, gi);
            void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(o.id));
            buf.emit_spawn(firing_make_spawn(fire_inputs(o, settled[o.target_rank])));
            buf.emit_signal(self, kSigFireOrderInProgress, nullptr, 0);
            buf.emit_sfx_event(self, 100u + (o.id % 9u));
        }
        // ── barrier ──
        std::vector<ShardBuffer*> ptrs; for (auto& s : shards) ptrs.push_back(&s);
        drain_parallel(ptrs, w);     // PHASE C · canonical (gom,rank,seq) order
    }
    return w;
}

// Stock-interleaved reference (settle + fire per object in ONE pass, rank order). A firer reads its
// target's CURRENT position — already-settled iff the target's rank < the firer's rank, else pre-move.
// Demonstrates the retrofit delta (NOT a determinism reference).
static FireWorld run_stock_interleaved(const std::vector<std::vector<Obj>>& goms) {
    FireWorld w(FIRST_ID);
    for (uint32_t gi = 0; gi < goms.size(); ++gi) {
        const auto& objs = goms[gi];
        std::vector<vec3> pos(objs.size());
        std::vector<bool> done(objs.size(), false);
        ShardBuffer buf;  // 1 "shard" = serial
        for (const Obj& o : objs) {
            pos[o.rank] = phaseA_pos(o.id, TICK); done[o.rank] = true;  // settle this object now
            if (o.firer) {
                // read target at its CURRENT state: settled if already processed, else its pre-move slot 0
                vec3 t = done[o.target_rank] ? pos[o.target_rank] : vec3{0,0,0};
                buf.begin_entity(o.rank, gi);
                void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(o.id));
                buf.emit_spawn(firing_make_spawn(fire_inputs(o, t)));
                buf.emit_signal(self, kSigFireOrderInProgress, nullptr, 0);
                buf.emit_sfx_event(self, 100u + (o.id % 9u));
            }
        }
        std::vector<ShardBuffer*> ptrs{ &buf };
        drain_parallel(ptrs, w);
    }
    return w;
}

int main() {
    // Build two GOMs. Creation order = ascending id; visitation = descending id (head-insert), so rank 0
    // is the newest/highest id — matching the engine (I4) and shard_scheduler_test.
    std::vector<std::vector<Obj>> goms(NGOM);
    for (int gi = 0; gi < NGOM; ++gi) {
        uint32_t base = 1000u + uint32_t(gi) * 1000u;
        for (uint32_t r = 0; r < PER_GOM; ++r) {
            Obj o;
            o.id          = base + (PER_GOM - 1 - r);   // rank 0 -> highest id
            o.rank        = r;
            o.target_rank = (r + 7u) % PER_GOM;         // some neighbor in the same GOM
            o.firer       = (o.id % 3u) == 0u;
            goms[gi].push_back(o);
        }
    }

    std::mt19937 prng(0x9A11F19Eu);

    // Reference: 1 shard, round-robin, no shuffle (the two-phase serial).
    FireWorld serial = run_two_phase(goms, 1, 0, /*shuffle*/false, prng);
    CHECK(!serial.spawns.empty());
    // ids are the contiguous append run (I1), GOM-0's spawns all before GOM-1's.
    for (size_t i = 0; i < serial.spawns.size(); ++i) CHECK(serial.spawns[i].id == FIRST_ID + (uint32_t)i);

    // CLAIM 1 — N-shard ≡ serial across partitions × shuffled BOTH phases.
    for (int n : {1, 2, 4, 8}) {
        for (int part : {0, 1}) {
            for (int rep = 0; rep < 3; ++rep) {
                FireWorld par = run_two_phase(goms, n, part, /*shuffle*/true, prng);
                CHECK(par == serial);
            }
        }
    }

    // CLAIM 1b — the fire pass really read the SETTLED positions (a spawn's launch_dir must equal the
    // target's Phase-A-settled pos, not its pre-move zero). Spot-check the first GOM-0 firer.
    {
        const auto& objs = goms[0];
        for (const Obj& o : objs) if (o.firer) {
            vec3 expect = phaseA_pos(objs[o.target_rank].id, TICK);
            // find this firer's spawn in the serial output by matching firer_id
            bool found = false;
            for (const auto& s : serial.spawns)
                if (s.rec.firer_id == o.id) {
                    CHECK(s.rec.launch_dir[0]==expect.x && s.rec.launch_dir[1]==expect.y &&
                          s.rec.launch_dir[2]==expect.z);
                    found = true; break;
                }
            CHECK(found);
            break;
        }
    }

    // CLAIM 2 — retrofit delta: the two-phase tick DIFFERS from stock-interleaved (expected, by design).
    {
        FireWorld stock = run_stock_interleaved(goms);
        CHECK(stock.spawns.size() == serial.spawns.size());   // same firings…
        CHECK(!(stock == serial));                            // …but different read semantics ⇒ different payloads
    }

    if (g_fail == 0) std::printf("parallel_fire_test: ALL PASS\n");
    else             std::printf("parallel_fire_test: %d FAILURE(S)\n", g_fail);
    return g_fail ? 1 : 0;
}
