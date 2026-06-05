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

    // Merge into canonical order: (gom_index, emitter visitation rank, per-entity emission seq) — the
    // I4-corrected key (schema §7). gom_index orders mgr[3] before mgr[4]; entity_id is the Pass-A
    // visitation RANK (NOT object_id — sorting by object_id would reverse the head-inserted walk and
    // desync); seq breaks ties within an entity. The triple is globally unique because each entity is
    // owned by exactly one shard, so this is a TOTAL order — independent of shard/completion order, and
    // equal to the serial tick's emission sequence. stable_sort documents intent (the key is already
    // total, so it costs nothing).
    std::stable_sort(all.begin(), all.end(),
        [](const BufferedOp* a, const BufferedOp* b) {
            if (a->gom_index != b->gom_index) return a->gom_index < b->gom_index;
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
