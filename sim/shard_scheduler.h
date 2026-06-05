// sim/shard_scheduler.h — Interface #3 (engine_integration_contract.md §3): the Phase-A partition + run.
//
// Splits the live objects into N shards (each owning WHOLE GameObjects — never splitting one object's
// hardpoints, schema §2: load-bearing for the intra-object force-sum determinism), runs each shard's
// Phase-A body, and produces the per-shard ShardBuffers that the already-built drain_parallel() consumes.
//
// Determinism obligations it honours (schema §7 + I4):
//  - shard_of() is a STABLE pure function of the object's stable id (obj+0x50), so the partition
//    reproduces across machines (lockstep) and is independent of object count/order.
//  - Phase A visits owned objects in VISITATION-RANK order and calls begin_entity(rank, gom_index) before
//    each, so emitted ops carry the canonical merge key (gom_index, rank, seq) — NOT object_id (I4: the
//    master list is head-inserted, so id order ≠ visitation order; keying by id would reverse the create
//    sequence). The rank is the object's index in the Pass-A ordered list the caller supplies.
//  - Bodies draw RNG only via SimRng::substream(base, object_id, channel, tick) (invariant I2 gone), and
//    read OTHER objects only through the FrozenSnapshot — so a shard's output depends only on
//    (object_id, tick, snapshot), never on which shard ran it or in what order. That is the property the
//    test exploits by shuffling the processing order and still getting a bit-identical drain.
//
// Concrete + host-testable (the contract's interface is engine-typed); generic over the snapshot and the
// per-object body so the host gate and the engine integration share one mechanism.
#pragma once
#include <cstdint>
#include <vector>
#include "sim_parallel.h"

namespace sim {

// One Phase-A work unit: a stable object id + its visitation rank in the GOM's Pass-A list.
struct WorkItem {
    uint32_t object_id = 0;   // obj+0x50 — drives shard_of() and substream keying
    uint32_t rank      = 0;   // Pass-A visitation index — the canonical drain key (I4)
};

class ShardScheduler {
public:
    explicit ShardScheduler(int nshards) : nshards_(nshards < 1 ? 1 : nshards), buffers_(nshards_) {}

    // Stable partition: id % N (contiguous-range or any other pure fn of id is equally valid). The whole
    // object lands in exactly one shard, so its hardpoints never split across threads.
    int shard_of(uint32_t object_id) const { return static_cast<int>(object_id % static_cast<uint32_t>(nshards_)); }

    // Clear all shard buffers for a fresh pass/GOM.
    void reset() { for (auto& b : buffers_) b.ops.clear(); }

    // Run one Phase-A pass over `items`. In the real engine each shard runs on its own worker; here we
    // run them in `process_order` (defaults to rank order) to let the test SIMULATE arbitrary thread
    // completion — the tagging is deterministic, so the drain is order-independent regardless.
    // body signature: void(uint32_t object_id, uint32_t rank, const Snapshot&, ShardBuffer&).
    template <class Snapshot, class Body>
    void run_phase(const std::vector<WorkItem>& items, uint32_t gom_index,
                   const Snapshot& snapshot, Body&& body,
                   const std::vector<size_t>* process_order = nullptr) {
        auto run_one = [&](const WorkItem& it) {
            ShardBuffer& buf = buffers_[shard_of(it.object_id)];
            buf.begin_entity(it.rank, gom_index);     // tag ops with the canonical (gom, rank) key
            body(it.object_id, it.rank, snapshot, buf);
        };
        if (process_order) for (size_t i : *process_order) run_one(items[i]);
        else               for (const WorkItem& it : items) run_one(it);
    }

    // Borrowed pointers for drain_parallel (the scheduler owns the storage; it outlives the drain).
    std::vector<ShardBuffer*> buffers() {
        std::vector<ShardBuffer*> p;
        p.reserve(buffers_.size());
        for (auto& b : buffers_) p.push_back(&b);
        return p;
    }

    int nshards() const { return nshards_; }

private:
    int nshards_;
    std::vector<ShardBuffer> buffers_;
};

} // namespace sim
