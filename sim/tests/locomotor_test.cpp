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
    if (g_fail) { std::printf("\nFAILED: %d check(s)\n", g_fail); return 1; }
    std::printf("\nAll locomotor checks passed.\n");
    return 0;
}
