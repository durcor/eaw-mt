// Phase-3 Unit-6 (Phase-B) implementation: the SignalDispatcherClass fan-out (FUN_140240940) + the
// serial command drain. See signal_dispatch.h for the layout map, the lockstep-cursor explanation,
// and the determinism contract. The emit() body deliberately mirrors the decompile's control flow
// (visited vector + cursor + touched-restart) rather than a cleaner std::set, so the restart/skip/
// append ordering is bit-faithful to the live dispatcher.
#include "signal_dispatch.h"

namespace sim {

// FUN_140240940 — fire `sig_id`'s slot list, head->tail, dedup'd by the lockstep cursor, restarting
// from head whenever a slot's callback trips the touched flag (a connect/disconnect/has-slot on this
// dispatcher). Slots already fired are skipped on restart; slots appended during dispatch fire when
// the cursor reaches them.
void SignalDispatcher::emit(uint32_t sig_id, const void* payload) {
    auto it = lists_.find(sig_id);     // 1cacb0 lookup
    if (it == lists_.end()) return;    // not found -> end sentinel -> no dispatch (240940 line 26)
    SignalList& list = it->second;

    std::vector<SignalSlot*> fired;    // local_58 visited-set
    fired.reserve(list.size());        // _Reallocate_exactly(_Var3 = SignalListClass+0x30 count)

    ++depth_;                          // +0x0c ++ (240940 line 42)
    const bool saved = touched_;       // uVar9 = *(disp+8)   (incoming flag, restored at exit)
    touched_ = false;                  // *(disp+8) = 0       (clear before the walk)
    bool restarted = false;            // tracks the decompile's `uVar9 = 1` left-set on restart

    std::size_t node   = 0;            // current node index (head->tail)
    std::size_t cursor = 0;            // iVar6/lVar7 dedup cursor into `fired` (reset only on restart)
    while (node < list.size()) {
        SignalSlot* slot = list[node];

        // Lockstep dedup (240940 lines 53-64): scan `fired` from the cursor; while the current
        // node's slot equals the cursor's fired entry, skip the node forward. The cursor advances
        // every step and never rewinds within a forward pass, so this only collapses the
        // already-fired prefix on a restart — it is NOT a general set membership test.
        if (cursor < fired.size()) {
            while (cursor < fired.size()) {
                if (fired[cursor] == slot) {
                    ++node;                          // lVar8 = next node
                    if (node >= list.size()) break;  // hit the sentinel
                    slot = list[node];               // re-read the slot from the new node
                }
                ++cursor;                            // iVar6++; lVar7++
            }
        }
        if (node >= list.size()) break;              // ran off the end during the skip

        fired.push_back(slot);                       // *puStack_50++ = slot
        slot->on_signal(*this, sig_id, payload);     // (**(slot+8))(slot, disp, sig_id, payload)

        if (!touched_) {                             // *(disp+8) still clear -> advance
            ++node;
        } else {                                     // touched -> restart from head, KEEP `fired`
            node     = 0;
            cursor   = 0;
            touched_ = false;
            restarted = true;                        // uVar9 = 1
        }
    }

    --depth_;                                        // +0x0c -- (240940 line 89)
    touched_ = restarted ? true : saved;             // *(disp+8) = uVar9  (1 if it ever restarted)
}

// Phase-B serial apply. Buffer order == canonical order (GOM tick × per-emission insertion), so a
// straight in-order replay reproduces the serial-tick global emit sequence. Mirrors one 220ed0 call
// per buffered command.
void drain_commands(const std::vector<RecordedCommand>& commands, EmitterResolver& resolve) {
    for (const RecordedCommand& rc : commands) {
        SignalDispatcher* disp = resolve.dispatcher_for(rc.emitter); // emitter+0x38
        if (disp == nullptr) continue;                               // 220ed0: null dispatcher -> skip
        // Route the only payload-bearing signal lifted so far (0x21); 0x20 is parameterless.
        const void* payload = (rc.sig_id == kSigOpportunityTargetAcquired && rc.has_payload)
                                  ? &rc.opp : nullptr;
        disp->emit(rc.sig_id, payload);
    }
}

} // namespace sim
