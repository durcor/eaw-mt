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
    f32  speed = 0.0f;    // +0xec — speed/throttle scalar (SimpleSpace: drives engine glow + decel)
    u8   special_flag = 0;// +0x260 — SimpleSpace "special mode" gate (vfunc_66)
};

// SimpleSpaceLocomotorBehaviorClass state codes (LocomotorState+0x48) — a DIFFERENT enum from the
// Starship LocoState. Normal path: Init -> Moving -> Arrived. Special modes are gated by vfunc_66.
enum class SsState : u32 {
    Init    = 0x00,  // -> Moving on first tick
    Moving  = 0x13,  // compute+apply the move via the mover virtual (vfunc_59 = 0x625990)
    Arrived = 0x16,  // bleed off residual speed
    // special modes (entered when vfunc_66 / state+0x260 is set):
    SpApproach = 0x04, SpDock = 0x05, Sp28 = 0x28, Sp2c = 0x2c,
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

// The deep callouts of the SimpleSpace dispatcher. Unlike Starship (whose integrator is lifted),
// SimpleSpace delegates the whole move to a 2354-byte mover virtual (vfunc_59 = 0x625990) that this
// first cut leaves as `compute_move`. SimpleSpace is the COMMON space mover (the behavior census's
// representative space unit, and 100% of the menu-demo space ships). It stores a UNIT direction at
// state+0x14/18/1c with the speed scalar at state+0xec (≠ Starship's raw velocity).
struct SimpleSpaceEnv {
    virtual ~SimpleSpaceEnv() = default;
    virtual bool special_mode(const LocomotorState&) { return false; }          // vfunc_66 (state+0x260)
    virtual void special_state(u32 /*code*/, LocomotorState&, GameObject&) {}    // states 4/5/0x28/0x2c
    // vfunc_59 (0x625990): compute the unit's new position + velocity; return true when the move is
    // complete / the unit has arrived (FUN_1405c5910). LIFTING THIS is the bit-exact oracle step.
    virtual bool compute_move(LocomotorState&, GameObject&, u32 /*tick*/, vec3& out_pos, vec3& out_vel) {
        out_pos = {}; out_vel = {}; return true;
    }
    virtual void decel_residual(LocomotorState&, GameObject&) {}                 // Arrived residual decel
    virtual void engine_effects(GameObject&, LocomotorState&) {}                 // banking + engine glow/sound
};

// SimpleSpaceLocomotorBehaviorClass::vfunc_6 (0x626420) — the dispatch/state-machine skeleton.
void simplespace_tick(LocomotorBehavior& b, GameObject& entity, u32 tick, SimpleSpaceEnv& env);

// The straight-line / move-along-facing form of the SimpleSpace mover, with the direction from
// FUN_14041c000: the unit advances along its heading. heading is in DEGREES (entity+0x8c); Z held.
//   out_pos = in_pos + ( cos(heading*DEG2RAD), sin(heading*DEG2RAD), 0 ) * speed
// ✅ ORACLE-VALIDATED in-game (2026-05-30, menu-demo capture, tools/analyze_loco_oracle.py):
//   - the direction ( cos(hd*DEG2RAD), sin(hd*DEG2RAD), 0 ) reproduces the engine's state+0x14/18/1c
//     BIT-EXACT (<=1e-4) on 182 facing-driven cruise ticks, and
//   - displacement is exactly along it (disp ∥ dir, 150/150).
// CAVEAT: `speed` is the per-tick move magnitude supplied by the caller. It is NOT state+0xec (that
// is only a 0..1 throttle); the real magnitude is spline/special-mode driven (FUN_140625990 Hermite
// segments / the 0x2c special-mode mover FUN_1406269f0), which is a separate, larger lift. This
// function + its direction extraction are validated; the magnitude source is not yet lifted.
vec3 simplespace_straight_move(const vec3& in_pos, f32 heading_deg, f32 speed);

// SimpleSpace special-mode 0x2c mover (FUN_1406269f0) — the timed fly-in / drift animation the menu
// ships use. Over timer ticks [0x19, 0x96) the unit advances along its NORMALIZED facing direction
// (state+0x14/18/1c) at a table-driven speed; outside that window it does not move. `timer` is
// state+0x5c, pre-incremented each tick. `speed_table` is the runtime 150-float curve DAT_140b31440
// (FUN_14049d400 returns table[timer] for timer<0x96 else 0) — loaded from game data; the static
// image is zero, so the caller supplies the live table.
//   new_pos = owner_pos + normalize(dir) * speed_table[timer]
// ✅ ORACLE-VALIDATED (FULL POSITION) in-game (2026-05-30, menu-demo capture): the per-tick position
// reproduces the live binary bit-exact — direction (cos/sin, 183 ticks) AND magnitude
// (|disp| == table[timer], 123/123 move-window ticks). This is the first complete-position oracle
// pass: the lifted formula equals the engine's output for the dominant menu-ship mover.
vec3 simplespace_drift_move(const vec3& owner_pos, const vec3& dir, int timer,
                            const std::vector<f32>& speed_table);

// A spline control point — one 0x34-byte node of the SimpleSpace path (state+0x18 array, stride
// 0x34, current index state+0x60). Fields used by the mover: pos [0..2], tangent [3..5] (a direction,
// normalized at eval), weight [6], and arc-param [0xc] (the t-domain coordinate, compared to tick).
struct SplineNode {
    vec3 pos;
    vec3 tangent;
    f32  weight = 0.0f;
    f32  arc    = 0.0f;
};

// The 2D cubic-Hermite segment evaluation inside SimpleSpaceLocomotorBehaviorClass::vfunc_59
// (FUN_140625990): position along the curve between two control points at sim time `tick`. For each
// of x,y:  out = c0 + c1*t + c2*t^2 + c3*t^3, where t = clamp((tick - p0.arc)/(p1.arc-p0.arc), 0, 1)
// and c_r = basis_row_r · [p0.pos, p1.pos, m0, m1] with scaled endpoint tangents
// m_i = normalize(p_i.tangent) * (p_i.weight * arclen). Z is held (out.z = owner's current z).
// `basis` is the runtime 4x4 matrix DAT_140b2f1f0 (row r = the t^r coefficients; cols = p0,p1,m0,m1),
// row-major [16]; it is loaded at runtime (static image is zero), so the caller supplies the live
// matrix. Returns owner_pos unchanged for a degenerate (zero-length) segment.
vec3 hermite_spline_eval(const SplineNode& p0, const SplineNode& p1, u32 tick, f32 hold_z,
                         const f32 basis[16]);

// --- Fighter locomotor family (vtable 0x8a6198 = FighterLocomotorBehaviorClass) ---
//
// The DOMINANT real-space-combat mover: 82% of the ticks in a full capital-ship battle (the menu
// demo never exercises it; it only appears once units engage). Unlike SimpleSpace (planar, vz==0,
// unit-dir + scalar speed) this family stores RAW 3D per-tick velocity at state+0x14/18/1c — the
// same representation as Starship — and flies real banking maneuvers (a 3D heading at +0x84/88/8c).
//
// vfunc_6 = 0x5cb830 dispatches on state+0x48 in two halves:
//   - normal flight: a base LocomotorCommonClass::vfunc_6 pre-step + heading integration, then a
//     switch over the flight states, each a large per-state mover that ultimately commits
//       new_pos = owner_pos(+0x78/7c/80) + velocity(state+0x14/18/1c)   via FUN_1403a8f90.
//   - special modes (state 4/5/0x28/0x2c): approach / dock / drift — the 0x2c drift reuses the
//     already-lifted simplespace_drift_move (unit-dir * table[timer]).
//
// Flight states (state+0x48) and their movers, with battle-capture tick counts:
enum class FsState : u32 {
    Init   = 0x00,                 // -> 0x1b (or 0x1e if a target exists)
    Move   = 0x1b,                 // FUN_1405ce010   (2589 ticks) — straight cruise toward goal
    Strafe = 0x1c,                 // FUN_1405cd8e0   (1895 ticks) — pass / reposition
    Idle   = 0x1d,                 // terminal (no mover)
    Engage = 0x1e,                 // FUN_1405cc220   (3490 ticks) — intercept / attack-run (dominant)
    Break  = 0x1f,                 // FUN_1405cbea0 -> 0x1d
};

// FighterLocomotorBehaviorClass integrator — the per-tick position commit shared by every flight
// state (0x1b/0x1c/0x1e). Each per-state mover steers (updates state.velocity) then commits:
//   new_pos = owner_pos + velocity            (velocity = state+0x14/18/1c, raw 3D)
// ✅ ORACLE-VALIDATED (FULL POSITION) in-game (2026-05-31, "[NR] han trollo - space battle" capture,
// logs/battle_capture_*.log): per-tick position delta == captured velocity BIT-EXACT on 7918/7918
// flight-state ticks. The only non-matching fighter ticks (4) were transient 0x2c drift, which match
// the already-lifted simplespace_drift_move (|disp| == table[timer] == 750). The steering layer that
// PRODUCES the velocity (base vfunc_6 + the per-state target/throttle/turn math in
// FUN_1405ca390/caaf0/c8b70) is a separate, larger lift; this confirms the integrator + the velocity
// representation + the family identity.
inline vec3 fighter_integrate(const vec3& owner_pos, const vec3& velocity) {
    return { owner_pos.x + velocity.x, owner_pos.y + velocity.y, owner_pos.z + velocity.z };
}

// --- Fighter steering layer: how the velocity is PRODUCED each tick ---
//
// The per-state movers call a 3D pursuit-steering controller, FUN_1405caaf0, which:
//   1. builds the entity's inverse-orientation matrix from its angles (climb +0x88, heading +0x8c,
//      negated; DEG2RAD), transforms (target_pos − owner_pos) into the entity LOCAL frame, and
//   2. takes the yaw/pitch BEARING ERRORS to the target (FUN_14020acd0 = local-dir → angles), then
//   3. integrates the heading angles toward the target, turn-rate-limited:
//        FUN_1405c95a0 (yaw/+0x88), FUN_1405c8fb0 (pitch/+0x8c)  — rate from the unit template
//        (FUN_1403724d0/372560/372440), and
//   4. throttles the SPEED toward the commanded max and recomputes the velocity (FUN_1405c9360).
// The max-speed cap is FUN_1405ca390 → FUN_140370f00 (template+0x37c, same as Starship).
// LIFTED HERE: the deterministic velocity PRODUCER (step 4, FUN_1405c9360) + its direction basis,
// AND the deterministic primitives of steps 2-3 — the local-frame bearing math (FUN_14020acd0) and
// the turn-rate-limited angle step (FUN_1405c8fb0, the pitch integrator). The full FUN_1405caaf0
// orchestration on top (roll coupling, the ±180° hard-turn snap, and the game-speed-scaled per-tick
// turn budgets read from the unit template at order+0x38c/0x394/0x3a0) is documented below but NOT
// committed as lifted: those branches need the target + budgets + pitch captured in the DIFFTRACE
// harness to validate (it currently logs only heading +0x8c), so they stay a further sub-lift.

// FUN_1405c9360 direction basis — the unit velocity direction from the two orientation angles.
// climb α = entity+0x88, heading β = entity+0x8c, both in DEGREES. From the commit in 0x5c9360:
//   dir = ( cos α · cos β,  cos α · sin β,  −sin α )
// For level flight (α=0) this is exactly the SimpleSpace planar form (cos β, sin β, 0). The binary
// uses an IEEE small-angle fast path (cos≈1, sin≈θ below exponent 0x1d000000); std::cos/sin here is
// functionally equivalent. ✅ Validated in-game: the XY heading atan2(vy,vx)==β==hd on 7948/7965
// flight ticks, and the full 3D decomposition (α recovered from vz) reproduces vx,vy on 7952/7974;
// the ~0.3% residual is the cos α<0 (|α|>90°) branch (asin can't represent it) — consistent, not a miss.
vec3 fighter_heading_dir(f32 climb_deg, f32 heading_deg);

// FUN_1405c9360 — throttle the current speed toward `max_speed`, rate-limited by `accel_budget`
// (the remaining per-tick speed-change allowance, *param_3; consumed and returned via the ref), then
// set velocity = new_speed · fighter_heading_dir(climb, heading). Returns the new velocity that the
// mover commits to state+0x14/18/1c (and which fighter_integrate then adds to position).
vec3 fighter_throttle_velocity(const vec3& cur_velocity, f32 climb_deg, f32 heading_deg,
                               f32 max_speed, f32& accel_budget);

// --- Heading-integrator primitives (FUN_14020acd0 / FUN_1405c8fb0 + the angle wraps) ---

// Engine angle normalizers, both operating in DEGREES (the engine stores all orientation angles in
// degrees; cf. the velocity producer). The constants 180/-180/360 are DAT_1408524f8/fc/DAT_1408007f4.
//   wrap180 (FUN_14020b6d0): normalize to the half-open range (-180, 180] — the bearing-ERROR form.
//   wrap360 (FUN_14020b710): normalize to [0, 360) — the stored-angle form written back to the entity.
inline f32 wrap180(f32 deg) {
    while (deg > 180.0f)   deg -= 360.0f;
    while (deg <= -180.0f) deg += 360.0f;
    return deg;
}
inline f32 wrap360(f32 deg) {
    while (deg >= 360.0f) deg -= 360.0f;
    while (deg < 0.0f)    deg += 360.0f;
    return deg;
}

// Local-frame bearing to a target (FUN_14020acd0). Given the target offset already rotated into the
// entity's LOCAL frame (FUN_1405caaf0 first transforms target_pos − owner_pos by the inverse of the
// entity orientation), this returns the yaw/pitch bearing of that local direction:
//   azimuth   = atan2(local.y, local.x)  in degrees, wrapped to [0, 360)   (→ drives heading +0x8c)
//   elevation = −atan2(local.z, hypot(local.x, local.y))  in degrees       (→ drives climb   +0x88)
//   roll      = 0 (the engine zeroes index 0)
// FUN_1405caaf0 then takes wrap180(azimuth) / wrap180(elevation) as the signed yaw/pitch ERRORS it
// feeds to the integrators. Degenerate axes match the engine: azimuth=0 when local.x==local.y==0;
// elevation=0 when local.x==local.z==0.
struct fighter_bearing { f32 roll; f32 elevation; f32 azimuth; };
fighter_bearing fighter_target_bearing(const vec3& local_target);

// Turn-rate-limited angle step — the deterministic core of the heading integrators (exactly
// FUN_1405c8fb0, the pitch integrator; FUN_1405c95a0 is the same primitive plus roll coupling).
// `delta_deg` is the signed bearing error toward the target; `turn_budget` is the remaining per-tick
// turn allowance (degrees, *param_3). If the budget can't cover the whole error, step by the budget in
// the error's direction and exhaust it; otherwise step by the full error and consume |error| from the
// budget. The result is wrapped to [0, 360). `turn_budget` is updated in place. (A zero error leaves
// both the angle and the budget unchanged.)
f32 fighter_turn_angle(f32 cur_deg, f32 delta_deg, f32& turn_budget);

} // namespace eaw
