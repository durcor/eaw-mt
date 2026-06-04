// Host-side validation for the parallel command system (sim/sim_parallel.{h,cpp} + the SpawnCommand
// op + SimRng::substream). This is the determinism-gate from sim_parallel_command_schema.md §9, run
// offline: it proves the N-shard Phase-A/Phase-B pipeline produces output BIT-IDENTICAL to the serial
// tick, that object ids are append-stable, and that the per-entity RNG substreams are reproducible
// and order-independent (the property that removes invariant I2).
//
// The "behavior" here is a deterministic TOY (not a lifted unit): each entity, given only its id and
// the tick, draws from its substream and emits a mix of spawn/signal/sfx ops. That is enough to
// exercise the buffer, the canonical-order merge, append-id assignment, and the three channels. The
// lifted units' OWN correctness is covered by their existing bit-exact oracles; this test covers the
// PLUMBING that routes their emissions deterministically under sharding.
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

static const eaw::u32 kBaseSeed = 0x0EA40000u;

// ── Toy Phase-A behavior: emit a deterministic op mix for one entity, using ONLY (id, tick). ──────
// Because every decision flows from substream(base, id, channel, tick), the emitted ops for an entity
// are identical no matter which shard runs it or in what order — the invariant the merge relies on.
static void run_entity(eaw::u32 id, eaw::u32 tick, ShardBuffer& sink) {
    sink.begin_entity(id);
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(id));
    SimRng rng = SimRng::substream(kBaseSeed, id, /*channel*/0, tick);

    int actions = rng.range_i(1, 6);
    for (int k = 0; k < actions; ++k) {
        int pick = rng.range_i(0, 2);
        if (pick == 0) {
            SpawnCommand sc;
            sc.requester   = self;
            sc.template_id = static_cast<eaw::u32>(rng.range_i(100, 199));
            sc.pos[0]      = static_cast<float>(id);
            sc.pos[1]      = static_cast<float>(k);
            sc.flags       = static_cast<eaw::u32>(rng.range_i(0, 3));
            sink.emit_spawn(sc);
        } else if (pick == 1) {
            // alternate the two FUN_140387400 signals; 0x21 carries a payload, 0x20 is parameterless.
            if (k & 1) {
                sink.emit_signal(self, kSigFireOrderInProgress, nullptr, 0);
            } else {
                OpportunityTargetAcquiredData d;
                d.target_slot = reinterpret_cast<void*>(static_cast<uintptr_t>(id * 7 + k));
                sink.emit_signal(self, kSigOpportunityTargetAcquired, &d, sizeof d);
            }
        } else {
            sink.emit_sfx_event(self, static_cast<eaw::u32>(rng.range_i(1, 9)));
        }
    }
}

// ── A recording WorldApply: captures the canonical apply sequence so two runs can be compared. ────
struct AppliedOp {
    OpKind   kind;
    uint32_t emitter;       // entity id (we encode id in the emitter/requester pointer)
    uint32_t a;             // Spawn: assigned id   Signal: sig_id        Sfx: sfx_id
    uint32_t b;             // Spawn: template_id   Signal: payload-tag   Sfx: 0
    bool operator==(const AppliedOp& o) const {
        return kind == o.kind && emitter == o.emitter && a == o.a && b == o.b;
    }
};

struct RecordingWorld : WorldApply {
    uint32_t next_id;
    std::vector<AppliedOp> lockstep;   // spawns + signals, in canonical order
    std::vector<AppliedOp> sfx;        // class-3, flushed after (off-lockstep)
    explicit RecordingWorld(uint32_t first_id) : next_id(first_id) {}

    static uint32_t id_of(const void* p) { return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(p)); }

    uint32_t create_object(const SpawnCommand& c) override {
        uint32_t id = next_id++;                       // append → assign id (invariant I1)
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

// Drive the whole world for one tick under a given shard partition, processing entities in a given
// (possibly shuffled) order to model arbitrary thread interleaving. Returns the drained world.
// `shard_of(id)` picks which shard owns an entity; `order` is the processing order of entity ids.
template <typename ShardFn>
static RecordingWorld run_tick(const std::vector<eaw::u32>& order, eaw::u32 tick,
                               int nshards, uint32_t first_spawn_id, ShardFn shard_of) {
    std::vector<ShardBuffer> shards(nshards);
    for (eaw::u32 id : order) {
        run_entity(id, tick, shards[shard_of(id)]);   // whole-object: all of id's ops land in one shard
    }
    std::vector<ShardBuffer*> ptrs;
    for (auto& s : shards) ptrs.push_back(&s);
    RecordingWorld w(first_spawn_id);
    drain_parallel(ptrs, w);
    return w;
}

int main() {
    const eaw::u32 M    = 500;          // entities, ids 0..M-1
    const eaw::u32 TICK = 1234;
    const uint32_t FIRST_SPAWN_ID = M;  // new objects append after the existing ones

    std::vector<eaw::u32> ascending(M);
    std::iota(ascending.begin(), ascending.end(), 0u);

    // ---- Reference: the serial tick. One shard, entities in ascending id order, drain in buffer
    //      order. This is the ground truth every parallel run must reproduce bit-for-bit. ----
    RecordingWorld serial = run_tick(ascending, TICK, 1, FIRST_SPAWN_ID, [](eaw::u32){ return 0; });
    CHECK(!serial.lockstep.empty());   // the toy actually emitted something to compare

    // ---- 1. Serial-equivalence over N ∈ {1,2,4,8}, two partition schemes, shuffled processing. ----
    std::mt19937 prng(0xC0FFEEu);
    for (int nshards : {1, 2, 4, 8}) {
        // Shuffle the processing order to simulate threads completing in arbitrary order. The drain
        // sorts by (entity_id, seq), so the result must be independent of this order.
        std::vector<eaw::u32> shuffled = ascending;
        std::shuffle(shuffled.begin(), shuffled.end(), prng);

        // (a) round-robin partition (id % N) — adjacent ids land on different shards.
        RecordingWorld rr = run_tick(shuffled, TICK, nshards, FIRST_SPAWN_ID,
                                     [nshards](eaw::u32 id){ return (int)(id % (eaw::u32)nshards); });
        CHECK(rr.lockstep == serial.lockstep);   // bit-identical canonical lockstep stream
        CHECK(rr.sfx      == serial.sfx);        // sfx canonical too (host reproducibility)

        // (b) contiguous-range partition — blocks of ids per shard.
        eaw::u32 block = (M + nshards - 1) / (eaw::u32)nshards;
        RecordingWorld cg = run_tick(shuffled, TICK, nshards, FIRST_SPAWN_ID,
                                     [block, nshards](eaw::u32 id){
                                         int s = (int)(id / block); return s < nshards ? s : nshards - 1; });
        CHECK(cg.lockstep == serial.lockstep);
        CHECK(cg.sfx      == serial.sfx);
    }

    // ---- 2. Spawn-id stability: the assigned ids in a parallel run equal the serial-tick ids. ----
    {
        RecordingWorld par = run_tick(ascending, TICK, 8, FIRST_SPAWN_ID,
                                      [](eaw::u32 id){ return (int)(id % 8u); });
        std::vector<uint32_t> ser_ids, par_ids;
        for (auto& op : serial.lockstep) if (op.kind == OpKind::Spawn) ser_ids.push_back(op.a);
        for (auto& op : par.lockstep)    if (op.kind == OpKind::Spawn) par_ids.push_back(op.a);
        CHECK(!ser_ids.empty());
        CHECK(ser_ids == par_ids);
        // ids must be exactly the contiguous append run FIRST_SPAWN_ID, +1, +2, … (append order).
        for (size_t i = 0; i < ser_ids.size(); ++i) CHECK(ser_ids[i] == FIRST_SPAWN_ID + (uint32_t)i);
    }

    // ---- 3. Replay/MP determinism proxy: the identical inputs reproduce identical output across
    //         independent runs (the host-testable half of the lockstep contract; the in-game desync
    //         check is the other half). ----
    {
        RecordingWorld a = run_tick(ascending, TICK, 4, FIRST_SPAWN_ID, [](eaw::u32 id){ return (int)(id % 4u); });
        RecordingWorld b = run_tick(ascending, TICK, 4, FIRST_SPAWN_ID, [](eaw::u32 id){ return (int)(id % 4u); });
        CHECK(a.lockstep == b.lockstep);
        CHECK(a.sfx == b.sfx);
    }

    // ---- 4. Substream reproducibility + order-independence + decorrelation. ----
    {
        // Same tuple → identical full sequence, regardless of construction order.
        SimRng x = SimRng::substream(kBaseSeed, 42, 0, TICK);
        SimRng y = SimRng::substream(kBaseSeed, 42, 0, TICK);
        for (int i = 0; i < 10000; ++i) CHECK(x.step() == y.step());

        // Order independence: building entity 5's stream before entity 2's gives the same streams as
        // the reverse — there is no shared global state threading them.
        SimRng a5 = SimRng::substream(kBaseSeed, 5, 0, TICK);
        SimRng a2 = SimRng::substream(kBaseSeed, 2, 0, TICK);
        SimRng b2 = SimRng::substream(kBaseSeed, 2, 0, TICK);
        SimRng b5 = SimRng::substream(kBaseSeed, 5, 0, TICK);
        for (int i = 0; i < 1000; ++i) { CHECK(a5.step() == b5.step()); CHECK(a2.step() == b2.step()); }

        // Adjacent ids must NOT produce the same stream (decorrelation — the reason for the SplitMix
        // seed rather than re-seeding the LCG with the raw id).
        SimRng s0 = SimRng::substream(kBaseSeed, 1000, 0, TICK);
        SimRng s1 = SimRng::substream(kBaseSeed, 1001, 0, TICK);
        int same = 0;
        for (int i = 0; i < 64; ++i) if (s0.step() == s1.step()) ++same;
        CHECK(same <= 2);   // essentially never collide

        // Channel separates draw purposes for one entity.
        SimRng c0 = SimRng::substream(kBaseSeed, 77, 0, TICK);
        SimRng c1 = SimRng::substream(kBaseSeed, 77, 1, TICK);
        same = 0;
        for (int i = 0; i < 64; ++i) if (c0.step() == c1.step()) ++same;
        CHECK(same <= 2);

        // Tick re-keys each frame.
        SimRng t0 = SimRng::substream(kBaseSeed, 88, 0, TICK);
        SimRng t1 = SimRng::substream(kBaseSeed, 88, 0, TICK + 1);
        same = 0;
        for (int i = 0; i < 64; ++i) if (t0.step() == t1.step()) ++same;
        CHECK(same <= 2);
    }

    if (g_fail == 0) std::printf("sim_parallel_test: ALL PASS\n");
    else             std::printf("sim_parallel_test: %d FAILURE(S)\n", g_fail);
    return g_fail ? 1 : 0;
}
