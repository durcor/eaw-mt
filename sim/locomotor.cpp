// Locomotor movement integrators — implementation. See locomotor.h for the source-function map.
#include "locomotor.h"
#include <cmath>
#include <cstring>

namespace eaw {

// True when |angle| is large enough that the engine takes the real cos/sin path. The original
// gates on the IEEE-754 exponent field (FUN_1404aaa40/FUN_1402bab90 test (bits & 0x7f800000) >
// 0x1d000000); below that, cos≈1 and sin≈angle (small-angle approximation).
static bool angle_needs_trig(f32 angle) {
    u32 bits;
    std::memcpy(&bits, &angle, sizeof(bits));
    return (bits & 0x7f800000u) > 0x1d000000u;
}

// FUN_1403a8f90
void set_position(GameObject& e, const vec3& p, bool force) {
    if (force || e.position != p) {
        e.position = p;
        if (force) {
            // DynamicTransformBehaviorClass::FUN_1403ac530(e, force) — presentation path (callout).
            return;
        }
        e.flags |= GO_FLAG_HAS_TRANSFORM;  // entity+0x3a1 |= 1  ==  flags bit 0x100 (transform dirty)
    }
}

// FUN_1404aaa40 — X-Z rotation. The original skips the cos/sin call for tiny angles (exponent
// threshold 0x1d000000): cos≈1, sin≈angle. We reproduce that small-angle fast path.
void rotate_xz(vec3& v, f32 angle) {
    f32 c = 1.0f, s = angle;
    if (angle_needs_trig(angle)) {
        c = std::cos(angle);
        s = std::sin(angle);
    }
    const f32 x0 = v.x, z0 = v.z;
    v.z = c * z0 - s * x0;
    v.x = s * z0 + c * x0;
}

// FUN_1402bab90 — X-Y rotation (same small-angle fast path).
void rotate_xy(vec3& v, f32 angle) {
    f32 c = 1.0f, s = angle;
    if (angle_needs_trig(angle)) {
        c = std::cos(angle);
        s = std::sin(angle);
    }
    const f32 x0 = v.x, y0 = v.y;
    v.y = c * y0 + s * x0;
    v.x = c * x0 - s * y0;
}

static bool velocity_is_zero(const LocomotorState& st) {
    return st.velocity.x == 0.0f && st.velocity.y == 0.0f && st.velocity.z == 0.0f;
}

static f32 vlen(const vec3& v) { return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

// Per-axis steering damp toward `target`, moved by `force` without overshoot (FUN_1406224b0
// lines 79-123, identical logic for x/y/z).
static f32 damp_axis(f32 cur, f32 target, f32 force) {
    if (cur < 0.0f || cur <= target) {
        if (cur < 0.0f && cur < target) {
            if (force <= cur - target) force = cur - target;
            return cur - force;
        }
        return cur;                 // current already at/above target on this side: no change
    }
    if (cur - target <= force) force = cur - target;
    return cur - force;
}

// FUN_1406224b0
void integrate_velocity(LocomotorState& st, const GameObject& e, const LocomotorTemplate& tpl,
                        f32 heading_scale, f32 steer_gain) {
    const f32 max_speed = tpl.max_speed;        // FUN_140370f00 base path (template+0x37c)

    if (st.accel_factor != 0.0f) {
        // Base acceleration along local +x, scaled by throttle, rotated into world by heading
        // (heading is in degrees; heading_scale = DEG2RAD).
        vec3 accel{ tpl.accel * st.accel_factor, 0.0f, 0.0f };  // template+0x384 * state+0x54
        rotate_xz(accel, heading_scale * e.heading_yaw);        // entity+0x88 (degrees)
        rotate_xy(accel, heading_scale * e.heading_pitch);      // entity+0x8c (degrees)

        // velocity += accel
        st.velocity.x += accel.x;
        st.velocity.y += accel.y;
        st.velocity.z += accel.z;

        // Speed cap: when over max_speed, scale velocity by (max^2 / speed^2).
        const f32 speed = vlen(st.velocity);
        if (max_speed * max_speed < speed * speed) {
            const f32 f = (max_speed * max_speed) / (speed * speed);
            st.velocity.x *= f;
            st.velocity.y *= f;
            st.velocity.z *= f;
        }

        // Steering-alignment damping (only when the accelerate body set a positive blend).
        if (st.steer_blend > 0.0f) {
            const f32 vmag = vlen(st.velocity);
            vec3 uvel = st.velocity;
            if (vmag != 0.0f) { uvel.x /= vmag; uvel.y /= vmag; uvel.z /= vmag; }

            const f32 amag = vlen(accel);
            vec3 uacc = accel;
            if (amag > 0.0f) { const f32 inv = 1.0f / amag; uacc.x *= inv; uacc.y *= inv; uacc.z *= inv; }

            const f32 g = steer_gain * st.steer_blend;          // DAT_1408754bc * state+0x58
            const f32 fx = (uvel.x - uacc.x) * g;
            const f32 fy = (uvel.y - uacc.y) * g;
            const f32 fz = (uvel.z - uacc.z) * g;
            // target component = |vel| * unit-accel (the velocity projected onto the accel direction)
            st.velocity.x = damp_axis(st.velocity.x, vmag * uacc.x, fx);
            st.velocity.y = damp_axis(st.velocity.y, vmag * uacc.y, fy);
            st.velocity.z = damp_axis(st.velocity.z, vmag * uacc.z, fz);
        }
    }

    // Drag (template+0x398): applied every tick, even when not accelerating.
    st.velocity.x *= tpl.drag;
    st.velocity.y *= tpl.drag;
    st.velocity.z *= tpl.drag;
}

// Default integrator hook: run the lifted integrate_velocity when the entity carries a locomotor
// template. Tests override this to inject a velocity directly.
void LocomotorEnv::integrate_accel(LocomotorState& st, GameObject& e) {
    if (e.locomotor_template)
        integrate_velocity(st, e, *e.locomotor_template, heading_scale, steer_gain);
}

// SimpleSpaceLocomotorBehaviorClass::vfunc_6 (0x626420)
void simplespace_tick(LocomotorBehavior& b, GameObject& entity, u32 tick, SimpleSpaceEnv& env) {
    reschedule(b, tick);                        // LocomotorCommonClass::vfunc_6 pre-step
    LocomotorState& st = b.state;

    // Special mode (vfunc_66 = state+0x260): hand off to the per-special-state handlers and return.
    if (env.special_mode(st)) {
        switch (static_cast<SsState>(st.state)) {
            case SsState::SpApproach:           // FUN_140626c80
            case SsState::SpDock:               // FUN_140627590
            case SsState::Sp28:                 // FUN_1405cd680
            case SsState::Sp2c:                 // FUN_1406269f0
                env.special_state(st.state, st, entity);
                break;
            default:
                break;
        }
        return;
    }

    // Normal path accepts only {Init, Moving, Arrived} (the 0x480001 state mask in the original).
    const SsState s = static_cast<SsState>(st.state);
    if (s != SsState::Init && s != SsState::Moving && s != SsState::Arrived)
        return;

    if (st.state == static_cast<u32>(SsState::Init))
        st.state = static_cast<u32>(SsState::Moving);

    if (st.state == static_cast<u32>(SsState::Moving)) {
        const vec3 oldpos = entity.position;
        vec3 newpos{}, newvel{};
        const bool complete = env.compute_move(st, entity, tick, newpos, newvel); // vfunc_59 (mover)
        set_position(entity, newpos);           // FUN_1403a8f90 — writes entity+0x78
        // Arrived when the move reports complete OR the position did not change this tick.
        if (complete || newpos == oldpos)
            st.state = static_cast<u32>(SsState::Arrived);
    } else { // Arrived
        env.decel_residual(st, entity);         // bleed residual heading speed toward 0
        st.speed = 0.0f;                        // *(state+0xec) = 0
    }

    env.engine_effects(entity, st);             // banking roll + engine glow/sound (presentation)
}

// FUN_140139800 — normalize a vec3 in place, return original length.
static f32 normalize3(vec3& v) {
    const f32 len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len > 0.0f) { const f32 inv = 1.0f / len; v.x *= inv; v.y *= inv; v.z *= inv; }
    return len;
}

// The 2D cubic-Hermite segment eval inside FUN_140625990.
vec3 hermite_spline_eval(const SplineNode& p0, const SplineNode& p1, u32 tick, f32 hold_z,
                         const f32 basis[16]) {
    const f32 arclen = p1.arc - p0.arc;
    if (!(arclen > 0.0f))                       // degenerate segment: no curve
        return { p0.pos.x, p0.pos.y, hold_z };

    f32 t = (static_cast<f32>(tick) - p0.arc) / arclen;
    if (t < 0.0f) t = 0.0f;
    else if (t > 1.0f) t = 1.0f;                // clamp to [0,1]

    // scaled endpoint tangents m_i = normalize(tangent_i) * (weight_i * arclen)
    vec3 m0 = p0.tangent, m1 = p1.tangent;
    normalize3(m0); normalize3(m1);
    const f32 s0 = p0.weight * arclen, s1 = p1.weight * arclen;
    m0.x *= s0; m0.y *= s0; m1.x *= s1; m1.y *= s1;

    const f32 t2 = t * t, t3 = t2 * t;
    auto axis = [&](f32 a0, f32 a1, f32 mm0, f32 mm1) -> f32 {
        const f32 v[4] = { a0, a1, mm0, mm1 };
        f32 c[4];
        for (int r = 0; r < 4; ++r)
            c[r] = basis[r * 4 + 0] * v[0] + basis[r * 4 + 1] * v[1]
                 + basis[r * 4 + 2] * v[2] + basis[r * 4 + 3] * v[3];
        return c[0] + c[1] * t + c[2] * t2 + c[3] * t3;
    };
    return { axis(p0.pos.x, p1.pos.x, m0.x, m1.x),
             axis(p0.pos.y, p1.pos.y, m0.y, m1.y),
             hold_z };                          // z held (out.z = owner's current z)
}

// FUN_1406269f0 — the 0x2c special-mode timed drift mover.
vec3 simplespace_drift_move(const vec3& owner_pos, const vec3& dir, int timer,
                            const std::vector<f32>& speed_table) {
    if (timer < 0x19 || timer >= 0x96)          // setup (<0x19) / cleanup (>=0x96): no movement
        return owner_pos;
    vec3 d = dir;                                // normalize state+0x14/18/1c
    const f32 m = vlen(d);
    if (m > 0.0f) { const f32 inv = 1.0f / m; d.x *= inv; d.y *= inv; d.z *= inv; }
    const f32 speed = (timer >= 0 && timer < static_cast<int>(speed_table.size()))
                          ? speed_table[static_cast<std::size_t>(timer)] : 0.0f;  // FUN_14049d400
    return { owner_pos.x + d.x * speed, owner_pos.y + d.y * speed, owner_pos.z + d.z * speed };
}

// FUN_140625990 straight-line branch + FUN_14041c000 (direction from heading angle).
vec3 simplespace_straight_move(const vec3& in_pos, f32 heading_deg, f32 speed) {
    const f32 a = heading_deg * DEG2RAD;        // heading degrees -> radians (FUN_14041c000)
    // cos/sin with the engine's small-angle fast path (FUN_140776150 cos / FUN_140776650 sin).
    f32 c = 1.0f, s = a;
    if (angle_needs_trig(a)) { c = std::cos(a); s = std::sin(a); }
    return { in_pos.x + c * speed, in_pos.y + s * speed, in_pos.z };  // Z unchanged (param_4[2]+0)
}

// StarshipLocomotorBehaviorClass::vfunc_6
void starship_tick(LocomotorBehavior& b, GameObject& entity, u32 tick, LocomotorEnv& env) {
    reschedule(b, tick);                       // LocomotorCommonClass::vfunc_6 pre-step
    LocomotorState& st = b.state;
    st.steer_blend = 0.0f;                     // state+0x58 cleared each tick (set by accelerate body)
    st.vert_step = 0.0f;                       // state+0x60 cleared each tick

    const vec3 p0 = entity.position;           // read entity+0x78
    bool run_integrator = true;                // bVar6

    switch (static_cast<LocoState>(st.state)) {
    case LocoState::Idle: {                     // case 0 — arrived check / depart
        const vec3 dest = env.destination(b);
        if (dest != p0) {                       // displaced from destination -> start moving
            st.accel_factor = 0.05f;            // 0x3d4ccccd
            set_state(st, LocoState::Accelerate);
        }
        break;
    }
    case LocoState::Accelerate:                 // case 1
        env.accelerate(st, entity);
        break;
    case LocoState::Turn:                       // cases 2,3
    case LocoState::Cruise:
        env.turn(st, entity);
        break;
    case LocoState::PostMoveWait:               // case 4 — countdown, then callback
        if (st.countdown > 0) {
            st.countdown -= 1;
            return;
        }
        env.post_move_callback(b);              // behavior vtable+0x298
        run_integrator = false;                 // LAB_140623bfd
        break;
    case LocoState::Burst:                      // case 5
        env.burst(st, entity);
        return;                                 // original returns after the burst impulse
    case LocoState::Decelerate:                 // case 6
        st.accel_factor = 0.0f;
        if (velocity_is_zero(st))
            set_state(st, LocoState::Stopped);
        break;
    case LocoState::Stopped:                    // case 7
        st.accel_factor = 0.0f;
        break;
    case LocoState::Docking:                    // case 8
        env.docking(st, entity);
        run_integrator = false;
        break;
    }

    // After the switch: a docking unit whose countdown reached 0 is done this tick.
    if (static_cast<LocoState>(st.state) == LocoState::Docking && st.countdown == 0)
        return;

    if (run_integrator)
        env.integrate_accel(st, entity);        // FUN_1406224b0: shapes accel -> velocity

    if (velocity_is_zero(st))                    // no motion this tick
        return;

    // --- core position integration (lines 211-241) ---
    // Horizontal advance by velocity; vertical driven toward (destination.z + terrain) at a
    // clamped rate (terrain/altitude following).
    vec3 np = p0;
    np.x += st.velocity.x;
    np.y += st.velocity.y;
    np.z += st.velocity.z;

    const f32 orig_z      = p0.z;
    const f32 target_h    = env.destination(b).z + env.terrain_height(np.x, np.y);
    const f32 max_rate    = env.max_climb_rate(entity);
    f32 dz = target_h - orig_z;
    if (dz >  max_rate) dz =  max_rate;          // clamp ascent
    if (dz < -max_rate) dz = -max_rate;          // clamp descent
    st.vert_step = dz;
    np.z += st.vert_step;

    if (static_cast<LocoState>(st.state) == LocoState::Docking) {
        // docking lerp handled by env.docking(); skeleton writes the integrated position.
        set_position(entity, np);
        return;
    }

    set_position(entity, np);                    // write entity+0x78

    // --- arrival FSM (lines 255-297): substate 0xa -> fire "arriving" (9) -> 0xb; while 0xb, when
    // within arrival_threshold of the destination, fire "arrived" (10) -> substate 9. ---
    if (st.substate == 0xa) {
        env.fire_event(entity, 9);
        st.substate = 0xb;
    } else if (st.substate != 0xb) {
        return;
    }
    const vec3 dest = env.destination(b);
    const f32 ddx = dest.x - entity.position.x;
    const f32 ddy = dest.y - entity.position.y;
    const f32 ddz = dest.z - entity.position.z;
    const f32 dist = std::sqrt(ddx * ddx + ddy * ddy + ddz * ddz);
    if (dist < env.arrival_threshold(entity)) {
        env.fire_event(entity, 10);
        st.substate = 9;
    }
}

// --- Fighter steering velocity producer (FUN_1405c9360) ---

vec3 fighter_heading_dir(f32 climb_deg, f32 heading_deg) {
    const f32 a = climb_deg   * DEG2RAD;   // entity+0x88 (binary negates then takes cos/sin: cos even,
    const f32 b = heading_deg * DEG2RAD;   // sin odd → +cos(a), −sin(a)); entity+0x8c not negated.
    const f32 ca = std::cos(a), sa = std::sin(a);
    const f32 cb = std::cos(b), sb = std::sin(b);
    return { ca * cb, ca * sb, -sa };
}

vec3 fighter_throttle_velocity(const vec3& cur_velocity, f32 climb_deg, f32 heading_deg,
                               f32 max_speed, f32& accel_budget) {
    // current speed magnitude (FUN_140776d48 = sqrtf)
    f32 speed = std::sqrt(cur_velocity.x * cur_velocity.x + cur_velocity.y * cur_velocity.y +
                          cur_velocity.z * cur_velocity.z);
    const f32 err = max_speed - speed;            // speed error toward the commanded max
    if (accel_budget <= std::fabs(err)) {         // can't close it this tick: step by the budget
        speed += (err >= 0.0f) ? accel_budget : -accel_budget;
        accel_budget = 0.0f;
    } else {                                      // reach the target exactly; consume |err| of budget
        speed += err;
        accel_budget -= std::fabs(err);
    }
    const vec3 d = fighter_heading_dir(climb_deg, heading_deg);
    return { d.x * speed, d.y * speed, d.z * speed };
}

} // namespace eaw
