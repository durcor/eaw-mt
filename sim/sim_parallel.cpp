// sim/sim_parallel.cpp — Phase-B canonical-order drain. See sim_parallel.h for the model and the
// invariant argument.
#include "sim_parallel.h"
#include <algorithm>

namespace sim {

void drain_parallel(const std::vector<ShardBuffer*>& shards, WorldApply& world) {
    // Gather pointers to every buffered op across all shards. Pointers (not copies) keep the merge
    // cheap; the shards own the storage and outlive the drain.
    std::vector<const BufferedOp*> all;
    for (const ShardBuffer* shard : shards) {
        if (shard == nullptr) continue;
        for (const BufferedOp& op : shard->ops) all.push_back(&op);
    }

    // Merge into canonical order: ascending entity_id (GOM tick order), then per-entity emission seq.
    // (entity_id, seq) is globally unique because each entity is owned by exactly one shard, so this
    // is a TOTAL order — stable_sort vs sort is immaterial, but stable_sort documents the intent and
    // costs nothing here. The resulting sequence equals the serial tick's emission sequence.
    std::stable_sort(all.begin(), all.end(),
        [](const BufferedOp* a, const BufferedOp* b) {
            if (a->entity_id != b->entity_id) return a->entity_id < b->entity_id;
            return a->seq < b->seq;
        });

    // Apply Class-2 (spawn) and Class-2b (signal) ops in that single canonical order. Spawns drain
    // through create_object → CreateObject append (invariant I1: ids match the serial tick). Signals
    // fan out per object. Class-3 SFX is collected and flushed AFTER the lockstep ops (off-lockstep).
    std::vector<const BufferedOp*> sfx;
    for (const BufferedOp* op : all) {
        switch (op->kind) {
            case OpKind::Spawn:
                world.create_object(op->spawn);
                break;
            case OpKind::Signal: {
                // Repoint the borrowed payload at the inline copy for the one payload-bearing signal.
                Command c = op->signal;
                if (c.sig_id == kSigOpportunityTargetAcquired) {
                    c.payload      = &op->opp;
                    c.payload_size = sizeof(OpportunityTargetAcquiredData);
                }
                world.apply_signal(c);
                break;
            }
            case OpKind::Sfx:
                sfx.push_back(op);
                break;
        }
    }
    for (const BufferedOp* op : sfx) world.flush_sfx(op->sfx);
}

} // namespace sim
