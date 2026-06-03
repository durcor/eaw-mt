// sim/targeting_dispatch.h — Phase-3 IN sim behavior #7 (part 1): TargetingBehaviorClass::vfunc_6's
// game-mode DISPATCHER.
//
// Lifted from RVA 0x633a30 (decomp/633a30.c). Targeting is the seventh of the 13 in-slice IN sim
// behaviors (decomp_plan.md Phase-3), and — unlike #1-6, which are tight numeric/list tickers — it is
// the AI FIRE-CONTROL ORCHESTRATOR: a ~5500-byte body split across two huge game-mode-specific
// sub-implementations (FUN_140634810 mode 1, FUN_140633ae0 mode 2) that are almost pure virtual-
// dispatch control flow over the targeting machinery. Per the incremental plan agreed with the user,
// THIS unit lifts only the small, clean DISPATCHER 0x633a30; the two sub-bodies are deferred.
//
// ── Dispatcher algorithm (633a30, 174 bytes) ───────────────────────────────────────────────────────
//   1. LocomotorCommonClass::vfunc_6()  — run the behavior base (side effects; env, not in the core).
//   2. controller = entity->get_component(1)         // entity vtbl +0x10, key 1 (the unit's controller
//                                                     //   / owner object; null for unowned entities).
//   3. Resolve the dispatch mode int:
//        controller != null  ->  mode = controller->control_mode()   // controller vtbl +0x238
//        controller == null  ->  mode = manager ? game_speed_mode()   // DAT_140b15418 vtbl +0xe0
//                                               : default_mode        // DAT_140b153fc
//   4. FUN_140565e50(behavior)          — dispatcher prep (side effect; env, not in the core).
//   5. Dispatch on the resolved mode:
//        mode == 1  ->  FUN_140634810   (individual targeting regime)
//        mode == 2  ->  FUN_140633ae0   (team targeting regime — body references TeamTargetingBehaviorClass)
//        else       ->  return 0x650000 (NoOp; the spine's "handled" sentinel, no targeting work)
//
//   The deterministic core lifted here is exactly steps 2-3-5: the 3-way mode resolution and the 3-way
//   sub-implementation selection. Steps 1 and 4 are side-effecting prep that the core does not model.
//
// ── On the mode int's meaning ──────────────────────────────────────────────────────────────────────
//   vtbl +0x238 on the controller is an int "control mode" accessor — the SAME slot FUN_140397900
//   reads (decomp/397900.c:26, where it tests == 2). On the no-controller fallback path the mode comes
//   from a DIFFERENT source, the global game-speed mode (DAT_140b15418 vtbl +0xe0: 1=fast, 2=slow,
//   else unscaled — see hardpoint.h game_speed_mode + FUN_140540070), with DAT_140b153fc the default
//   when there is no manager. The two paths share the int SLOT but not the semantics; the dispatcher
//   only cares that the resolved int is 1, 2, or other. We characterise the values (1 -> individual,
//   2 -> team) from the sub-body class references but do NOT byte-assert the controller's enumeration.
//
// ── Determinism / threading notes ──────────────────────────────────────────────────────────────────
//   * The dispatcher core itself is a pure, side-effect-free integer decision (no RNG, no writes, no
//     cross-entity reads) — trivially Phase-A parallel-safe. The threading character of Targeting lives
//     entirely in the two SUB-bodies (which read the live global object set + emit target-assign / move
//     / fire commands into the GOM, the UnitAI #4 seam-class); those are deferred to a later unit.
#pragma once

namespace sim {

// Which targeting sub-implementation the dispatcher routes to (633a30.c lines 23-31).
enum class TargetingImpl {
    NoOp  = 0,   // resolved mode is neither 1 nor 2 -> behavior returns 0x650000 with no targeting work
    Mode1 = 1,   // FUN_140634810 — individual/per-unit targeting regime
    Mode2 = 2,   // FUN_140633ae0 — team targeting regime (refs TeamTargetingBehaviorClass)
};

// Resolved per-tick dispatch inputs. The mode int is sourced from one of two places depending on
// whether the entity has a controller (entity->get_component(1), vtbl +0x10):
//   * controller present -> mode = controller->control_mode()   (vtbl +0x238)
//   * controller absent   -> mode = has_manager ? game_speed_mode  (DAT_140b15418 vtbl +0xe0)
//                                               : default_mode      (DAT_140b153fc)
struct TargetingDispatchInputs {
    bool has_controller  = false;  // entity->get_component(1) != null
    int  controller_mode = 0;      // controller->vfunc_0x238()  (used iff has_controller)
    bool has_manager     = false;  // DAT_140b15418 != null
    int  game_speed_mode = 0;      // DAT_140b15418->vfunc_0xe0()  (used iff !has_controller && has_manager)
    int  default_mode    = 0;      // DAT_140b153fc              (used iff !has_controller && !has_manager)
};

// Resolve the dispatch mode int from the fallback chain. 633a30.c lines 12-21.
int targeting_resolve_mode(const TargetingDispatchInputs& in);

// Map a resolved mode int to the sub-implementation. 633a30.c lines 23-31.
TargetingImpl targeting_dispatch(int mode);

// Convenience: resolve + dispatch in one call (the dispatcher's deterministic core).
TargetingImpl targeting_dispatch_tick(const TargetingDispatchInputs& in);

// Env for wiring into the spine. The base LocomotorCommonClass::vfunc_6 + FUN_140565e50 prep run as
// side effects around the dispatch; only the mode-resolution inputs feed the deterministic core.
struct TargetingDispatchEnv {
    virtual ~TargetingDispatchEnv() = default;
    virtual void* get_controller(void* entity) = 0;     // entity vtbl +0x10 (key 1)
    virtual int   control_mode(void* controller) = 0;   // controller vtbl +0x238
    virtual bool  has_manager() = 0;                    // DAT_140b15418 != null
    virtual int   game_speed_mode() = 0;                // DAT_140b15418 vtbl +0xe0
    virtual int   default_mode() = 0;                   // DAT_140b153fc
};

} // namespace sim
