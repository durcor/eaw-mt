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
    CHECK(approx(b.state.turn_rate, 0.05f));

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
    if (g_fail) { std::printf("\nFAILED: %d check(s)\n", g_fail); return 1; }
    std::printf("\nAll locomotor checks passed.\n");
    return 0;
}
