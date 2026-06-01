// sim/command_sink.h — Phase-3 Unit-5 FRAMING: the cross-entity command-emission boundary.
//
// STATUS: interface framing only. The dispatch *machinery* (the SignalDispatcherClass fan-out
// FUN_140240940 and the OutgoingEventQueueClass drain) is Phase-B and is NOT lifted here — this
// header defines the SEAM that the already-lifted compute units (FUN_140387400 target-acq, the
// locomotors, the entity spine) emit into, plus the determinism contract the eventual Phase-B
// drain must honour. It is the analogue of OppScanEnv/LocomotorEnv: world-coupled side effects
// are modelled behind an interface, not byte-lifted.
//
// ── Why this boundary exists ───────────────────────────────────────────────────────────────────
// A per-entity update (Phase A) reads its own + others' last-tick state and computes its new
// state, writing ONLY its own fields. Everything it needs to do to *another* entity or to a
// global is a cross-entity side effect. In the live binary those side effects run INLINE, in the
// middle of the firing unit's update, via two distinct channels (both observed in FUN_140387400):
//
//   (1) PER-OBJECT SIGNAL/SLOT  (observer pattern).  Each GameObject embeds a SignalDispatcherClass
//       at `obj+0x38` (RTTI: SingletonInstance<SignalDispatcherClass>; per-object instance, not the
//       global singleton). Layout used by the dispatcher core (FUN_140240940):
//           +0x08  uint8   reentrancy-suppression flag
//           +0x0c  int32   active-dispatch depth counter
//           +0x10  ptr     hash-map  signal_id -> SignalListClass   (lookup = FUN_1401cacb0,
//                          key hashed `(id ^ 0xdeadbeef) & 0x7fffffff` Park-Miller)
//       A SignalListClass is an INSERTION-ORDERED intrusive slot list; emit walks it and invokes
//       each slot's vfunc_2 `(slot, dispatcher, sig_id, payload)`. Connected slots mutate arbitrary
//       other entities — THAT is the cross-entity write. Emission reads only `self+0x38`; the
//       fan-out reaches everywhere.
//
//         emit  = FUN_140220ed0(_, self+0x38, sig_id, payload)  -> FUN_140240940   (fire signal)
//         connect = FUN_140220e90(...)  -> FUN_1402406c0   (add slot; lazily builds the list and
//                          registers it into the GLOBAL SingletonInstance<SignalDispatcherClass>
//                          returned by FUN_140058570, which is just a registry of live emitters)
//         has-slot = FUN_140220eb0(...) -> FUN_1402408c0   (bool query)
//
//       NOTE: FUN_140058570()'s return is PASSED to FUN_140220ed0 but IGNORED there — emit
//       dispatches on `self+0x38`, not on the global singleton. The singleton call survives only
//       to force lazy construction/registration; treat it as a no-op for emission semantics.
//
//   (2) GLOBAL SCHEDULED-EVENT QUEUE.  OutgoingEventQueueClass (RTTI confirmed; strings
//       "OutgoingEventQueueClass::Add -- Adding {scheduled,non-scheduled} event %s"). Two live
//       global instances seen: DAT_140b27e60 (FUN_1402d5290, the "ordered" branch of 387400 line 99)
//       and DAT_140b2ed18 (OutgoingEventQueueClass::vfunc_1, FUN_14028d400). Events carry a fire
//       time and drain when the sim clock reaches it — this is the deferred/timed channel, distinct
//       from the synchronous signal fan-out.
//
// ── The two signals FUN_140387400 emits (the only emission sites in the lifted unit) ───────────
//   sig_id 0x20  — "fire-order in progress", PARAMETERLESS. Emitted on `self+0x38` (or the parent
//                  context `self+0x2b0` +0x38 when the unit is sub-ordinate) the first tick the
//                  ordered-fire path commits (in_progress_flag 0->1). decomp 387400.c lines 191-192.
//   sig_id 0x21  — OpportunityTargetAcquired, payload = OpportunityTargetAcquiredDataClass, a 3-ptr
//                  POD {vftable, opp_target_slot, hardpoint*}. Emitted on the autonomous path after
//                  a fresh opp target is bound and passes the fire-allowed gate. lines 297-314.
//
// ── Determinism contract the Phase-B drain MUST preserve ───────────────────────────────────────
//   * Per emission, slots fire in SignalListClass INSERTION order (FUN_140240940 walks the list
//     head->tail). FUN_140240940 also snapshots a visited-set and guards re-entrant emits with the
//     +0x08 flag / +0x0c depth — a slot may emit during dispatch; ordering across that nesting is
//     part of the contract and must be replayed, not reordered.
//   * Across entities, emissions are produced in GOM tick order (same order the RNG is drawn in).
//     The parallel model buffers each thread's emissions and drains buffers in canonical entity
//     order in Phase B, so the GLOBAL emit sequence equals the serial-tick sequence.
//   * The scheduled-event queue orders by (fire_time, insertion) — insertion order within a tick
//     is the tie-break and is therefore also GOM-order-sensitive.
//
// None of the above changes WHAT the lifted compute units decide; it changes only WHEN/WHERE the
// resulting side effects run. So a CommandSink that merely records (emitter, sig_id, payload) in
// order is a faithful Phase-A stand-in, and is fully host-testable: a unit's emission decisions are
// deterministic given its inputs, independent of any slot's behaviour.

#pragma once
#include <cstdint>
#include <cstddef>

namespace sim {

// Canonical signal ids seen at the FUN_140387400 emission sites. Extend as more units are lifted.
enum SignalId : uint32_t {
    kSigFireOrderInProgress      = 0x20, // parameterless; ordered-fire commit
    kSigOpportunityTargetAcquired = 0x21, // payload = OpportunityTargetAcquiredData
};

// POD payload for kSigOpportunityTargetAcquired (OpportunityTargetAcquiredDataClass, 3 ptrs).
// vtable is supplied by the live binary at drain; the lifted code only fills the two data ptrs.
struct OpportunityTargetAcquiredData {
    const void* vftable     = nullptr; // OpportunityTargetAcquiredDataClass::vftable (drain-filled)
    void*       target_slot = nullptr; // self->opp_target_slot at emit time (387400 local_60)
    void*       hardpoint   = nullptr; // the emitting HardPointClass*           (387400 local_58)
};

// One buffered cross-entity command. `emitter` is the GameObject* whose `+0x38` dispatcher the
// signal fires on (NOT pre-offset — the drain resolves +0x38, matching the live call which passes
// emitter+0x38). `payload` is borrowed; the lifted unit owns it until drain (host tests keep it
// alive). For parameterless signals payload==nullptr / payload_size==0.
struct Command {
    void*       emitter      = nullptr;
    uint32_t    sig_id       = 0;
    const void* payload      = nullptr;
    size_t      payload_size = 0;
};

// The Phase-A seam. Lifted compute units take a CommandSink& and EMIT instead of dispatching
// inline. A trivial recording sink is enough for host tests; the real Phase-B implementation
// replays buffered commands against the live SignalDispatcherClass / OutgoingEventQueueClass in
// canonical order (see determinism contract above).
struct CommandSink {
    virtual ~CommandSink() = default;

    // Per-object signal fan-out (channel 1). Mirrors FUN_140220ed0(_, emitter+0x38, sig_id, payload).
    virtual void emit_signal(void* emitter, uint32_t sig_id,
                             const void* payload = nullptr, size_t payload_size = 0) = 0;

    // Convenience for the only two FUN_140387400 signals.
    void emit_fire_order_in_progress(void* emitter) {
        emit_signal(emitter, kSigFireOrderInProgress);
    }
    void emit_opportunity_target_acquired(void* emitter, const OpportunityTargetAcquiredData& d) {
        emit_signal(emitter, kSigOpportunityTargetAcquired, &d, sizeof d);
    }

    // Global scheduled-event channel (channel 2). `fire_time` is the sim-clock tick the event drains
    // on (FUN_1402d5290 / OutgoingEventQueueClass::Add). Framed for completeness; no lifted unit
    // emits here yet (387400's ordered branch at line 99 is env-modelled for now).
    virtual void schedule_event(void* /*event*/, uint64_t /*fire_time*/) {}
};

} // namespace sim
