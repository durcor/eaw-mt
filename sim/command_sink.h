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
//   (2) PRESENTATION SFX CUE.  387400 line 99 is FUN_1402d5290(&DAT_140b27e60, DAT_140b301c8, 0,0,0)
//       = SFXEventManagerClass::Start_SFX_Event_Internally (decomp 2d5440: ActiveSFXEventClass, SFX
//       presets, looping, voice limits) — a sound cue fired when an ordered fire-order completes, NOT
//       a gameplay scheduled event. It is PRESENTATION-ONLY: mutates no sim state and draws NO sim
//       RNG (its probability gate at 2d5440:153 draws from &DAT_140a13e20, a SEPARATE LCG word from
//       the sim RNG &DAT_140a13e24 that every core sim fn uses; a13e20 is independently seeded in
//       init, 5d990:95). Its internal queue (2d72c0/b27e60) even sorts by WALL-CLOCK timeGetTime()
//       (2d5290:21) — a lockstep sim queue can never key on real time. So its ordering is NOT
//       lockstep-relevant (see sfx_channel.h). 387400 does NOT touch a gameplay scheduled-event queue.
//
//       (The genuine OutgoingEventQueueClass — DAT_140b2ed18, "OutgoingEventQueueClass::Add" — is a
//       galactic/strategic-layer MP event queue, 28d400: PENDING_TACTICAL_BATTLE_VOTE; a different
//       layer from this tactical sim slice. Framed below as schedule_event, but no lifted unit
//       emits there.)
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

// Canonical signal ids seen at the lifted emission sites. Extend as more units are lifted.
enum SignalId : uint32_t {
    kSigFireOrderInProgress      = 0x20, // parameterless; ordered-fire commit (FUN_140387400)
    kSigOpportunityTargetAcquired = 0x21, // payload = OpportunityTargetAcquiredData (FUN_140387400)
    kSigAbilityChargeComplete    = 0x29, // parameterless; ability chargeup reached target (FUN_14042f460)
    kSigAbilityRecharged         = 0x2c, // payload = AbilitySignalData; cooldown hit 0 (FUN_14042f910)
    kSigDamageEffectsCleared     = 0x2d, // parameterless; last active damage effect expired (FUN_14058bd80)
};

// POD payload for kSigOpportunityTargetAcquired (OpportunityTargetAcquiredDataClass, 3 ptrs).
// vtable is supplied by the live binary at drain; the lifted code only fills the two data ptrs.
struct OpportunityTargetAcquiredData {
    const void* vftable     = nullptr; // OpportunityTargetAcquiredDataClass::vftable (drain-filled)
    void*       target_slot = nullptr; // self->opp_target_slot at emit time (387400 local_60)
    void*       hardpoint   = nullptr; // the emitting HardPointClass*           (387400 local_58)
};

// POD payload for kSigAbilityRecharged (AbilitySignalDataClass). The stack-built struct at the
// FUN_14042f910 emit site is {vftable@+0x00, slot@+0x08} (local_48 = vftable, local_40 = slot).
// vtable is supplied by the live binary at drain; the lifted code only fills the slot index.
struct AbilitySignalData {
    const void* vftable = nullptr; // +0x00 AbilitySignalDataClass::vftable (drain-filled)
    int         slot    = 0;       // +0x08 ability slot index (42f910 local_40)
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

// One buffered presentation SFX cue (channel 2). `emitter` is the firing GameObject*; `sfx_id`
// identifies the SFX-event template (the live call passes DAT_140b301c8, the resolved SFXEvent*).
// PRESENTATION-ONLY — see sfx_channel.h for the determinism contract (no sim state, no sim RNG,
// order not lockstep-relevant).
struct SfxCommand {
    void*    emitter = nullptr;
    uint32_t sfx_id  = 0;
};

// One buffered object-creation op (channel = Class 2). Models the inline
// GameObjectManager::CreateObject (FUN_14029f810) call a Phase-A unit would run to spawn a
// projectile / impact-fx / detached child. Deferred to the serial Phase-B drain because CreateObject
// APPENDS to the global registry (the new object's id == append order) and advances the shared sim
// LCG — the two lockstep invariants. CRUCIALLY there is NO pre-assigned id field: the drain calls
// CreateObject in canonical requester order, and the append assigns the id, so every new id is
// identical to the serial tick (invariant I1). The lifted unit only records WHAT to spawn, never WHERE
// in the id space. See sim_parallel_command_schema.md §6.2.
struct SpawnCommand {
    // I2-confirmed (3825b0:266): the real spawn site calls 29f810(*(requester+0x2b8), ...), i.e. 29f810
    // param_1 is the requester's MANAGER, not the requester itself. The drain resolves it the same way:
    //   mgr = *(GameObjectManager**)((char*)requester + 0x2b8)   // never a global singleton
    void*    requester   = nullptr;  // emitting GameObject*; manager = *(requester+0x2b8) = 29f810 param_1
    uint32_t template_id = 0;        // object-type/template (29f810 param_2)
    float    pos[3]      = {0, 0, 0};// world spawn position (29f810 param_4)
    uint32_t flags       = 0;        // spawn-kind packed (projectile / fx / detached; param_3/6/7)
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

    // Presentation SFX cue (channel 2). Mirrors FUN_1402d5290(&SFXEventManager, sfx, ...) at
    // 387400:99. PRESENTATION-ONLY: no sim state, no sim RNG; ordering is not lockstep-relevant.
    // A lifted unit emits here and the serial drain (drain_sfx, sfx_channel.h) routes to an SfxSink.
    // Default no-op so units that ignore audio need not override.
    virtual void emit_sfx_event(void* /*emitter*/, uint32_t /*sfx_id*/) {}

    // Object creation (Class 2). Mirrors the inline GameObjectManager::CreateObject (FUN_14029f810)
    // a unit runs to spawn a projectile/fx/child. Recorded, never run inline: the Phase-B drain calls
    // CreateObject in canonical requester order so the appended id matches the serial tick (I1). See
    // sim_parallel.h for the buffer + drain. Default no-op so units that never spawn need not override.
    virtual void emit_spawn(const SpawnCommand& /*cmd*/) {}

    // Galactic/strategic OutgoingEventQueueClass (DAT_140b2ed18). A genuine timed/scheduled queue
    // ordered by (fire_time, insertion), but it lives OUTSIDE this tactical sim slice — no lifted
    // tactical unit emits here. Framed for completeness only.
    virtual void schedule_event(void* /*event*/, uint64_t /*fire_time*/) {}
};

} // namespace sim
