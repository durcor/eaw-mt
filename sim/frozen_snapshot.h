// sim/frozen_snapshot.h — Interface #2 (engine_integration_contract.md §2): the double-buffered
// tick-start read view that makes Phase-A workers see a CONSISTENT last-tick world.
//
// The hazard (threading_model.md wall #1/#2): in Phase A every object writes its OWN state (position,
// health, …) concurrently. A worker that reads ANOTHER object's field directly could observe a mid-tick
// mutation — a read-after-write race whose result depends on thread timing ⇒ non-deterministic ⇒ desync.
// FrozenSnapshot removes it: at tick start it copies the cross-object read set (§2.1 bullet 2) into a
// frozen buffer; Phase A reads other objects ONLY through read_object(), which returns the tick-start
// value regardless of how far their owners have advanced. Own-state is still read/written live (the
// owner races nobody for it under object-granular sharding).
//
// This is a concrete, host-testable realization (the contract's abstract interface is engine-typed). It
// is generic over the per-object read-state payload so the host determinism gate and the eventual engine
// ObjectState share one mechanism. Only the MUTABLE cross-read fields are frozen (registry membership and
// grid geometry are held constant by the Phase-A sweep barrier — they need no copy, only the
// no-mutation invariant the scheduler upholds).
#pragma once
#include <cstdint>
#include <vector>

namespace sim {

// The cross-object read fields a Phase-A worker may read off ANOTHER object (contract §2.1 bullet 2):
// transform, health, current target, team/faction, visibility. Mirrors the engine offsets in comments.
// (Kept POD so acquire() is a trivial copy — the double-buffer cost is one working-set memcpy/tick.)
struct ObjectState {
    uint32_t object_id = 0;        // obj+0x50 — stable identity (NOT the visitation rank)
    float    pos[3]    = {0,0,0};  // obj+0x78/+0x7c/+0x80 — locomotor writes this in Phase A
    float    health    = 0.0f;
    uint32_t target_id = 0;        // current target's object_id (0 = none)
    uint32_t team      = 0;
    uint8_t  visible   = 0;
};

// Double-buffered tick-start view. acquire() snapshots the live cross-read fields; read_object() serves
// the frozen copy; release() drops it so the next acquire() rebuilds from the now-updated world. Indexed
// by the object's VISITATION RANK in the Pass-A list (the scheduler's stable index), matching how the
// drain keys ops — callers that hold an object_id resolve it to a rank via the scheduler's order.
template <class State = ObjectState>
class FrozenSnapshot {
public:
    // Tick start: capture the double-buffer. `live` is the Pass-A-ordered cross-read state of every
    // object in this GOM (caller supplies it in visitation/rank order). Copy = the double-buffer.
    void acquire(const std::vector<State>& live) { frozen_ = live; }

    // Cross-object read — the FROZEN (tick-start) value. `rank` is the emitter's Pass-A index.
    const State& read_object(uint32_t rank) const { return frozen_[rank]; }

    uint32_t object_count() const { return static_cast<uint32_t>(frozen_.size()); }

    // Resolve a stable object_id to its frozen state (linear; the engine uses the registry index). Host
    // helper so a worker can read a *target* it knows only by id. Returns nullptr if not present.
    const State* find_by_id(uint32_t object_id) const {
        for (const State& s : frozen_) if (s.object_id == object_id) return &s;
        return nullptr;
    }

    // Tick end: release the read buffer.
    void release() { frozen_.clear(); }

private:
    std::vector<State> frozen_;
};

} // namespace sim
