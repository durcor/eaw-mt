// sim/unit_ai.h — Phase-3 IN sim behavior #4: UnitAIBehaviorClass::vfunc_6.
//
// Lifted from RVA 0x4f6070 (decomp/4f6070.c). Fourth of the 13 in-slice IN sim behaviors
// (decomp_plan.md Phase-3). Picked (user sign-off) as the "richest/hardest" next candidate after
// AbilityCountdown (#3): unlike #1-3 — which each touched ONLY their own entity's state — this one
// reads the GLOBAL object list and writes a per-(observer,object) visibility matrix plus a SHARED
// sensor grid. It is the first CROSS-ENTITY behavior, and so the first with a real MP-determinism /
// thread-safety story (see the determinism notes below — that is the architectural payoff).
//
// ── What it is ───────────────────────────────────────────────────────────────────────────────────
// A per-entity FOG-OF-WAR / SENSOR-VISIBILITY ticker over the entity's AI manager at owner+0x108.
// Each tick (while the sim is simulating) it does three things:
//   A. THROTTLED SELF-REFRESH (every 300 ticks): when the observer's own world transform changed
//      since last refresh, push a "this object moved" update into the shared sensor grid and recache
//      the transform. A fixed-phase scheduler: next_due += 300 (NOT next_due = now + 300).
//   B. REVEAL HOOK (gated): a separate per-object reveal pass (FUN_1404f6470), pure env side effect.
//   C. VISIBILITY MATRIX: for every object in the global list, evaluate a heavy LOS/faction predicate
//      and maintain a per-object visibility BIT (owner-private array at ai+0x58). On each bit EDGE
//      (hidden->visible or visible->hidden) notify the shared sensor grid.
//
// ── Per-tick algorithm (4f6070) ──────────────────────────────────────────────────────────────────
//   0. prologue: LocomotorCommonClass::vfunc_6 — shared spine schedule stamp (entity-spine owned).
//   1. ai = owner+0x108 (the AI manager sub-object); now = game[2] (the global sim tick counter).
//      if !game->vfunc_0xf0()  → return    // sim not simulating this frame: whole tick is a no-op
//   --- BLOCK A: throttled self-refresh (only if the shared sensor mgr exists) -------------------
//   2. if (ai->next_due(+0x20) <= now  &&  sensor_mgr != 0):
//        if (ai->init_flag(+0x18) == 0):                 // first pass: build the per-object bit array
//            ensure_initialized(ai)        // FUN_1406537d0 + FUN_1404f4c60 + resize(ai+0x50, #objs),
//                                          //   zero ai->vis[0..count); the init routine sets +0x18.
//        if (transform(obj+0x248..+0x274) == cache(ai+0x68..+0x94)):  // 12 floats, exact compare
//            ai->next_due += 300                         // unchanged: just advance the schedule
//        else:
//            object_moved(owner)           // FUN_140659760(sensor_mgr, owner) — refresh sensor print
//            ai->next_due += 300
//            cache(ai+0x68..) = transform(obj+0x248..)   // recache the 12 floats
//   --- BLOCK B: reveal hook (gated) -------------------------------------------------------------
//   3. if (owner+0x333 != -1 && (ai+0x38 != 0 || FUN_1403973b0()+0x6a != 0)):
//        reveal(owner)                     // FUN_1404f6470(behavior) — env side effect
//   --- BLOCK C: visibility matrix ----------------------------------------------------------------
//   4. if (!fog_active):                   // DAT_140a284c4==0 OR observer hidden (owner+0x34b/0x348)
//        clear ai->vis[0..count) = 0       // fog off / self hidden → nothing is visible to anyone
//      else:
//        for idx in [0, ai->count(+0x60)):
//            if idx >= #objects: return     // the live global list is shorter than the array: stop
//            obj = object_at(idx)           // FUN_140294bc0(&DAT_140a16fd0, idx); may be null → skip
//            v = visible(owner, obj)        // FUN_14035f470(game, obj->id(+0x4c), owner, 0) → 0/1
//            if v == 0:                                   // not visible
//                if ai->vis[idx] != 0 && sensor_mgr: became_hidden(owner,obj)   // edge 1->0
//                ai->vis[idx] = 0
//            else:                                        // visible
//                if ai->vis[idx] == 0 && sensor_mgr: became_visible(owner,obj)  // edge 0->1
//                ai->vis[idx] = 1
//
// ── The cross-boundary acts (and why this one is different) ───────────────────────────────────────
// All three notifies mutate the SHARED sensor-grid manager DAT_140b31900, keyed by (observer,object):
//   * object_moved   (FUN_140659760): re-stamps the observer's footprint across the grid.
//   * became_visible (FUN_140657ce0): writes the object's grid cells for this observer.
//   * became_hidden  (FUN_140657db0): clears the object's grid cells for this observer.
// They are env-modeled (LOS rasterisation into the grid) exactly like AbilityCountdownEnv's gates.
// The per-object visibility BIT array (ai+0x58) is owner-PRIVATE; only the notify side-effects touch
// shared memory. The predicate `visible` (FUN_14035f470) is a heavy LOS/faction/range test — modeled,
// not byte-lifted.
//
// ── Determinism notes (the architectural payoff) ─────────────────────────────────────────────────
//   * NOT Phase-A parallel-safe as written, unlike DamageTracking/EnergyPool/AbilityCountdown. This
//     behavior READS the global object list (DAT_140a16fd0) and the global fog/tick state, and its
//     three cross-boundary acts WRITE the shared sensor grid (DAT_140b31900). Two observers ticking
//     concurrently both mutate that grid. To parallelise: either buffer the became_visible/hidden/
//     moved updates (like the CommandSink emits) and drain them in a serial phase, or shard the grid
//     per-observer. The visibility BIT array itself is owner-private and safe to update in parallel.
//   * Must run in a phase where the object SET is stable — it indexes the live global list mid-sweep
//     and early-returns if the list shrank under it (the `idx >= #objects` guard), so concurrent
//     spawn/despawn would change which objects a sweep covers.
//   * No sim RNG in the per-tick path. The throttle and the transform compare/cache are pure integer/
//     float reads of own state; the only nondeterminism risk is the shared-grid write ordering above.
//   * The throttle is a FIXED-PHASE scheduler: next_due += 300 (never reset to now+300), so a behavior
//     that fell behind catches up by fixed 300-tick steps rather than snapping to the current tick.
#pragma once
#include "eaw_types.h"
#include <vector>

namespace sim {

// The per-entity AI manager at owner+0x108. The visibility bit array tracks the global object list.
struct UnitAiState {
    static constexpr int kCacheFloats = 12;   // ai+0x68..+0x94 — the cached object world transform
    static constexpr int kRefreshTicks = 300; // ai+0x20 throttle increment (sensor refresh interval)

    int                 next_due    = 0;      // ai+0x20 — next tick the throttled refresh is due
    bool                initialized = false;  // ai+0x18 == 0 means "not yet built" (first pass builds)
    eaw::f32            cache[kCacheFloats] = {}; // ai+0x68..+0x94 — cached observer world transform
    std::vector<eaw::u8> vis;                  // ai+0x58 (count ai+0x60) — per-object visibility bits
};

// World-coupled inputs + cross-boundary acts, modeled (not byte-lifted) like AbilityCountdownEnv.
struct UnitAiEnv {
    virtual ~UnitAiEnv() = default;

    // game->vfunc_0xf0(): is the sim actually simulating this frame? false → the whole tick no-ops.
    virtual bool sim_active() = 0;

    // DAT_140b31900 != 0: does the shared sensor-grid manager exist? Gates Block A and every notify.
    virtual bool sensor_present() = 0;

    // DAT_140a284c4 != 0 && !(owner+0x34b==-1 && owner+0x348==-1): is fog-of-war live for this observer?
    // When false, Block C clears the whole visibility array instead of sweeping.
    virtual bool fog_active() = 0;

    // (DAT_140a16fd8 - DAT_140a16fd0) >> 3: number of objects in the live global object list.
    virtual int object_count() = 0;

    // FUN_140294bc0(&DAT_140a16fd0, idx): the idx-th global object pointer (may be null → skipped).
    virtual void* object_at(int idx) = 0;

    // FUN_14035f470(game, obj->id(+0x4c), owner, 0): is `obj` visible to `owner` this tick? The heavy
    // LOS/faction/range predicate — the core calls it once per object and stores 0/1 as the vis bit.
    virtual bool visible(void* owner, void* obj) = 0;

    // First-pass init: FUN_1406537d0 + FUN_1404f4c60 + resize(ai+0x50) to object_count + zero the bits.
    // The engine's init routine also sets ai+0x18 (the init flag); the core flips st.initialized after.
    virtual void ensure_initialized(UnitAiState& st, int object_count) = 0;

    // owner+0x333 != -1 && (ai+0x38 != 0 || FUN_1403973b0()+0x6a != 0): should the Block-B reveal run?
    virtual bool reveal_due(void* owner) = 0;
    // FUN_1404f6470(behavior): the gated per-object reveal pass — pure env side effect.
    virtual void reveal(void* owner) = 0;

    // The three shared-sensor-grid writes (DAT_140b31900), keyed by (observer, object):
    virtual void object_moved(void* owner) = 0;                 // FUN_140659760 — observer moved
    virtual void became_visible(void* owner, void* obj) = 0;    // FUN_140657ce0 — obj entered sensors
    virtual void became_hidden(void* owner, void* obj) = 0;     // FUN_140657db0 — obj left sensors
};

// UnitAIBehaviorClass::vfunc_6 (RVA 0x4f6070), deterministic core.
//   st        — owner+0x108 AI-manager state (next_due, init flag, transform cache, vis bit array)
//   owner     — the owning GameObjectClass* (= behavior+0x28; the observer + env subject)
//   now       — the global sim tick (game[2]); the throttle compares next_due against it
//   transform — the observer's current world transform, 12 floats (obj+0x248..+0x274), for change-detect
//   env       — sim/fog/sensor gates, the LOS predicate, and the three shared-grid cross-boundary acts
void unit_ai_tick(UnitAiState& st, void* owner, int now,
                  const eaw::f32 transform[UnitAiState::kCacheFloats], UnitAiEnv& env);

} // namespace sim
