// sim/signal_dispatch.h — Phase-3 Unit-6 (Phase-B): the per-object signal/slot DISPATCH FAN-OUT.
//
// This is the DRAIN side of the CommandSink seam (command_sink.h frames the EMIT side). It lifts the
// synchronous observer fan-out the live binary runs inline in the middle of a unit's update:
//
//     FUN_140220ed0(_, emitter+0x38, sig_id, payload)  ->  FUN_140240940(dispatcher, sig_id, payload)
//
// In the parallel rewrite, Phase A records (emitter, sig_id, payload) into a CommandSink instead of
// dispatching inline; Phase B drains that buffer in canonical order, running this fan-out per
// command. So `SignalDispatcher::emit` IS FUN_140240940, and `drain_commands` is the serial Phase-B
// apply pass (the analogue of FUN_140220ed0 per buffered command). The slot bodies — the actual
// cross-entity writes — stay in the live binary / are env-modelled; what is lifted here is the
// determinism-critical DISPATCH ORDER and the re-entrancy / list-mutation handling.
//
// ── SignalDispatcherClass layout (obj+0x38; recovered from 240940 / 2406c0 / 2408c0 / 1cacb0) ──
//   +0x08  uint8   "touched" flag — set by structural/query access to the dispatcher DURING a
//                  dispatch. has-slot (2408c0) provably sets it (when the queried sig has a list);
//                  connect/disconnect mutate the list the walk is iterating, so they trip it too.
//                  240940 clears it before the walk and re-checks it after EACH slot fires: if set,
//                  the slot list may have changed under the iterator, so the walk RESTARTS from head
//                  (already-fired slots skipped via the cursor below). Left at 1 after a restart, so
//                  an OUTER (nested) dispatch in progress also restarts — the propagation is part of
//                  the contract (decompile `uVar9 = 1`).
//   +0x0c  int32   active-dispatch depth (bumped on entry, dropped on exit; supports nested emits).
//   +0x10  ptr     hash-map  sig_id -> SignalListClass  (lookup 1cacb0; key (id^0xdeadbeef)&0x7fffffff
//                  Park-Miller hashed). An absent sig_id => no dispatch at all (240940 line 26 guard).
//
//   SignalListClass: an INSERTION-ORDERED intrusive slot list. +0x10 head, +0x08 sentinel/end,
//   +0x30 slot count (used only to pre-size the visited vector). Node: +0x08 next, +0x18 slot*.
//   Slot: vtable[1] is the callback, invoked as (slot, dispatcher, sig_id, payload) (240940 line 74).
//
// ── The dedup is a LOCKSTEP CURSOR, not a set (the determinism-critical subtlety) ──
//   240940 keeps a `fired` vector (slots invoked so far, in fire order) plus a cursor that advances
//   in lockstep with the list head->tail. In a forward pass the cursor only ever moves forward and
//   never rewinds, so it dedups nothing (each node fires as reached). On a touched-RESTART the cursor
//   resets to 0 and re-walks the `fired` prefix against the (possibly changed) list, skipping the
//   contiguous run of already-fired slots, then fires whatever was appended during the callback.
//   We mirror the cursor exactly (not a std::set membership test) so the restart/skip/append ordering
//   is bit-faithful — a set would wrongly dedup a forward duplicate the cursor's monotonic scan lets
//   through.
//
// Determinism contract honoured (per command_sink.h): per emission, slots fire in insertion order;
// re-entrant nested emits are walked depth-first and their list-mutation propagates the restart; the
// drain replays the recorded buffer in canonical (GOM tick × per-emission insertion) order. None of
// this changes WHAT a Phase-A unit decided to emit — only WHEN/WHERE the side effects run.
#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include <unordered_map>
#include "recording_command_sink.h"

namespace sim {

struct SignalDispatcher; // fwd

// A connected slot. The live binary invokes slot->vtable[1](slot, dispatcher, sig_id, payload); we
// model that single virtual as on_signal. Slot bodies do the arbitrary cross-entity work.
struct SignalSlot {
    virtual ~SignalSlot() = default;
    virtual void on_signal(SignalDispatcher& dispatcher, uint32_t sig_id, const void* payload) = 0;
};

// SignalListClass — insertion-ordered slot list for one sig_id (head->tail == push order).
using SignalList = std::vector<SignalSlot*>;

// SignalDispatcherClass embedded at GameObject+0x38. Lifts the fan-out core (FUN_140240940) plus the
// connect / has-slot / disconnect primitives that set the +0x08 touched flag.
struct SignalDispatcher {
    // ── connect (220e90 -> 2406c0): append `slot` to sig_id's insertion-ordered list. ──
    void connect(uint32_t sig_id, SignalSlot* slot) {
        lists_[sig_id].push_back(slot);
        touched_ = true; // mutates the list a dispatch may be walking -> trip the restart guard
    }

    // ── disconnect: remove the first matching slot from sig_id's list. ──
    void disconnect(uint32_t sig_id, SignalSlot* slot) {
        auto it = lists_.find(sig_id);
        if (it == lists_.end()) return;
        SignalList& l = it->second;
        for (std::size_t i = 0; i < l.size(); ++i) {
            if (l[i] == slot) { l.erase(l.begin() + i); break; }
        }
        touched_ = true;
    }

    // ── has_slot (220eb0 -> 2408c0): is `slot` connected to sig_id? Sets the touched flag iff the
    //    sig has a list (matches 2408c0: `*(disp+8)=1` only inside the `found-in-map` branch). ──
    bool has_slot(uint32_t sig_id, SignalSlot* slot) {
        auto it = lists_.find(sig_id);
        if (it == lists_.end()) return false; // sig not in map -> no flag set, false
        touched_ = true;                      // 2408c0: query of a present sig trips the guard
        for (SignalSlot* s : it->second)
            if (s == slot) return true;
        return false;
    }

    // ── emit (220ed0 -> 240940): synchronous fan-out over sig_id's slots, in insertion order, with
    //    the lockstep-cursor dedup and touched-restart. THE Phase-B dispatch core. ──
    void emit(uint32_t sig_id, const void* payload = nullptr);

    // Inspectors for host tests / Phase-B drain bookkeeping.
    int  depth()    const { return depth_; }    // +0x0c active-dispatch depth
    bool flag_set() const { return touched_; }  // +0x08 touched flag (left set after a restart)

private:
    std::unordered_map<uint32_t, SignalList> lists_; // +0x10 sig_id -> SignalListClass
    bool touched_ = false;                           // +0x08
    int  depth_   = 0;                               // +0x0c
};

// Resolves a buffered command's emitter to the SignalDispatcher the live binary would fire on
// (emitter+0x38). Returning null skips the command (mirrors 220ed0's `param_2 != 0` guard).
struct EmitterResolver {
    virtual ~EmitterResolver() = default;
    virtual SignalDispatcher* dispatcher_for(void* emitter) = 0;
};

// Phase-B serial apply: replay a recorded command buffer (the Phase-A CommandSink output) in buffer
// order — which is canonical = GOM tick order × per-emission insertion. For each command, resolve
// emitter+0x38 and run the fan-out, routing the 0x21 opportunity-target payload through.
void drain_commands(const std::vector<RecordedCommand>& commands, EmitterResolver& resolve);

} // namespace sim
