// Locomotor movement integrators — lifted from StarWarsG.exe (Phase-3 unit 3).
//
// The real mover: each concrete LocomotorBehaviorClass overrides vtable slot 6 (vfunc_6), called
// per tick from the entity-update spine (FUN_1403a6b80 behavior loop). vfunc_6 reads the unit's
// world position (entity+0x78), runs a per-type state machine over the locomotor state record
// (entity+0xa8), and writes the new position back (entity+0x78). This is the Phase-A body the
// rewrite parallelizes — it reads its own + others' last-tick state and writes only its own
// position/locomotor-state (cross-entity effects are emitted as events → Phase B).
//
// Source functions:
//   LocomotorCommonClass::vfunc_6  0x4c2f70  shared pre-step (reschedule) -> reschedule()
//   StarshipLocomotorBehaviorClass::vfunc_6  0x6236b0  the state machine  -> starship_tick()
//   FUN_1403a8f90  position writer (entity+0x78, sets dirty flag)         -> set_position()
//   FUN_140623340  destination getter (move target's +0x78)              -> LocomotorEnv::destination
//   FUN_140559250  state setter (state+0x48 = v)                          -> set_state()
//   FUN_1404aaa40 / FUN_1402bab90  2D rotation (X-Z / X-Y planes)         -> rotate_xz/xy()
//
// FIRST CUT: the shared data model (LocomotorState + state enum), the deterministic primitives, and
// the Starship vfunc_6 control skeleton (dispatch + read/write framing + the self-contained states,
// velocity integration, terrain-follow clamp, and arrival FSM). The deep per-state physics
// (acceleration, turning, burst, docking, accel->velocity integration) and world queries (terrain
// height, max climb rate, destination, arrival threshold, listener events) are behind LocomotorEnv,
// to be lifted as their own sub-units. Validated host-side (state logic + math); the in-game
// DIFFTRACE stream (folds entity+0x78 + motion state) is the end-to-end oracle.
#pragma once
#include "eaw_types.h"
#include "entity_spine.h"

namespace eaw {

// LocomotorState (entity+0xa8) — the per-unit movement state record, shared by all locomotor types.
// Field offsets recovered from StarshipLocomotorBehaviorClass::vfunc_6 + FUN_1403a6b80.
struct LocomotorState {
    vec3 velocity;        // +0x14/+0x18/+0x1c — per-tick position delta
    vec3 prev_position;   // +0x2c/+0x30/+0x34 — position snapshot before the move
    u32  state = 0;       // +0x48 — LocomotorStateType
    u32  substate = 0;    // +0x50 — arrival sub-FSM (9 / 0xa / 0xb) + listener-event gating
    f32  accel_factor = 0.0f; // +0x54 — throttle (0..1) scaling acceleration; set by the state bodies
    f32  steer_blend = 0.0f;  // +0x58 — steering-damping blend; cleared each tick, set by accelerate
    i32  countdown = 0;   // +0x5c — per-state timer (states 4/5/8)
    f32  vert_step = 0.0f;// +0x60 — terrain-follow vertical step (cleared each tick)
    vec3 anchor_a;        // +0x88/+0x8c/+0x90 — docking interpolation start
    vec3 anchor_b;        // +0xa0/+0xa4/+0xa8 — docking interpolation target
};

// GameObjectType locomotor tuning block (entity+0x298) — the per-unit-type physics parameters the
// integrators read. Offsets recovered from FUN_1406224b0 / FUN_140370f00 / the state bodies.
struct LocomotorTemplate {
    f32 accel = 0.0f;            // +0x384 — base acceleration magnitude
    f32 drag = 1.0f;             // +0x398 — per-tick velocity multiplier (<=1)
    f32 max_speed = 1e30f;       // +0x37c — speed cap (FUN_140370f00 base path)
    f32 cruise_altitude = 0.0f;  // +0xdfc — preferred altitude (terrain follow / arrival)
    f32 turn_rate = 1e30f;       // +0x39c — max heading change per tick (used by turn/accel bodies)
};

// LocomotorStateType (LocomotorState+0x48). Names are best-effort from the Starship state machine.
enum class LocoState : u32 {
    Idle         = 0,  // at rest; on displacement from destination -> Accelerate
    Accelerate   = 1,  // spin up to cruise (FUN_140622b80)
    Turn         = 2,  // turning toward heading (FUN_140622e90)
    Cruise       = 3,  // same handler as Turn
    PostMoveWait = 4,  // countdown wait, then a virtual callback (vtable+0x298)
    Burst        = 5,  // impulse/jump burst (inline; rotation + RNG)
    Decelerate   = 6,  // winding down; when velocity hits zero -> Stopped
    Stopped      = 7,  // halted
    Docking      = 8,  // docking/landing interpolation (FUN_1404ec820 / FUN_1405c3d10)
};

struct LocomotorBehavior; // fwd

// The deep per-state physics + world queries the Starship state machine calls into. The
// accel->velocity integrator (integrate_accel) is now lifted (calls integrate_velocity below); the
// remaining steering-decision bodies (accelerate/turn/burst/docking) are still future lifts and
// default to inert so the skeleton + tests run in isolation.
// Heading-angle unit conversion used by the integrator's rotations. The binary computes it as
// (DAT_1408007dc * DAT_1408007d4) / DAT_1408007f4 = (PI * 2) / 360 = 2*PI/360 — i.e. degrees->radians.
// So the entity heading fields (+0x88/+0x8c) are in DEGREES. Computed in float to match the binary.
inline constexpr f32 DEG2RAD = (3.14159274f * 2.0f) / 360.0f;
// Steering-damping gain (DAT_1408754bc), read from the binary.
inline constexpr f32 STEER_GAIN = 1.0f;

struct LocomotorEnv {
    virtual ~LocomotorEnv() = default;
    // Heading->radians factor and steering gain, sourced from the binary (see constants above).
    f32 heading_scale = DEG2RAD;
    f32 steer_gain = STEER_GAIN;
    // Per-state physics (mutate LocomotorState; may read/write entity position).
    virtual void accelerate(LocomotorState&, GameObject&) {}        // state 1  (FUN_140622b80)
    virtual void turn(LocomotorState&, GameObject&) {}              // states 2,3 (FUN_140622e90)
    virtual void burst(LocomotorState&, GameObject&) {}             // state 5  (inline impulse)
    virtual void docking(LocomotorState&, GameObject&) {}           // state 8  (dock/land)
    // accel->velocity integrator (FUN_1406224b0) — LIFTED. Default applies it when the entity has a
    // locomotor template; tests override to inject a velocity directly.
    virtual void integrate_accel(LocomotorState& st, GameObject& e);
    // World queries.
    virtual vec3 destination(const LocomotorBehavior&) { return {}; }    // FUN_140623340
    virtual f32  terrain_height(f32 /*x*/, f32 /*y*/) { return 0.0f; }   // FUN_140135140
    virtual f32  max_climb_rate(GameObject&) { return 1e30f; }           // FUN_140372440 (default: no clamp)
    virtual f32  arrival_threshold(GameObject&) { return 0.0f; }         // bounding calc
    // Cross-entity effect: an arrival listener event (becomes a Phase-B command in the rewrite).
    virtual void fire_event(GameObject& /*entity*/, int /*code*/) {}     // FUN_140220ed0
    // PostMoveWait (state 4) completion callback (behavior vtable+0x298).
    virtual void post_move_callback(LocomotorBehavior&) {}
};

// LocomotorBehaviorClass view: a behavior on a GameObject (its owner) that moves it.
struct LocomotorBehavior {
    GameObject* owner = nullptr;   // +0x28 — the unit this locomotor moves
    u32 next_service_tick = 0;     // +0x30 — schedule gate (the spine reads this)
    u32 base_interval = 0;         // +0x34 — service interval added each reschedule
    LocomotorState state;          // entity+0xa8 (owned here for the lift)
};

// --- primitives ---

// FUN_1403a8f90 — write world position to entity+0x78 (skip if unchanged unless `force`); marks the
// transform dirty (entity+0x3a1 |= 1, i.e. flags bit 0x100). `force` takes the presentation path
// (DynamicTransformBehavior) instead, modeled as a no-op here.
void set_position(GameObject& e, const vec3& p, bool force = false);

// FUN_140559250
inline void set_state(LocomotorState& st, LocoState s) { st.state = static_cast<u32>(s); }

// LocomotorCommonClass::vfunc_6 — reschedule the behavior: next = base_interval + tick.
inline void reschedule(LocomotorBehavior& b, u32 tick) { b.next_service_tick = b.base_interval + tick; }

// FUN_1404aaa40 — rotate the (x,z) components of `v` by `angle` radians (X-Z plane).
void rotate_xz(vec3& v, f32 angle);
// FUN_1402bab90 — rotate the (x,y) components of `v` by `angle` radians (X-Y plane).
void rotate_xy(vec3& v, f32 angle);

// --- the integrator ---

// FUN_1406224b0 — the accel->velocity integrator. Rotates the template's base acceleration
// (scaled by state.accel_factor) by the entity heading, adds it to the velocity (state+0x14/18/1c),
// clamps to max_speed, applies the steering-alignment damping (when state.steer_blend > 0), and
// applies drag. `heading_scale` converts the heading from degrees to radians (DEG2RAD); `steer_gain`
// scales the damping. This is the deterministic producer of the per-tick velocity that drives
// entity+0x78.
void integrate_velocity(LocomotorState& st, const GameObject& e, const LocomotorTemplate& tpl,
                        f32 heading_scale, f32 steer_gain);

// StarshipLocomotorBehaviorClass::vfunc_6 — advance one unit's movement by one tick.
void starship_tick(LocomotorBehavior& b, GameObject& entity, u32 tick, LocomotorEnv& env);

} // namespace eaw
