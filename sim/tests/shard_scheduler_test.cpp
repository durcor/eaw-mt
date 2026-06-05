// sim/tests/shard_scheduler_test.cpp — Increment I5 host determinism gate for the ASSEMBLED parallel
// tick: FrozenSnapshot (Int #2) + ShardScheduler (Int #3) + drain_parallel (built) wired into the I4
// tick structure (two GOMs in fixed order, each: Phase-A sharded over the visitation-RANK list → barrier
// → canonical-order drain with a shared global id counter).
//
// What this proves beyond the existing sim_parallel_test (which validated the drain in isolation):
//  1. The assembled N-shard tick is BIT-IDENTICAL to the serial tick for N ∈ {1,2,4,8}, across two
//     partition schemes and a SHUFFLED processing order (threads completing out of order).
//  2. The canonical key is the VISITATION RANK, not object_id (I4): the master list is head-inserted, so
//     the serial visit order is descending-id. A naive ascending-id ordering produces a DIFFERENT
//     spawn-id assignment — the negative control that makes the I4 correction load-bearing, not cosmetic.
//  3. The shared GLOBAL id counter respects GOM order: every spawn by a GOM-0 emitter gets a lower
//     assigned id than every GOM-1 spawn (I4 finding #2).
//  4. FrozenSnapshot supplies cross-object reads: each body reads a NEIGHBOR's tick-start position from
//     the snapshot, so the result is independent of processing order — which is exactly why (1) holds.
#include "../shard_scheduler.h"
#include "../frozen_snapshot.h"
#include "../sim_parallel.h"
#include "../sim_rng.h"
#include <cstdio>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

using namespace sim;
using eaw::SimRng;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

static const eaw::u32 kBaseSeed = 0x0EA50000u;
static const eaw::u32 TICK      = 7777;
static const int      NGOM      = 2;
static const eaw::u32 PER_GOM   = 300;
static const uint32_t FIRST_ID  = 100000;     // new objects append after the existing population

// ── The world: two GOMs. Object ids are assigned in CREATION order (ascending); the master list is
// head-inserted, so the per-tick VISITATION order is DESCENDING id (rank 0 = newest = highest id). ──
struct GomModel {
    std::vector<WorkItem>   items;        // Pass-A list in visitation (rank) order
    std::vector<ObjectState> states;      // cross-read state, parallel to items (index == rank)
};

static GomModel build_gom(uint32_t gom_index) {
    GomModel g;
    const uint32_t base = 1000u + gom_index * 1000u;     // GOM0 ids 1000.., GOM1 ids 2000..
    // creation order = ascending id; visitation = descending id (head-insert).
    for (uint32_t r = 0; r < PER_GOM; ++r) {
        uint32_t id = base + (PER_GOM - 1 - r);          // rank 0 -> highest id
        g.items.push_back({id, r});
        ObjectState s;
        s.object_id = id;
        s.pos[0]    = static_cast<float>(id);
        s.pos[1]    = static_cast<float>(gom_index);
        s.team      = id & 1u;
        s.visible   = 1;
        g.states.push_back(s);
    }
    return g;
}

// ── Phase-A body: deterministic in (object_id, tick) + FROZEN cross-object reads. Emits a spawn/signal/
// sfx mix. Reads a NEIGHBOR's tick-start pos from the snapshot and folds it into a spawn — exercising
// the double-buffer (the value must be the neighbor's tick-start, never a mid-tick write). ──
template <class Snapshot>
static void phaseA_body(uint32_t id, uint32_t rank, const Snapshot& snap, ShardBuffer& buf) {
    SimRng rng = SimRng::substream(kBaseSeed, id, /*channel*/0, TICK);
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(id));
    uint32_t n = snap.object_count();
    const ObjectState& neighbor = snap.read_object((rank + 1u) % n);   // frozen cross-object read

    int actions = rng.range_i(1, 5);
    for (int k = 0; k < actions; ++k) {
        int pick = rng.range_i(0, 2);
        if (pick == 0) {
            SpawnCommand sc;
            sc.requester   = self;
            sc.template_id = static_cast<eaw::u32>(rng.range_i(100, 199));
            sc.pos[0]      = static_cast<float>(id);
            sc.pos[1]      = neighbor.pos[0];          // depends on snapshot, not live state
            sc.flags       = static_cast<eaw::u32>(rng.range_i(0, 3));
            buf.emit_spawn(sc);
        } else if (pick == 1) {
            if (k & 1) buf.emit_signal(self, kSigFireOrderInProgress, nullptr, 0);
            else {
                OpportunityTargetAcquiredData d;
                d.target_slot = reinterpret_cast<void*>(static_cast<uintptr_t>(neighbor.object_id));
                buf.emit_signal(self, kSigOpportunityTargetAcquired, &d, sizeof d);
            }
        } else {
            buf.emit_sfx_event(self, static_cast<eaw::u32>(rng.range_i(1, 9)));
        }
    }
}

// ── Recording WorldApply with a SHARED global id counter (one counter across both GOMs — the I1 global
// path). Records the canonical apply sequence so two runs can be compared bit-for-bit. ──
struct AppliedOp {
    OpKind kind; uint32_t emitter; uint32_t a; uint32_t b;
    bool operator==(const AppliedOp& o) const {
        return kind == o.kind && emitter == o.emitter && a == o.a && b == o.b; }
};
struct GlobalWorld : WorldApply {
    uint32_t next_id;
    std::vector<AppliedOp> lockstep, sfx;
    explicit GlobalWorld(uint32_t first) : next_id(first) {}
    static uint32_t id_of(const void* p) { return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(p)); }
    uint32_t create_object(const SpawnCommand& c) override {
        uint32_t id = next_id++;
        lockstep.push_back({OpKind::Spawn, id_of(c.requester), id, c.template_id});
        return id;
    }
    void apply_signal(const Command& c) override {
        uint32_t tag = (c.sig_id == kSigOpportunityTargetAcquired && c.payload) ? 1u : 0u;
        lockstep.push_back({OpKind::Signal, id_of(c.emitter), c.sig_id, tag});
    }
    void flush_sfx(const SfxCommand& c) override {
        sfx.push_back({OpKind::Sfx, id_of(c.emitter), c.sfx_id, 0});
    }
};

// Assemble the full I4 tick: for gom in fixed order { acquire snapshot; Phase-A sharded; drain }. The
// shared GlobalWorld counter persists across the two per-GOM drains (so GOM-0 spawns get earlier ids).
// `shuffle` reorders within-GOM processing to simulate arbitrary thread completion.
template <typename ShardFnUnused>
static GlobalWorld assembled_tick(const std::vector<GomModel>& goms, int nshards, bool shuffle,
                                  std::mt19937& prng) {
    GlobalWorld w(FIRST_ID);
    for (uint32_t gi = 0; gi < goms.size(); ++gi) {
        FrozenSnapshot<ObjectState> snap;
        snap.acquire(goms[gi].states);                 // tick-start double-buffer

        ShardScheduler sched(nshards);
        std::vector<size_t> order(goms[gi].items.size());
        std::iota(order.begin(), order.end(), 0u);
        if (shuffle) std::shuffle(order.begin(), order.end(), prng);

        sched.run_phase(goms[gi].items, gi, snap,
                        [](uint32_t id, uint32_t rank, const FrozenSnapshot<ObjectState>& s, ShardBuffer& b) {
                            phaseA_body(id, rank, s, b); },
                        &order);
        auto ptrs = sched.buffers();
        drain_parallel(ptrs, w);                       // Phase B serial, canonical (gom,rank,seq) order
        snap.release();
    }
    return w;
}

int main() {
    std::vector<GomModel> goms;
    for (int gi = 0; gi < NGOM; ++gi) goms.push_back(build_gom(gi));

    std::mt19937 prng(0x5EED1234u);

    // Reference: 1 shard, rank order, no shuffle.
    GlobalWorld serial = assembled_tick<int>(goms, 1, /*shuffle*/false, prng);
    CHECK(!serial.lockstep.empty());

    // ---- 1. Assembled serial-equivalence over N ∈ {1,2,4,8}, shuffled processing. ----
    for (int nshards : {1, 2, 4, 8}) {
        GlobalWorld par = assembled_tick<int>(goms, nshards, /*shuffle*/true, prng);
        CHECK(par.lockstep == serial.lockstep);   // bit-identical canonical lockstep stream
        CHECK(par.sfx      == serial.sfx);
    }

    // ---- 2. Negative control: keying by ascending object_id (instead of head-insert visitation rank)
    //         MUST diverge from the serial tick — proving the I4 rank correction is load-bearing. ----
    {
        std::vector<GomModel> ascending = goms;
        for (auto& g : ascending) {
            // re-rank in ASCENDING-id order (the "naive sort by id" a careless drain would imply)
            std::sort(g.items.begin(), g.items.end(),
                      [](const WorkItem& a, const WorkItem& b){ return a.object_id < b.object_id; });
            // states must stay parallel to items (index == rank), and snapshot neighbor reads follow rank
            std::sort(g.states.begin(), g.states.end(),
                      [](const ObjectState& a, const ObjectState& b){ return a.object_id < b.object_id; });
            for (uint32_t r = 0; r < g.items.size(); ++r) g.items[r].rank = r;
        }
        GlobalWorld naive = assembled_tick<int>(ascending, 1, false, prng);
        // Same set of emitters/templates, but the spawn-id ASSIGNMENT order differs ⇒ streams differ.
        CHECK(naive.lockstep != serial.lockstep);
    }

    // ---- 3. Shared global counter respects GOM order: every GOM-0 spawn id < every GOM-1 spawn id. ----
    {
        uint32_t max_g0 = 0, min_g1 = 0xffffffffu; bool any0 = false, any1 = false;
        for (const AppliedOp& op : serial.lockstep) {
            if (op.kind != OpKind::Spawn) continue;
            bool gom0 = (op.emitter / 1000u) == 1u;     // ids 1xxx = GOM0, 2xxx = GOM1
            if (gom0) { max_g0 = std::max(max_g0, op.a); any0 = true; }
            else      { min_g1 = std::min(min_g1, op.a); any1 = true; }
        }
        CHECK(any0 && any1);
        CHECK(max_g0 < min_g1);                          // GOM-0 drained first ⇒ lower ids
        // ids are the contiguous append run FIRST_ID, +1, +2, …
        std::vector<uint32_t> ids;
        for (const AppliedOp& op : serial.lockstep) if (op.kind == OpKind::Spawn) ids.push_back(op.a);
        for (size_t i = 0; i < ids.size(); ++i) CHECK(ids[i] == FIRST_ID + (uint32_t)i);
    }

    // ---- 4. Snapshot RAW-freedom: re-running with a different shuffle still matches serial (the body's
    //         cross-object reads come from the frozen snapshot, so processing order cannot perturb). ----
    {
        std::mt19937 p2(0xABCDEF01u);
        GlobalWorld a = assembled_tick<int>(goms, 4, true, p2);
        GlobalWorld b = assembled_tick<int>(goms, 8, true, p2);
        CHECK(a.lockstep == serial.lockstep);
        CHECK(b.lockstep == serial.lockstep);
    }

    if (g_fail == 0) std::printf("shard_scheduler_test: ALL PASS\n");
    else             std::printf("shard_scheduler_test: %d FAILURE(S)\n", g_fail);
    return g_fail ? 1 : 0;
}
