// Common primitive typedefs for the lifted EaW sim core.
// Mirrors the widths the MSVC x64 binary uses; kept tiny and dependency-free so every sim/ unit
// and its host test compile standalone.
#pragma once
#include <cstdint>

namespace eaw {

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
using f32 = float;
using f64 = double;

// World-space 3-vector. The engine stores entity position as three consecutive floats at
// GameObjectClass+0x78/+0x7c/+0x80 (x/y/z); this mirrors that layout.
struct vec3 {
    f32 x = 0.0f, y = 0.0f, z = 0.0f;
    bool operator==(const vec3& o) const { return x == o.x && y == o.y && z == o.z; }
    bool operator!=(const vec3& o) const { return !(*this == o); }
};

} // namespace eaw
