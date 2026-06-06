// sim/dynamic_transform.cpp — see dynamic_transform.h. Literal transcription of FUN_1400480f0 /
// FUN_1402cf8d0 / the inline 3ac530 Givens, and the FUN_1403ac530 rebuild; operand order preserved.
#include "dynamic_transform.h"
#include <cstring>

namespace sim {

using eaw::f32;
using eaw::u32;
using eaw::vec3;

static const f32 PI_F = 3.1415927410125732f;   // DAT_1408007dc (the float32 pi)

// The engine's small-angle guard (480f0/cf8d0/3ac530 all share it): if the exponent field of `angle`
// exceeds 0x1d000000 (|angle| above ~2^-69) take sin/cos, else use the linearization (angle, 1.0).
static inline void guarded_sc(f32 angle, SinFn sinf, CosFn cosf, f32& s, f32& c) {
    u32 bits;
    std::memcpy(&bits, &angle, sizeof bits);
    if (0x1d000000u < (bits & 0x7f800000u)) { s = sinf(angle); c = cosf(angle); }
    else                                    { s = angle;       c = 1.0f; }
}

// 480f0: per row, a=col0 b=col1 → col1' = b*c - a*s ; col0' = a*c + b*s.
void dyn_rot_cols01(f32 m[12], f32 angle, SinFn sinf, CosFn cosf) {
    f32 s, c; guarded_sc(angle, sinf, cosf, s, c);
    for (int r = 0; r < 3; ++r) {
        const int i = 4 * r;
        const f32 a = m[i], b = m[i + 1];
        m[i + 1] = b * c - a * s;
        m[i]     = a * c + b * s;
    }
}

// cf8d0: per row, a=col2 b=col1 → col2' = a*c - b*s ; col1' = b*c + a*s.
void dyn_rot_cols12(f32 m[12], f32 angle, SinFn sinf, CosFn cosf) {
    f32 s, c; guarded_sc(angle, sinf, cosf, s, c);
    for (int r = 0; r < 3; ++r) {
        const int i = 4 * r;
        const f32 b = m[i + 1], a = m[i + 2];
        m[i + 2] = a * c - b * s;
        m[i + 1] = b * c + a * s;
    }
}

// inline 3ac530 Givens: per row, a=col2 b=col0 → col2' = a*c + b*s ; col0' = b*c - a*s.
// (The decompile writes row 2 in the mirrored order col0'=col0*c-col2*s / col2'=col2*c+col0*s — the same
//  rotation; all three rows are uniform.)
void dyn_rot_cols20(f32 m[12], f32 angle, SinFn sinf, CosFn cosf) {
    f32 s, c; guarded_sc(angle, sinf, cosf, s, c);
    for (int r = 0; r < 3; ++r) {
        const int i = 4 * r;
        const f32 a = m[i + 2], b = m[i];
        m[i + 2] = a * c + b * s;
        m[i]     = b * c - a * s;
    }
}

void dynamic_transform_rebuild(f32 m[12], f32 euler_x_deg, f32 euler_y_deg, f32 euler_z_deg,
                               const vec3& pos, SinFn sinf, CosFn cosf) {
    // reset to identity rotation + translation = position (3ac530's prologue writes).
    m[0] = 1.0f; m[1] = 0.0f; m[2]  = 0.0f; m[3]  = pos.x;
    m[4] = 0.0f; m[5] = 1.0f; m[6]  = 0.0f; m[7]  = pos.y;
    m[8] = 0.0f; m[9] = 0.0f; m[10] = 1.0f; m[11] = pos.z;

    const f32 d2r = (PI_F * 2.0f) / 360.0f;          // (DAT_1408007dc·DAT_1408007d4)/DAT_1408007f4
    dyn_rot_cols01(m, d2r * euler_z_deg, sinf, cosf);   // 480f0 by euler_z (+0x98)
    dyn_rot_cols20(m, d2r * euler_y_deg, sinf, cosf);   // inline by euler_y (+0x94)
    dyn_rot_cols12(m, d2r * euler_x_deg, sinf, cosf);   // cf8d0  by euler_x (+0x90)
    dyn_rot_cols01(m, d2r * 90.0f,       sinf, cosf);   // 480f0 by const 90° (DAT_1408007ec)
}

} // namespace sim
