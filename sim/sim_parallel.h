// sim/sim_parallel.h — Phase-3 Phase-B: the shard-local command buffer + the canonical-order drain.
//
// This is the realization of the boundary-scope's command-system design (sim_parallel_command_schema.md):
// the concrete buffer a Phase-A worker records into, and the serial Phase-B drain that replays every
// buffered cross-boundary op in CANONICAL ENTITY ORDER so the parallel tick is bit-identical to the
// serial tick. It sits on top of command_sink.h (the EMIT seam) — a ShardBuffer IS a CommandSink, so
// any already-lifted compute unit (FUN_140387400 target-acq, the locomotors, …) emits into it
// unchanged.
//
// ── The model (sim_parallel_command_schema.md §1–2, §7) ─────────────────────────────────────────
//   Phase A (parallel): the world is partitioned into shards by a stable function of object id; each
//   shard owns WHOLE GameObjects (never splitting one object's hardpoints across threads — §2). A
//   worker processes its owned objects, writing only own-state directly and recording every
//   cross-boundary side effect as a BufferedOp into its OWN ShardBuffer. No shard touches another's
//   buffer, so Phase A needs no locks.
//   Phase B (serial): drain_parallel() merges all shard buffers into the single canonical order and
//   applies them. Canonical order == (gom_index, emitter_iteration_rank, per-entity emission seq) —
//   CORRECTED per Increment I4 (schema §7 I4 box): the merge key is the emitter's VISITATION RANK in
//   its GOM's Pass-A list, NOT its object_id. The serial tick stamps each new object's id in visitation
//   order, and the engine's master list is head-inserted (20a9b0/29bfe0:10) so the walk runs
//   newest-first ≈ descending object_id; sorting by object_id would reverse the create order and
//   desync. gom_index orders mgr[3] before mgr[4] (the shared global id counter depends on it), rank is
//   the Pass-A index the ShardScheduler assigns, seq is the monotonic counter of that entity's
//   emissions this tick. Because each entity is owned by exactly one shard, (gom_index, rank, seq) is
//   globally unique, so the merge is a total deterministic sort — INDEPENDENT of which shard produced
//   an op or the wall-clock order shards finished. That independence is the whole correctness argument.
//
// ── Why this preserves the two lockstep invariants ──────────────────────────────────────────────
//   (I1) object-id append order: SpawnCommands drain through WorldApply::create_object in canonical
//        order, so CreateObject appends — and thus assigns new ids — in the exact serial sequence.
//   (I2) RNG draw order: ELIMINATED upstream by per-entity substreams (SimRng::substream); a unit's
//        emitted ops depend only on (entity_id, tick), never on a global draw order, so the buffer
//        contents are sharding-invariant before the merge even runs.
//   Class-1 sensor/fog writes are observer-slot-disjoint and applied directly in Phase A (no op type,
//   just a sweep barrier — §3); they never enter this buffer. Class-3 SFX is off-lockstep and flushed
//   after the lockstep merge purely for reproducible host playback (§6.3).
#pragma once
#include <cstdint>
#include <vector>
#include "command_sink.h"

namespace sim {

// Which channel a buffered op belongs to. Signal = Class 2b (per-object +0x38 fan-out); Spawn =
// Class 2 (registry-append create); Sfx = Class 3 (presentation, off-lockstep).
enum class OpKind : uint8_t { Signal, Spawn, Sfx };

// One recorded cross-boundary op, tagged with its canonical position. `entity_id`/`seq` are the
// merge key; exactly one of the three payload members is valid per `kind`. The 0x21 opportunity-
// target payload is COPIED inline (`opp`) because the emitting unit only owns it until emit returns
// (same reason RecordingCommandSink copies it).
struct BufferedOp {
    uint32_t     gom_index = 0;     // which GOM serviced the emitter (mgr[3]=0 before mgr[4]=1); I4
    uint32_t     entity_id = 0;     // emitter's Pass-A VISITATION RANK within its GOM (NOT object_id); I4
    uint32_t     seq       = 0;     // per-entity monotonic emission counter (tie-break within entity)
    OpKind       kind      = OpKind::Signal;
    Command                       signal{};  // valid iff kind==Signal (payload repointed to `opp`)
    OpportunityTargetAcquiredData opp{};      // inline copy for sig 0x21
    SpawnCommand spawn{};                      // valid iff kind==Spawn
    SfxCommand   sfx{};                        // valid iff kind==Sfx
};

// A shard's thread-local command buffer. IS a CommandSink, so lifted Phase-A units emit into it with
// no change. The driver calls begin_entity(id) before processing each owned object so emitted ops are
// tagged with that entity's id and a fresh per-entity seq.
struct ShardBuffer : CommandSink {
    std::vector<BufferedOp> ops;

    // Open the emission scope for one owned object (Phase A). `entity_id` is the emitter's Pass-A
    // visitation RANK in its GOM (assigned by the ShardScheduler — I4), `gom_index` orders the GOMs.
    // Resets the per-entity emission counter. (gom_index defaults to 0 for single-GOM callers/tests.)
    void begin_entity(uint32_t entity_id, uint32_t gom_index = 0) {
        cur_entity_ = entity_id;
        cur_gom_    = gom_index;
        cur_seq_    = 0;
    }

    void emit_signal(void* emitter, uint32_t sig_id,
                     const void* payload, size_t payload_size) override {
        BufferedOp op;
        op.gom_index   = cur_gom_;
        op.entity_id   = cur_entity_;
        op.seq         = cur_seq_++;
        op.kind        = OpKind::Signal;
        op.signal      = Command{emitter, sig_id, nullptr, 0};
        if (sig_id == kSigOpportunityTargetAcquired && payload &&
            payload_size >= sizeof(OpportunityTargetAcquiredData)) {
            op.opp = *static_cast<const OpportunityTargetAcquiredData*>(payload);
        }
        ops.push_back(op);
    }

    void emit_spawn(const SpawnCommand& cmd) override {
        BufferedOp op;
        op.gom_index = cur_gom_;
        op.entity_id = cur_entity_;
        op.seq       = cur_seq_++;
        op.kind      = OpKind::Spawn;
        op.spawn     = cmd;
        ops.push_back(op);
    }

    void emit_sfx_event(void* emitter, uint32_t sfx_id) override {
        BufferedOp op;
        op.gom_index = cur_gom_;
        op.entity_id = cur_entity_;
        op.seq       = cur_seq_++;
        op.kind      = OpKind::Sfx;
        op.sfx       = SfxCommand{emitter, sfx_id};
        ops.push_back(op);
    }

private:
    uint32_t cur_entity_ = 0;
    uint32_t cur_gom_    = 0;
    uint32_t cur_seq_    = 0;
};

// The Phase-B world boundary the drain applies ops to. The real engine impl runs CreateObject /
// SignalDispatcher::emit / SFXEventManager::Start; host tests record. create_object MUST assign the
// next id by append (return value == that id) — that is where invariant I1 is honoured.
struct WorldApply {
    virtual ~WorldApply() = default;

    // Class 2: append-create. Returns the assigned object id (== append order == next free id).
    virtual uint32_t create_object(const SpawnCommand& cmd) = 0;

    // Class 2b: per-object +0x38 signal fan-out (the analogue of one FUN_140220ed0 call).
    virtual void apply_signal(const Command& cmd) = 0;

    // Class 3: presentation cue. Off-lockstep — called after the lockstep merge, order carries no
    // determinism weight. Default no-op so a headless/dedicated-server drain can drop audio.
    virtual void flush_sfx(const SfxCommand& /*cmd*/) {}
};

// Phase-B serial drain. Merges every shard's buffer into canonical (entity_id, seq) order and applies
// the Class-2/2b ops in that single order (spawns and signals interleaved exactly as the serial tick
// would have emitted them), then flushes Class-3 SFX afterward. The merge is a stable total sort, so
// the result does not depend on shard order or completion order. `shards` is borrowed; not mutated.
void drain_parallel(const std::vector<ShardBuffer*>& shards, WorldApply& world);

} // namespace sim
