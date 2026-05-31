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
                        f32 gamespeed, f32 steer_gain) {
    const f32 max_speed = tpl.max_speed;        // FUN_140370f00 base path (template+0x37c)

    if (st.accel_factor != 0.0f) {
        // Base acceleration along local +x, scaled by throttle, rotated into world by heading.
        vec3 accel{ tpl.accel * st.accel_factor, 0.0f, 0.0f };  // template+0x384 * state+0x54
        rotate_xz(accel, gamespeed * e.heading_yaw);            // entity+0x88
        rotate_xy(accel, gamespeed * e.heading_pitch);          // entity+0x8c

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
        integrate_velocity(st, e, *e.locomotor_template, gamespeed_scale, steer_gain);
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

} // namespace eaw
