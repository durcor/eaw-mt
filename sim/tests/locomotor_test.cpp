// Host-side behavioral test for the lifted Starship locomotor integrator.
// Validates the deterministic pieces recovered from StarshipLocomotorBehaviorClass::vfunc_6:
// the reschedule pre-step, position writer semantics, rotation math, state transitions, the
// countdown/velocity-zero early-outs, the terrain-follow vertical clamp, and the arrival FSM.
// Deep per-state physics is supplied by a controllable LocomotorEnv; the in-game DIFFTRACE stream
// (entity+0x78 + motion state) is the end-to-end oracle.
#include "../locomotor.h"
#include <cstdio>
#include <cmath>
#include <vector>

using namespace eaw;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)
static bool approx(f32 a, f32 b, f32 eps = 1e-4f) { return std::fabs(a - b) <= eps; }

struct FakeEnv : LocomotorEnv {
    vec3 dest{};
    f32 terrain = 0.0f;
    f32 climb = 1e30f;
    f32 threshold = 0.0f;
    std::vector<int> events;
    bool turn_called = false, accel_called = false, post_cb = false;

    vec3 destination(const LocomotorBehavior&) override { return dest; }
    f32  terrain_height(f32, f32) override { return terrain; }
    f32  max_climb_rate(GameObject&) override { return climb; }
    f32  arrival_threshold(GameObject&) override { return threshold; }
    void fire_event(GameObject&, int code) override { events.push_back(code); }
    void turn(LocomotorState&, GameObject&) override { turn_called = true; }
    void accelerate(LocomotorState&, GameObject&) override { accel_called = true; }
    void post_move_callback(LocomotorBehavior&) override { post_cb = true; }
    // integrate_accel left as no-op so a test-set velocity persists into the integration.
};

static void test_reschedule_prestep() {
    std::printf("test_reschedule_prestep\n");
    LocomotorBehavior b; b.base_interval = 30; b.state.state = (u32)LocoState::Stopped;
    GameObject e; FakeEnv env;
    starship_tick(b, e, 100, env);
    CHECK(b.next_service_tick == 130);   // base_interval + tick
}

static void test_set_position() {
    std::printf("test_set_position\n");
    GameObject e; e.position = {1, 2, 3};
    set_position(e, {1, 2, 3});                 // unchanged -> no write, no dirty
    CHECK((e.flags & GO_FLAG_HAS_TRANSFORM) == 0);
    set_position(e, {4, 5, 6});                 // changed -> write + dirty
    CHECK(e.position == (vec3{4, 5, 6}));
    CHECK((e.flags & GO_FLAG_HAS_TRANSFORM) != 0);
    GameObject e2; e2.position = {7, 8, 9};
    set_position(e2, {7, 8, 9}, /*force=*/true); // force path writes but does NOT set the dirty flag
    CHECK((e2.flags & GO_FLAG_HAS_TRANSFORM) == 0);
}

static void test_rotation_math() {
    std::printf("test_rotation_math\n");
    const f32 HALF_PI = 1.57079637f;
    vec3 v{1, 0, 0};
    rotate_xz(v, HALF_PI);   // (x,z): (1,0) rotated +90deg -> x->0, z->-1
    CHECK(approx(v.x, 0.0f) && approx(v.z, -1.0f) && approx(v.y, 0.0f));
    vec3 w{1, 0, 0};
    rotate_xy(w, HALF_PI);   // (x,y): (1,0) rotated +90deg -> x->0, y->+1
    CHECK(approx(w.x, 0.0f) && approx(w.y, 1.0f) && approx(w.z, 0.0f));
    // tiny-angle fast path: cos≈1, sin≈angle, no trig call.
    vec3 t{1, 0, 0};
    rotate_xz(t, 1e-6f);
    CHECK(approx(t.x, 1.0f) && approx(t.z, -1e-6f, 1e-7f));
}

static void test_idle_departs_on_displacement() {
    std::printf("test_idle_departs_on_displacement\n");
    LocomotorBehavior b; b.state.state = (u32)LocoState::Idle;
    GameObject e; e.position = {0, 0, 0};
    FakeEnv env; env.dest = {10, 0, 0};         // destination != position
    starship_tick(b, e, 1, env);
    CHECK(b.state.state == (u32)LocoState::Accelerate);
    CHECK(approx(b.state.accel_factor, 0.05f));

    LocomotorBehavior b2; b2.state.state = (u32)LocoState::Idle;
    GameObject e2; e2.position = {5, 5, 5};
    FakeEnv env2; env2.dest = {5, 5, 5};        // already at destination
    starship_tick(b2, e2, 1, env2);
    CHECK(b2.state.state == (u32)LocoState::Idle);  // no departure
}

static void test_postmovewait_countdown() {
    std::printf("test_postmovewait_countdown\n");
    LocomotorBehavior b; b.state.state = (u32)LocoState::PostMoveWait; b.state.countdown = 2;
    GameObject e; e.position = {1, 1, 1};
    FakeEnv env;
    starship_tick(b, e, 1, env);   // countdown 2 -> 1, early return
    CHECK(b.state.countdown == 1);
    CHECK(!env.post_cb);
    CHECK(e.position == (vec3{1, 1, 1}));
    starship_tick(b, e, 2, env);   // 1 -> 0, early return
    CHECK(b.state.countdown == 0);
    CHECK(!env.post_cb);
    starship_tick(b, e, 3, env);   // 0 -> callback fires, integrator skipped
    CHECK(env.post_cb);
}

static void test_decelerate_to_stopped() {
    std::printf("test_decelerate_to_stopped\n");
    LocomotorBehavior b; b.state.state = (u32)LocoState::Decelerate;
    b.state.velocity = {0, 0, 0};               // already stopped
    GameObject e; FakeEnv env;
    starship_tick(b, e, 1, env);
    CHECK(b.state.state == (u32)LocoState::Stopped);

    LocomotorBehavior b2; b2.state.state = (u32)LocoState::Decelerate;
    b2.state.velocity = {1, 0, 0};              // still moving -> stays Decelerate
    GameObject e2; FakeEnv env2; env2.dest = {0,0,0};
    starship_tick(b2, e2, 1, env2);
    CHECK(b2.state.state == (u32)LocoState::Decelerate);
}

static void test_velocity_zero_no_write() {
    std::printf("test_velocity_zero_no_write\n");
    LocomotorBehavior b; b.state.state = (u32)LocoState::Cruise; b.state.velocity = {0, 0, 0};
    GameObject e; e.position = {3, 3, 3};
    FakeEnv env; env.dest = {9, 9, 9};
    starship_tick(b, e, 1, env);
    CHECK(e.position == (vec3{3, 3, 3}));        // no motion -> position untouched
    CHECK((e.flags & GO_FLAG_HAS_TRANSFORM) == 0);
}

static void test_horizontal_integration() {
    std::printf("test_horizontal_integration\n");
    LocomotorBehavior b; b.state.state = (u32)LocoState::Cruise; b.state.velocity = {2, 3, 0};
    b.state.substate = 0;                        // arrival FSM idle
    GameObject e; e.position = {0, 0, 5};
    FakeEnv env; env.dest = {0, 0, 5};           // dest.z == orig z, terrain 0 -> no vertical step
    starship_tick(b, e, 1, env);
    CHECK(e.position == (vec3{2, 3, 5}));
    CHECK(approx(b.state.vert_step, 0.0f));
}

static void test_terrain_climb_clamp() {
    std::printf("test_terrain_climb_clamp\n");
    LocomotorBehavior b; b.state.state = (u32)LocoState::Cruise; b.state.velocity = {1, 0, 0};
    b.state.substate = 0;
    GameObject e; e.position = {0, 0, 0};
    FakeEnv env; env.dest = {0, 0, 100};         // wants +100 altitude
    env.terrain = 0.0f; env.climb = 5.0f;        // but climb rate capped at 5/tick
    starship_tick(b, e, 1, env);
    CHECK(approx(b.state.vert_step, 5.0f));      // clamped ascent
    CHECK(approx(e.position.z, 5.0f));           // z = 0 + vz(0) + 5

    LocomotorBehavior b2; b2.state.state = (u32)LocoState::Cruise; b2.state.velocity = {1, 0, 0};
    b2.state.substate = 0;
    GameObject e2; e2.position = {0, 0, 0};
    FakeEnv env2; env2.dest = {0, 0, -100}; env2.terrain = 0.0f; env2.climb = 5.0f;
    starship_tick(b2, e2, 1, env2);
    CHECK(approx(b2.state.vert_step, -5.0f));    // clamped descent
}

static void test_arrival_fsm() {
    std::printf("test_arrival_fsm\n");
    LocomotorBehavior b; b.state.state = (u32)LocoState::Cruise; b.state.velocity = {1, 0, 0};
    b.state.substate = 0xa;                      // pending "arriving"
    GameObject e; e.position = {0, 0, 5};
    FakeEnv env; env.dest = {1, 0, 5};           // unit lands exactly on the destination this tick
    env.threshold = 10.0f;                       // generous arrival radius
    starship_tick(b, e, 1, env);
    // substate 0xa -> fire 9 -> 0xb; then within threshold -> fire 10 -> 9.
    CHECK(env.events.size() == 2);
    CHECK(env.events[0] == 9 && env.events[1] == 10);
    CHECK(b.state.substate == 9);

    // substate not in {0xa,0xb}: arrival FSM does not run.
    LocomotorBehavior b2; b2.state.state = (u32)LocoState::Cruise; b2.state.velocity = {1, 0, 0};
    b2.state.substate = 0;
    GameObject e2; e2.position = {0, 0, 5};
    FakeEnv env2; env2.dest = {0, 0, 5}; env2.threshold = 10.0f;
    starship_tick(b2, e2, 1, env2);
    CHECK(env2.events.empty());
}

// --- integrate_velocity (FUN_1406224b0) ---

static void test_integrate_accel_add() {
    std::printf("test_integrate_accel_add\n");
    LocomotorState st; st.accel_factor = 1.0f; st.steer_blend = 0.0f;
    GameObject e; e.heading_yaw = 0; e.heading_pitch = 0;
    LocomotorTemplate tpl; tpl.accel = 2.0f; tpl.max_speed = 10.0f; tpl.drag = 1.0f;
    integrate_velocity(st, e, tpl, /*gamespeed=*/1.0f, /*steer_gain=*/0.0f);
    CHECK(approx(st.velocity.x, 2.0f) && approx(st.velocity.y, 0) && approx(st.velocity.z, 0));
}

static void test_integrate_speed_cap() {
    std::printf("test_integrate_speed_cap\n");
    LocomotorState st; st.accel_factor = 1.0f;
    GameObject e;
    LocomotorTemplate tpl; tpl.accel = 20.0f; tpl.max_speed = 5.0f; tpl.drag = 1.0f;
    integrate_velocity(st, e, tpl, 1.0f, 0.0f);
    // |vel| starts 20; cap scales by max^2/speed^2 = 25/400 -> vel.x = 20 * 0.0625 = 1.25.
    CHECK(approx(st.velocity.x, 1.25f));
}

static void test_integrate_drag() {
    std::printf("test_integrate_drag\n");
    LocomotorState st; st.accel_factor = 1.0f;
    GameObject e;
    LocomotorTemplate tpl; tpl.accel = 2.0f; tpl.max_speed = 100.0f; tpl.drag = 0.5f;
    integrate_velocity(st, e, tpl, 1.0f, 0.0f);
    CHECK(approx(st.velocity.x, 1.0f));   // (0 + 2) * 0.5

    // accel_factor == 0: no accel added, drag still applies.
    LocomotorState st2; st2.accel_factor = 0.0f; st2.velocity = {4, 0, 0};
    integrate_velocity(st2, e, tpl, 1.0f, 0.0f);
    CHECK(approx(st2.velocity.x, 2.0f));  // 4 * 0.5
}

static void test_integrate_heading_rotation() {
    std::printf("test_integrate_heading_rotation\n");
    LocomotorState st; st.accel_factor = 1.0f;
    GameObject e; e.heading_yaw = 1.57079637f;  // +90deg yaw rotates accel +x -> -z
    LocomotorTemplate tpl; tpl.accel = 2.0f; tpl.max_speed = 100.0f; tpl.drag = 1.0f;
    integrate_velocity(st, e, tpl, 1.0f, 0.0f);
    CHECK(approx(st.velocity.x, 0.0f) && approx(st.velocity.z, -2.0f));
}

static void test_integrate_steer_damp_aligned_noop() {
    std::printf("test_integrate_steer_damp_aligned_noop\n");
    // Velocity already aligned with the accel direction -> steering force zero -> no damping change.
    LocomotorState st; st.accel_factor = 1.0f; st.steer_blend = 0.5f; st.velocity = {3, 0, 0};
    GameObject e;
    LocomotorTemplate tpl; tpl.accel = 2.0f; tpl.max_speed = 100.0f; tpl.drag = 1.0f;
    integrate_velocity(st, e, tpl, 1.0f, /*steer_gain=*/1.0f);
    CHECK(approx(st.velocity.x, 5.0f));   // 3 + 2, undamped (aligned), drag 1
}

static void test_integrate_through_starship_tick() {
    std::printf("test_integrate_through_starship_tick\n");
    // End-to-end: a templated entity in Cruise produces velocity via the lifted integrator, which
    // the core integration then applies to entity+0x78.
    LocomotorTemplate tpl; tpl.accel = 2.0f; tpl.max_speed = 100.0f; tpl.drag = 1.0f;
    LocomotorBehavior b; b.state.state = (u32)LocoState::Cruise;
    b.state.accel_factor = 1.0f; b.state.substate = 0;
    GameObject e; e.position = {0, 0, 5}; e.locomotor_template = &tpl;
    FakeEnv env; env.dest = {0, 0, 5};   // dest.z == z, terrain 0 -> no vertical step
    // FakeEnv does NOT override integrate_accel, so the base lifted integrator runs.
    starship_tick(b, e, 1, env);
    CHECK(approx(b.state.velocity.x, 2.0f));    // integrator produced vx = accel
    CHECK(e.position == (vec3{2, 0, 5}));       // position advanced by velocity
}

// --- SimpleSpace dispatcher (FUN_140626420) ---

struct SsEnv : SimpleSpaceEnv {
    bool special = false;
    u32 special_seen = 0;
    bool move_complete = false;
    vec3 move_to{};
    int decel_calls = 0, effect_calls = 0;
    bool special_mode(const LocomotorState&) override { return special; }
    void special_state(u32 code, LocomotorState&, GameObject&) override { special_seen = code; }
    bool compute_move(LocomotorState&, GameObject&, u32, vec3& out_pos, vec3& out_vel) override {
        out_pos = move_to; out_vel = {}; return move_complete;
    }
    void decel_residual(LocomotorState&, GameObject&) override { ++decel_calls; }
    void engine_effects(GameObject&, LocomotorState&) override { ++effect_calls; }
};

static void test_ss_init_to_moving_and_apply() {
    std::printf("test_ss_init_to_moving_and_apply\n");
    LocomotorBehavior b; b.base_interval = 5; b.state.state = (u32)SsState::Init;
    GameObject e; e.position = {1, 2, 3};
    SsEnv env; env.move_complete = false; env.move_to = {4, 2, 3};
    simplespace_tick(b, e, 100, env);
    CHECK(b.next_service_tick == 105);                 // reschedule pre-step
    CHECK(b.state.state == (u32)SsState::Moving);       // Init -> Moving, stays Moving (not complete)
    CHECK(e.position == (vec3{4, 2, 3}));               // mover output applied
    CHECK((e.flags & GO_FLAG_HAS_TRANSFORM) != 0);
    CHECK(env.effect_calls == 1);
}

static void test_ss_arrives_on_complete() {
    std::printf("test_ss_arrives_on_complete\n");
    LocomotorBehavior b; b.state.state = (u32)SsState::Moving;
    GameObject e; e.position = {0, 0, 0};
    SsEnv env; env.move_complete = true; env.move_to = {9, 9, 9};
    simplespace_tick(b, e, 1, env);
    CHECK(e.position == (vec3{9, 9, 9}));
    CHECK(b.state.state == (u32)SsState::Arrived);
}

static void test_ss_arrives_on_no_motion() {
    std::printf("test_ss_arrives_on_no_motion\n");
    LocomotorBehavior b; b.state.state = (u32)SsState::Moving;
    GameObject e; e.position = {5, 5, 5};
    SsEnv env; env.move_complete = false; env.move_to = {5, 5, 5};  // mover returns same position
    simplespace_tick(b, e, 1, env);
    CHECK(b.state.state == (u32)SsState::Arrived);      // no displacement -> arrived
}

static void test_ss_arrived_decel() {
    std::printf("test_ss_arrived_decel\n");
    LocomotorBehavior b; b.state.state = (u32)SsState::Arrived; b.state.speed = 12.0f;
    GameObject e;
    SsEnv env;
    simplespace_tick(b, e, 1, env);
    CHECK(env.decel_calls == 1);
    CHECK(approx(b.state.speed, 0.0f));                 // *(state+0xec) cleared
}

static void test_ss_special_mode() {
    std::printf("test_ss_special_mode\n");
    LocomotorBehavior b; b.state.state = (u32)SsState::SpDock;
    GameObject e; e.position = {1, 1, 1};
    SsEnv env; env.special = true;
    simplespace_tick(b, e, 1, env);
    CHECK(env.special_seen == (u32)SsState::SpDock);    // dispatched to special handler
    CHECK(e.position == (vec3{1, 1, 1}));               // normal move path skipped
    CHECK(env.effect_calls == 0);
}

static void test_ss_invalid_state_noop() {
    std::printf("test_ss_invalid_state_noop\n");
    LocomotorBehavior b; b.state.state = 0x44;          // not in {0,0x13,0x16}, not special
    GameObject e; e.position = {2, 2, 2};
    SsEnv env; env.special = false;
    simplespace_tick(b, e, 1, env);
    CHECK(e.position == (vec3{2, 2, 2}));               // rejected -> no move, no effects
    CHECK(env.effect_calls == 0);
}

static void test_ss_straight_move() {
    std::printf("test_ss_straight_move\n");
    // heading 0deg -> +x; 90deg -> +y; z always unchanged.
    vec3 a = simplespace_straight_move({10, 20, 30}, 0.0f, 5.0f);
    CHECK(approx(a.x, 15.0f) && approx(a.y, 20.0f) && approx(a.z, 30.0f));
    vec3 b = simplespace_straight_move({10, 20, 30}, 90.0f, 5.0f);
    CHECK(approx(b.x, 10.0f) && approx(b.y, 25.0f) && approx(b.z, 30.0f));
    // the menu-demo cruise heading (cos=0.7973, sin=-0.6036 => ~ -37.15deg) at speed 750.
    vec3 c = simplespace_straight_move({-11941.301f, 8661.881f, -850.0f}, -37.1505f, 750.0f);
    CHECK(approx(c.x, -11941.301f + 597.95f, 0.3f));   // ~ +597.96 in x
    CHECK(approx(c.y, 8661.881f - 452.71f, 0.3f));     // ~ -452.71 in y
    CHECK(approx(c.z, -850.0f));                        // z held
}

static void test_ss_drift_move() {
    std::printf("test_ss_drift_move\n");
    std::vector<f32> table(150, 0.0f);
    table[30] = 750.0f;   // cruise plateau
    table[60] = 100.0f;   // ramp-down
    vec3 dir{0.8f, -0.6f, 0.0f};   // already unit
    // timer 30 (in [0x19,0x96)) -> move by dir*750
    vec3 a = simplespace_drift_move({1000, 2000, -850}, dir, 30, table);
    CHECK(approx(a.x, 1000 + 0.8f * 750) && approx(a.y, 2000 - 0.6f * 750) && approx(a.z, -850));
    // timer 60 -> dir*100
    vec3 b = simplespace_drift_move({0, 0, -850}, dir, 60, table);
    CHECK(approx(b.x, 80.0f) && approx(b.y, -60.0f) && approx(b.z, -850));
    // setup phase (timer < 0x19) -> no move
    vec3 c = simplespace_drift_move({5, 5, 5}, dir, 10, table);
    CHECK(c == (vec3{5, 5, 5}));
    // cleanup phase (timer >= 0x96) -> no move
    vec3 d = simplespace_drift_move({7, 7, 7}, dir, 200, table);
    CHECK(d == (vec3{7, 7, 7}));
    // non-unit dir is normalized first
    vec3 e = simplespace_drift_move({0, 0, 0}, {3, 4, 0}, 30, table);  // |dir|=5 -> (0.6,0.8)
    CHECK(approx(e.x, 0.6f * 750) && approx(e.y, 0.8f * 750));
}

static void test_hermite_spline_eval() {
    std::printf("test_hermite_spline_eval\n");
    // Standard cubic-Hermite basis in c0+c1t+c2t^2+c3t^3 form, rows = t^0..t^3, cols = p0,p1,m0,m1:
    //   p(t) = (2t^3-3t^2+1)p0 + (-2t^3+3t^2)p1 + (t^3-2t^2+t)m0 + (t^3-t^2)m1
    const f32 H[16] = {
        1.0f,  0.0f,  0.0f,  0.0f,   // t^0
        0.0f,  0.0f,  1.0f,  0.0f,   // t^1
       -3.0f,  3.0f, -2.0f, -1.0f,   // t^2
        2.0f, -2.0f,  1.0f,  1.0f,   // t^3
    };
    SplineNode p0, p1;
    p0.pos = {0, 0, 0};  p0.tangent = {1, 0, 0}; p0.weight = 1.0f; p0.arc = 100.0f;
    p1.pos = {10, 4, 0}; p1.tangent = {1, 0, 0}; p1.weight = 1.0f; p1.arc = 110.0f;

    // t=0 (tick == p0.arc) -> endpoint p0 (z held).
    vec3 a = hermite_spline_eval(p0, p1, 100, -850.0f, H);
    CHECK(approx(a.x, 0.0f) && approx(a.y, 0.0f) && approx(a.z, -850.0f));
    // t=1 (tick == p1.arc) -> endpoint p1.
    vec3 b = hermite_spline_eval(p0, p1, 110, -850.0f, H);
    CHECK(approx(b.x, 10.0f) && approx(b.y, 4.0f));
    // t=0.5 midpoint: with equal unit tangents (arclen 10, weight 1 -> m=±(10,0)).
    // Hermite at t=0.5: 0.5*(p0+p1) + 0.125*(m0-m1). m0=m1=(10,0) -> m0-m1=0 -> exactly the midpoint.
    vec3 c = hermite_spline_eval(p0, p1, 105, -850.0f, H);
    CHECK(approx(c.x, 5.0f) && approx(c.y, 2.0f));
    // clamp: tick beyond the segment clamps t to 1 (endpoint p1), not extrapolated.
    vec3 d = hermite_spline_eval(p0, p1, 200, -850.0f, H);
    CHECK(approx(d.x, 10.0f) && approx(d.y, 4.0f));
    // degenerate (zero-length) segment -> p0 position, z held.
    SplineNode q = p1; q.arc = p0.arc;
    vec3 e = hermite_spline_eval(p0, q, 100, -850.0f, H);
    CHECK(approx(e.x, 0.0f) && approx(e.y, 0.0f) && approx(e.z, -850.0f));
}

// FighterLocomotorBehaviorClass (vtable 0x8a6198) integrator. Golden triples (prev_pos, velocity,
// new_pos) lifted directly from the in-game battle capture (logs/battle_capture_*.log), one per
// flight state. The lifted fighter_integrate must reproduce new_pos = prev_pos + velocity exactly —
// this is the same equality the oracle confirmed on 7918/7918 flight-state ticks.
static void test_fighter_integrate() {
    struct Tri { vec3 p0, v, p1; const char* st; };
    const Tri golden[] = {
        // tick 106115 state 0x1c (Strafe)
        { {2758.191895f,4816.124512f,-300.060638f}, {-2.868669f,-7.574816f,-0.054032f}, {2755.323242f,4808.549805f,-300.114685f}, "0x1c" },
        // tick 106185 state 0x1b (Move)
        { {5181.729492f,5630.509766f,-0.066110f}, {-17.475063f,-21.031256f,-0.066454f}, {5164.254395f,5609.478516f,-0.132564f}, "0x1b" },
        // tick 110214 state 0x1e (Engage)
        { {3056.158203f,2013.334473f,-449.291321f}, {-1.223794f,-0.092313f,-0.081898f}, {3054.934326f,2013.242188f,-449.373230f}, "0x1e" },
    };
    for (const auto& g : golden) {
        vec3 np = fighter_integrate(g.p0, g.v);
        // capture stores positions at f32 precision; a 1-ULP-scale eps absorbs the print round-trip.
        CHECK(approx(np.x, g.p1.x, 1e-2f) && approx(np.y, g.p1.y, 1e-2f) && approx(np.z, g.p1.z, 1e-2f));
    }
}

// FighterLocomotorBehaviorClass velocity PRODUCER (FUN_1405c9360). Golden (speed, climb α, head β,
// velocity) samples from the battle capture — level, climbing, and steep-dive — confirm the lifted
// direction basis velocity = speed·(cosα cosβ, cosα sinβ, −sinα). Also exercises the throttle step.
static void test_fighter_velocity() {
    struct VTri { f32 speed, climb_deg, head_deg; vec3 v; };
    const VTri golden[] = {
        { 13.41350f,   0.00006f,  40.30302f, {10.22959f,  8.67625f, -0.00001f} }, // near-level
        {  8.79000f,  -8.78998f, 213.46562f, {-7.24664f, -4.79020f,  1.34323f} }, // shallow dive
        {  4.44175f, -54.99178f, 110.06250f, {-0.87415f,  2.39358f,  3.63810f} }, // steep dive (|α|<90)
    };
    for (const auto& g : golden) {
        vec3 d = fighter_heading_dir(g.climb_deg, g.head_deg);
        CHECK(approx(d.x * g.speed, g.v.x, 2e-2f) &&
              approx(d.y * g.speed, g.v.y, 2e-2f) &&
              approx(d.z * g.speed, g.v.z, 2e-2f));
    }
    // throttle: from rest, budget >= error -> reach max_speed exactly along heading; budget consumed.
    {
        f32 budget = 100.0f;
        vec3 v = fighter_throttle_velocity({0, 0, 0}, 0.0f, 90.0f, 12.0f, budget);
        // heading 90deg, level: dir=(0,1,0) -> v=(0,12,0); budget 100-12=88 left.
        CHECK(approx(v.x, 0.0f) && approx(v.y, 12.0f) && approx(v.z, 0.0f));
        CHECK(approx(budget, 88.0f));
    }
    // throttle: budget < error -> step by budget only, budget hits 0.
    {
        f32 budget = 3.0f;
        vec3 v = fighter_throttle_velocity({0, 0, 0}, 0.0f, 0.0f, 50.0f, budget);
        // heading 0, level: dir=(1,0,0); speed steps 0 -> 0+3 = 3; budget -> 0.
        CHECK(approx(v.x, 3.0f) && approx(v.y, 0.0f) && approx(v.z, 0.0f));
        CHECK(approx(budget, 0.0f));
    }
}

// Heading-integrator primitives — angle wraps, local-frame bearing (FUN_14020acd0), turn step
// (FUN_1405c8fb0). All deterministic pure math; goldens hand-derived from the formulas.
static void test_fighter_angle_wraps() {
    // wrap180 -> (-180, 180]
    CHECK(approx(wrap180(0.0f), 0.0f));
    CHECK(approx(wrap180(180.0f), 180.0f));      // boundary stays
    CHECK(approx(wrap180(-180.0f), 180.0f));     // -180 folds up to +180
    CHECK(approx(wrap180(190.0f), -170.0f));
    CHECK(approx(wrap180(360.0f), 0.0f));
    CHECK(approx(wrap180(350.0f), -10.0f));      // stored angle -> signed error form
    // wrap360 -> [0, 360)
    CHECK(approx(wrap360(0.0f), 0.0f));
    CHECK(approx(wrap360(360.0f), 0.0f));
    CHECK(approx(wrap360(370.0f), 10.0f));
    CHECK(approx(wrap360(-10.0f), 350.0f));
}

static void test_fighter_target_bearing() {
    // azimuth = atan2(y,x) deg in [0,360); elevation = -atan2(z, hypot(x,y)) deg; roll = 0.
    struct G { vec3 local; f32 az; f32 el; };
    const G gs[] = {
        { {1, 0, 0},  0.0f,   0.0f },   // straight ahead
        { {0, 1, 0},  90.0f,  0.0f },   // +y
        { {1, 1, 0},  45.0f,  0.0f },
        { {-1, 0, 0}, 180.0f, 0.0f },
        { {0, -1, 0}, 270.0f, 0.0f },   // -y folds to 270
        { {1, 0, 1},  0.0f,  -45.0f },  // +z below -> negative elevation
        { {1, 0, -1}, 0.0f,   45.0f },  // -z above -> positive elevation
    };
    for (const auto& g : gs) {
        fighter_bearing b = fighter_target_bearing(g.local);
        CHECK(approx(b.azimuth, g.az, 1e-3f));
        CHECK(approx(b.elevation, g.el, 1e-3f));
        CHECK(approx(b.roll, 0.0f));
    }
    // degenerate axes match the engine guards
    CHECK(approx(fighter_target_bearing({0, 0, 5}).azimuth, 0.0f));   // x==y==0 -> az 0
    CHECK(approx(fighter_target_bearing({0, 7, 0}).elevation, 0.0f)); // x==z==0 -> el 0
}

static void test_fighter_turn_angle() {
    // budget covers the error -> full step, |error| spent.
    { f32 b = 100.0f; CHECK(approx(fighter_turn_angle(0.0f, 10.0f, b), 10.0f));  CHECK(approx(b, 90.0f)); }
    // budget short -> step by budget in the error's direction, budget exhausted.
    { f32 b = 5.0f;   CHECK(approx(fighter_turn_angle(0.0f, 10.0f, b), 5.0f));   CHECK(approx(b, 0.0f)); }
    { f32 b = 5.0f;   CHECK(approx(fighter_turn_angle(0.0f, -10.0f, b), 355.0f)); CHECK(approx(b, 0.0f)); } // wrap360
    // stored angle wraps through 360.
    { f32 b = 100.0f; CHECK(approx(fighter_turn_angle(350.0f, 20.0f, b), 10.0f)); CHECK(approx(b, 80.0f)); }
    // stored 350 read as -10 (wrap180), exact-budget boundary uses the full-step branch.
    { f32 b = 20.0f;  CHECK(approx(fighter_turn_angle(350.0f, 20.0f, b), 10.0f)); CHECK(approx(b, 0.0f)); }
    // zero error -> no change, budget untouched.
    { f32 b = 5.0f;   CHECK(approx(fighter_turn_angle(42.0f, 0.0f, b), 42.0f));  CHECK(approx(b, 5.0f)); }
}

int main() {
    std::printf("== locomotor host validation ==\n");
    test_reschedule_prestep();
    test_set_position();
    test_rotation_math();
    test_idle_departs_on_displacement();
    test_postmovewait_countdown();
    test_decelerate_to_stopped();
    test_velocity_zero_no_write();
    test_horizontal_integration();
    test_terrain_climb_clamp();
    test_arrival_fsm();
    test_integrate_accel_add();
    test_integrate_speed_cap();
    test_integrate_drag();
    test_integrate_heading_rotation();
    test_integrate_steer_damp_aligned_noop();
    test_integrate_through_starship_tick();
    test_ss_init_to_moving_and_apply();
    test_ss_arrives_on_complete();
    test_ss_arrives_on_no_motion();
    test_ss_arrived_decel();
    test_ss_special_mode();
    test_ss_invalid_state_noop();
    test_ss_straight_move();
    test_ss_drift_move();
    test_hermite_spline_eval();
    test_fighter_integrate();
    test_fighter_velocity();
    test_fighter_angle_wraps();
    test_fighter_target_bearing();
    test_fighter_turn_angle();
    if (g_fail) { std::printf("\nFAILED: %d check(s)\n", g_fail); return 1; }
    std::printf("\nAll locomotor checks passed.\n");
    return 0;
}
