// sim/tests/spline_mover_test.cpp — host structural validation of the SimpleSpaceLocomotor vfunc_59
// cubic-Hermite mover core. Asserts the mathematical contract (endpoints, C1 velocity = derivative,
// Z pass-through, near-zero-velocity gate, the shared Hermite basis). Bit-exact-vs-binary is the
// DTVEL in-game oracle's job (same split as targeting_aim_test vs DTARG2).
#include "spline_mover.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace sim;

static int g_fail = 0;
static void check(bool ok, const char* msg) {
    if (!ok) { std::printf("  FAIL: %s\n", msg); ++g_fail; }
}
static bool close(eaw::f32 a, eaw::f32 b, eaw::f32 eps = 1e-3f) { return std::fabs(a - b) <= eps; }

int main() {
    std::printf("spline_mover_test\n");

    // ── 1. Hermite basis is the exact standard integer matrix ────────────────────────────────────────
    check(kHermite[0][0]==1 && kHermite[0][1]==0 && kHermite[0][2]==0 && kHermite[0][3]==0, "basis a-row");
    check(kHermite[1][0]==0 && kHermite[1][1]==0 && kHermite[1][2]==1 && kHermite[1][3]==0, "basis b-row");
    check(kHermite[2][0]==-3&& kHermite[2][1]==3 && kHermite[2][2]==-2&& kHermite[2][3]==-1,"basis c-row");
    check(kHermite[3][0]==2 && kHermite[3][1]==-2&& kHermite[3][2]==1 && kHermite[3][3]==1, "basis d-row");

    // Two waypoints in the XY plane. heading is +X at w0, +Y at w1; unit speeds; time gap 1.0.
    Waypoint w0{}; w0.pos={0,0,5};  w0.heading={1,0,0}; w0.speed=1.0f; w0.time=10.0f;
    Waypoint w1{}; w1.pos={10,4,5}; w1.heading={0,1,0}; w1.speed=1.0f; w1.time=11.0f;
    eaw::vec3 prev{ -1, -1, 7 };  // prev_pos: only z (=7) should survive into the output

    // ── 2. Endpoints: t=0 -> p0, t=1 -> p1 (XY); z always = prev_pos.z ────────────────────────────────
    SplineStep s0 = spline_step(w0, w1, 10.0f, prev);   // time == t0
    check(close(s0.pos.x, w0.pos.x) && close(s0.pos.y, w0.pos.y), "t=0 -> p0 (xy)");
    check(s0.pos.z == prev.z, "t=0 z = prev_pos.z");

    SplineStep s1 = spline_step(w0, w1, 11.0f, prev);   // time == t1
    check(close(s1.pos.x, w1.pos.x) && close(s1.pos.y, w1.pos.y), "t=1 -> p1 (xy)");
    check(s1.pos.z == prev.z, "t=1 z = prev_pos.z");

    // ── 3. Velocity == analytic derivative of the spline, /dT, at an interior t ───────────────────────
    // Numerically differentiate pos(t) and compare to the reported vel (the engine's C1 tangent field).
    eaw::f32 dT = w1.time - w0.time;
    eaw::f32 tm = 10.4f;
    SplineStep sm = spline_step(w0, w1, tm, prev);
    eaw::f32 h = 1e-3f;
    SplineStep sp = spline_step(w0, w1, tm + h, prev);
    SplineStep sn = spline_step(w0, w1, tm - h, prev);
    eaw::f32 num_vx = (sp.pos.x - sn.pos.x) / (2*h);   // d pos.x / d(time)
    eaw::f32 num_vy = (sp.pos.y - sn.pos.y) / (2*h);
    // pos is parameterized by t=(time-t0)/dT, so d pos/d time = (d pos/dt)/dT = reported vel. Match.
    check(close(sm.vel.x, num_vx, 5e-2f), "vel.x == d pos.x/d time");
    check(close(sm.vel.y, num_vy, 5e-2f), "vel.y == d pos.y/d time");
    check(sm.vel.z == 0.0f, "vel.z == 0 (2D spline)");
    (void)dT;

    // ── 4. speed == |vel_xy|, and the moving gate trips when speed > eps ──────────────────────────────
    check(close(sm.speed, std::sqrt(sm.vel.x*sm.vel.x + sm.vel.y*sm.vel.y)), "speed == |vel_xy|");
    check(sm.moving == (sm.speed > kSplineEps), "moving gate == (speed>eps)");

    // ── 5. Degenerate (zero-speed) waypoints -> near-zero velocity -> NOT moving ──────────────────────
    Waypoint z0{}; z0.pos={3,3,1}; z0.heading={1,0,0}; z0.speed=0.0f; z0.time=0.0f;
    Waypoint z1{}; z1.pos={3,3,1}; z1.heading={1,0,0}; z1.speed=0.0f; z1.time=1.0f;
    SplineStep sz = spline_step(z0, z1, 0.5f, prev);
    check(!sz.moving, "coincident zero-speed waypoints -> not moving");
    check(sz.speed <= kSplineEps, "coincident zero-speed -> speed <= eps");

    // ── 6. t clamps below 0 and above 1 (extrapolation guard) ─────────────────────────────────────────
    SplineStep below = spline_step(w0, w1, 9.0f, prev);    // time < t0  -> t clamps to 0
    check(close(below.pos.x, w0.pos.x) && close(below.pos.y, w0.pos.y), "time<t0 clamps to p0");
    SplineStep above = spline_step(w0, w1, 99.0f, prev);   // time > t1  -> t clamps to 1
    check(close(above.pos.x, w1.pos.x) && close(above.pos.y, w1.pos.y), "time>t1 clamps to p1");

    // ── 7. normalize() contract: unit length out, zero vector left untouched ──────────────────────────
    eaw::vec3 v{3,4,0};
    eaw::f32 len = spline_normalize(v);
    check(close(len, 5.0f), "normalize returns 3-4-5 length");
    check(close(v.x,0.6f) && close(v.y,0.8f) && v.z==0.0f, "normalize -> unit vector");
    eaw::vec3 zv{0,0,0};
    check(spline_normalize(zv)==0.0f && zv.x==0 && zv.y==0 && zv.z==0, "normalize(0) leaves zero");

    if (g_fail==0) std::printf("ALL PASS\n"); else std::printf("%d FAILED\n", g_fail);
    return g_fail==0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
