// sim/geom_distance.cpp — implementation of the shared 3D squared-distance primitive (FUN_140397640),
// the bit-matchable numeric core of LureBehaviorClass::vfunc_6 (#9). See geom_distance.h for the asm and
// the FP-grouping rationale.
#include "geom_distance.h"

namespace sim {

eaw::f64 geom_dist_sq(const eaw::vec3& entity, const eaw::vec3& query) {
    // Deltas in float32 (subss). The binary evaluates them y, x, z; order is irrelevant to the result
    // (independent subtractions), only the add-grouping below matters.
    const eaw::f32 dx = entity.x - query.x;
    const eaw::f32 dy = entity.y - query.y;
    const eaw::f32 dz = entity.z - query.z;
    // Each square is a float32 multiply (mulss), then promoted to double (cvtps2pd); the sum accumulates
    // in double, grouped left-associatively ((dx²+dy²)+dz²) exactly as the binary does.
    return static_cast<eaw::f64>(dx * dx)
         + static_cast<eaw::f64>(dy * dy)
         + static_cast<eaw::f64>(dz * dz);
}

bool geom_within_range(const eaw::vec3& entity, const eaw::vec3& origin, eaw::f32 range) {
    // The binary computes range² in float32 (fVar1 * fVar1) then promotes to double for the `<=` compare
    // against the double squared distance.
    const eaw::f64 range_sq = static_cast<eaw::f64>(range * range);
    return geom_dist_sq(entity, origin) <= range_sq;
}

} // namespace sim
