// Host-side behavioral test for the DynamicTransform rebuild (b2′, FUN_1403ac530 + its Givens leaves
// FUN_1400480f0 / FUN_1402cf8d0). Asserts STRUCTURE: the Givens primitives are proper rotations, the
// rebuild keeps the rotation part orthonormal, and the translation column tracks position. The bit-exact-
// vs-binary check is the in-game oracle (DTDYN, to follow). sin/cos via libm here.
#include "../dynamic_transform.h"
#include <cstdio>
#include <cmath>

using namespace sim;
using eaw::vec3;
using eaw::f32;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

static bool near(f32 a, f32 b, f32 eps = 1e-5f) { return std::fabs(a - b) <= eps; }
static f32 hsin(f32 x) { return std::sin(x); }   // host stand-ins for FUN_140776650/FUN_140776150
static f32 hcos(f32 x) { return std::cos(x); }

static void identity(f32 m[12]) {
    for (int i = 0; i < 12; ++i) m[i] = 0.0f;
    m[0] = m[5] = m[10] = 1.0f;
}

// rows of the 3×3 rotation part must be orthonormal (R·Rᵀ = I) — true for identity ∘ Givens rotations.
static bool orthonormal(const f32 m[12]) {
    const f32 r[3][3] = {{m[0],m[1],m[2]},{m[4],m[5],m[6]},{m[8],m[9],m[10]}};
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            f32 dot = r[i][0]*r[j][0] + r[i][1]*r[j][1] + r[i][2]*r[j][2];
            if (!near(dot, i == j ? 1.0f : 0.0f, 2e-5f)) return false;
        }
    return true;
}

static void test_givens_are_rotations() {
    std::printf("test_givens_are_rotations\n");
    const f32 q = 3.14159265f / 2.0f;   // 90°
    // cols(0,1) by 90°: R = [[0,-1,0],[1,0,0],[0,0,1]]  (about col-2 axis)
    { f32 m[12]; identity(m); dyn_rot_cols01(m, q, hsin, hcos);
      CHECK(near(m[0],0) && near(m[1],-1) && near(m[4],1) && near(m[5],0) && near(m[10],1));
      CHECK(orthonormal(m)); }
    // cols(1,2) by 90°: R = [[1,0,0],[0,0,-1],[0,1,0]]  (about col-0 axis)
    { f32 m[12]; identity(m); dyn_rot_cols12(m, q, hsin, hcos);
      CHECK(near(m[0],1) && near(m[6],-1) && near(m[9],1) && near(m[5],0) && near(m[10],0));
      CHECK(orthonormal(m)); }
    // cols(0,2) by 90°: R = [[0,0,1],[0,1,0],[-1,0,0]]  (about col-1 axis)
    { f32 m[12]; identity(m); dyn_rot_cols20(m, q, hsin, hcos);
      CHECK(near(m[2],1) && near(m[0],0) && near(m[8],-1) && near(m[10],0) && near(m[5],1));
      CHECK(orthonormal(m)); }
}

static void test_rebuild_translation() {
    std::printf("test_rebuild_translation\n");
    f32 m[12];
    const vec3 p{12.5f, -7.25f, 100.0f};
    dynamic_transform_rebuild(m, 30.0f, -15.0f, 75.0f, p, hsin, hcos);
    // translation column == position, bit-for-bit (copied, never touched by the rotations).
    CHECK(m[3] == p.x && m[7] == p.y && m[11] == p.z);
}

static void test_rebuild_orthonormal() {
    std::printf("test_rebuild_orthonormal\n");
    // arbitrary Euler poses → the rotation part stays orthonormal (composition of rotations).
    const f32 cases[][3] = {{0,0,0},{45,0,0},{0,90,0},{0,0,180},{33,-66,127},{-200,200,-200}};
    for (auto& e : cases) {
        f32 m[12];
        dynamic_transform_rebuild(m, e[0], e[1], e[2], vec3{1,2,3}, hsin, hcos);
        CHECK(orthonormal(m));
    }
}

static void test_rebuild_deterministic() {
    std::printf("test_rebuild_deterministic\n");
    f32 a[12], b[12];
    dynamic_transform_rebuild(a, 10, 20, 30, vec3{4,5,6}, hsin, hcos);
    dynamic_transform_rebuild(b, 10, 20, 30, vec3{4,5,6}, hsin, hcos);
    for (int i = 0; i < 12; ++i) CHECK(a[i] == b[i]);   // pure function ⇒ identical output
}

int main() {
    std::printf("=== dynamic_transform_test ===\n");
    test_givens_are_rotations();
    test_rebuild_translation();
    test_rebuild_orthonormal();
    test_rebuild_deterministic();
    if (g_fail == 0) std::printf("ALL PASS\n");
    else std::printf("%d FAIL\n", g_fail);
    return g_fail ? 1 : 0;
}
