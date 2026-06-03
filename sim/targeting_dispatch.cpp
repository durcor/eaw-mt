// sim/targeting_dispatch.cpp — deterministic core of TargetingBehaviorClass::vfunc_6's dispatcher
// (RVA 0x633a30). See targeting_dispatch.h for the full derivation.
#include "targeting_dispatch.h"

namespace sim {

int targeting_resolve_mode(const TargetingDispatchInputs& in) {
    // 633a30.c lines 12-21: controller present -> controller mode; else manager game-speed mode; else
    // the default-mode constant.
    if (in.has_controller) {
        return in.controller_mode;
    }
    return in.has_manager ? in.game_speed_mode : in.default_mode;
}

TargetingImpl targeting_dispatch(int mode) {
    // 633a30.c lines 23-31.
    if (mode == 1) return TargetingImpl::Mode1;
    if (mode == 2) return TargetingImpl::Mode2;
    return TargetingImpl::NoOp;
}

TargetingImpl targeting_dispatch_tick(const TargetingDispatchInputs& in) {
    return targeting_dispatch(targeting_resolve_mode(in));
}

} // namespace sim
