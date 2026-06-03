// Host-side behavioral test for the Targeting team-targeting AIM GEOMETRY core (part 2 of IN behavior
// #7), lifted from FUN_14020acd0 / FUN_14020b6d0 / the 633ae0 face-command tail. The engine's atan2/
// sqrt/sin/cos are NOT bit-reproducible by libm, so these tests assert STRUCTURE + grouping + edge
// cases (the bit-exact-vs-binary check is the DTARG2 in-game oracle); magnitudes use a tolerance.
#include "../targeting_aim.h"
#include <cstdio>
#include <cmath>

using namespace sim;
using eaw::vec3;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

static bool near(float a, float b, float eps = 1e-3f) { return std::fabs(a - b) <= eps; }
static bool is_neg(float v) { return std::signbit(v); }

static void test_aim_angles_axes() {
    std::printf("test_aim_angles_axes\n");
    const vec3 o{0, 0, 0};

    // self == tgt: degenerate — yaw 0 (dx==dy==0), pitch 0 (dz==dx==0), roll 0.
    {
        AimAngles a = targeting_aim_angles(o, o);
        CHECK(a.roll == 0.0f);
        CHECK(a.yaw == 0.0f);
        CHECK(a.pitch == 0.0f);
    }
    // +x: yaw atan2(0,1)=0; pitch dz==0&&dx!=0 -> computed atan2(0,1)=0 -> -0.
    {
        AimAngles a = targeting_aim_angles(o, vec3{1, 0, 0});
        CHECK(near(a.yaw, 0.0f));
        CHECK(near(a.pitch, 0.0f));
        CHECK(a.roll == 0.0f);
    }
    // +y: yaw atan2(1,0) -> 90; pitch dz==0&&dx==0 -> exactly 0 (skip branch).
    {
        AimAngles a = targeting_aim_angles(o, vec3{0, 1, 0});
        CHECK(near(a.yaw, 90.0f));
        CHECK(a.pitch == 0.0f);
    }
    // -x: yaw atan2(0,-1) -> 180 (>=0, no +360 wrap).
    {
        AimAngles a = targeting_aim_angles(o, vec3{-1, 0, 0});
        CHECK(near(a.yaw, 180.0f));
    }
    // -y: yaw atan2(-1,0) -> -90 -> +360 -> 270 (exercises the negative-yaw normalize).
    {
        AimAngles a = targeting_aim_angles(o, vec3{0, -1, 0});
        CHECK(near(a.yaw, 270.0f));
        CHECK(a.yaw >= 0.0f && a.yaw < 360.0f);
    }
    // +z straight up: yaw 0 (dx==dy==0); pitch dz!=0 -> -(atan2(1,0)=90) -> -90.
    {
        AimAngles a = targeting_aim_angles(o, vec3{0, 0, 1});
        CHECK(a.yaw == 0.0f);
        CHECK(near(a.pitch, -90.0f));
    }
}

static void test_aim_angles_grouping_and_sign() {
    std::printf("test_aim_angles_grouping_and_sign\n");
    const vec3 o{0, 0, 0};

    // 45-degree heading: atan2(1,1) -> 45.
    {
        AimAngles a = targeting_aim_angles(o, vec3{1, 1, 0});
        CHECK(near(a.yaw, 45.0f));
        CHECK(a.pitch == 0.0f || near(a.pitch, 0.0f));  // dz==0,dx!=0 -> atan2(0,horiz)=0 -> -0
    }
    // pitch sign: target ABOVE (dz>0) gives negative pitch; BELOW (dz<0) gives positive (the ^sign flip).
    {
        AimAngles up   = targeting_aim_angles(o, vec3{1, 0, 1});
        AimAngles down = targeting_aim_angles(o, vec3{1, 0, -1});
        CHECK(is_neg(up.pitch));        // -(+45)
        CHECK(!is_neg(down.pitch));     // -(-45) = +45
        CHECK(near(up.pitch, -45.0f));
        CHECK(near(down.pitch, 45.0f));
        CHECK(near(up.pitch, -down.pitch));
    }
    // Translation-invariance: aim depends only on the delta, not absolute position.
    {
        AimAngles a = targeting_aim_angles(vec3{0, 0, 0}, vec3{3, 4, 0});
        AimAngles b = targeting_aim_angles(vec3{10, 20, 5}, vec3{13, 24, 5});
        CHECK(near(a.yaw, b.yaw));
        CHECK(near(a.pitch, b.pitch));
    }
}

static void test_wrap_180() {
    std::printf("test_wrap_180\n");
    CHECK(targeting_wrap_180(0.0f) == 0.0f);
    CHECK(targeting_wrap_180(90.0f) == 90.0f);
    CHECK(targeting_wrap_180(180.0f) == 180.0f);          // 180 not > 180 -> unchanged
    CHECK(targeting_wrap_180(181.0f) == -179.0f);         // 181 - 360
    CHECK(targeting_wrap_180(360.0f) == 0.0f);            // 360 - 360
    CHECK(targeting_wrap_180(540.0f) == 180.0f);          // 540 - 360
    CHECK(targeting_wrap_180(-180.0f) == 180.0f);         // -180 <= -180 -> +360 -> 180
    CHECK(targeting_wrap_180(-181.0f) == 179.0f);         // -181 + 360
    CHECK(targeting_wrap_180(720.0f) == 0.0f);            // two subtractions
    // result is always in (-180, 180]
    for (float d = -1000.0f; d <= 1000.0f; d += 7.5f) {
        float w = targeting_wrap_180(d);
        CHECK(w > -180.0f && w <= 180.0f);
    }
}

static void test_face_from_yaw() {
    std::printf("test_face_from_yaw\n");
    // yaw 0 -> turn 0 -> small-angle guard FALSE -> facing {1.0, 0.0(=turn), 0} exactly.
    {
        Facing f = targeting_face_from_yaw(0.0f);
        CHECK(f.x == 1.0f);
        CHECK(f.y == 0.0f);
        CHECK(f.z == 0.0f);
    }
    // yaw 90 -> turn ~pi/2 -> cos~0, sin~1.
    {
        Facing f = targeting_face_from_yaw(90.0f);
        CHECK(near(f.x, 0.0f));
        CHECK(near(f.y, 1.0f));
        CHECK(f.z == 0.0f);
    }
    // yaw 180 -> cos~-1, sin~0.
    {
        Facing f = targeting_face_from_yaw(180.0f);
        CHECK(near(f.x, -1.0f));
        CHECK(near(f.y, 0.0f));
    }
    // unit magnitude on the guarded path.
    {
        Facing f = targeting_face_from_yaw(37.0f);
        CHECK(near(f.x * f.x + f.y * f.y, 1.0f));
        CHECK(f.z == 0.0f);
    }
    // tiny yaw -> guard FALSE -> small-angle facing {1.0, turn, 0} exactly (no trig call).
    {
        float tiny = 1e-5f;
        Facing f = targeting_face_from_yaw(tiny);
        CHECK(f.x == 1.0f);                 // cos -> exactly 1.0
        CHECK(f.y != 0.0f && near(f.y, 0.0f));  // sin -> turn (the small-angle value)
        CHECK(f.z == 0.0f);
    }
}

int main() {
    std::printf("targeting_aim_test\n");
    test_aim_angles_axes();
    test_aim_angles_grouping_and_sign();
    test_wrap_180();
    test_face_from_yaw();
    if (g_fail == 0) std::printf("ALL PASS\n");
    else             std::printf("%d FAILURES\n", g_fail);
    return g_fail ? 1 : 0;
}
