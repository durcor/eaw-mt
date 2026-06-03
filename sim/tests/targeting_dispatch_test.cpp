// Host-side behavioral test for TargetingBehaviorClass::vfunc_6's DISPATCHER (RVA 0x633a30), part 1 of
// the seventh in-slice IN sim behavior. Validates:
//   * the 3-way mode resolution: controller present -> controller mode; absent + manager -> game-speed
//     mode; absent + no manager -> default mode (and the fallback ORDER / which source wins),
//   * the 3-way dispatch: mode 1 -> Mode1, mode 2 -> Mode2, everything else -> NoOp,
//   * that controller_mode shadows the game-speed/default sources whenever a controller is present
//     (even if the game-speed/default ints would dispatch differently).
#include "../targeting_dispatch.h"
#include <cstdio>
#include <initializer_list>

using namespace sim;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

static void test_resolve_source_selection() {
    std::printf("test_resolve_source_selection\n");
    // controller present -> controller_mode wins, regardless of manager/default.
    {
        TargetingDispatchInputs in;
        in.has_controller = true;  in.controller_mode = 2;
        in.has_manager = true;     in.game_speed_mode = 1;  in.default_mode = 1;
        CHECK(targeting_resolve_mode(in) == 2);
    }
    // no controller, manager present -> game-speed mode wins over default.
    {
        TargetingDispatchInputs in;
        in.has_controller = false; in.controller_mode = 9;   // must be ignored
        in.has_manager = true;     in.game_speed_mode = 1;  in.default_mode = 2;
        CHECK(targeting_resolve_mode(in) == 1);
    }
    // no controller, no manager -> default mode.
    {
        TargetingDispatchInputs in;
        in.has_controller = false; in.controller_mode = 9;   // ignored
        in.has_manager = false;    in.game_speed_mode = 9;   // ignored
        in.default_mode = 2;
        CHECK(targeting_resolve_mode(in) == 2);
    }
}

static void test_dispatch_mapping() {
    std::printf("test_dispatch_mapping\n");
    CHECK(targeting_dispatch(1) == TargetingImpl::Mode1);
    CHECK(targeting_dispatch(2) == TargetingImpl::Mode2);
    // every other int -> NoOp (sample a spread incl. 0, negatives, and out-of-range positives).
    for (int m : {0, 3, 4, 0x20, -1, 100, 0x7fffffff}) {
        CHECK(targeting_dispatch(m) == TargetingImpl::NoOp);
    }
}

static void test_dispatch_tick_end_to_end() {
    std::printf("test_dispatch_tick_end_to_end\n");
    // controller mode 1 -> Mode1.
    {
        TargetingDispatchInputs in;
        in.has_controller = true; in.controller_mode = 1;
        CHECK(targeting_dispatch_tick(in) == TargetingImpl::Mode1);
    }
    // controller mode 2 -> Mode2.
    {
        TargetingDispatchInputs in;
        in.has_controller = true; in.controller_mode = 2;
        CHECK(targeting_dispatch_tick(in) == TargetingImpl::Mode2);
    }
    // controller mode 3 -> NoOp (the dispatcher's else branch).
    {
        TargetingDispatchInputs in;
        in.has_controller = true; in.controller_mode = 3;
        CHECK(targeting_dispatch_tick(in) == TargetingImpl::NoOp);
    }
    // no controller, manager game-speed 2 -> Mode2 (fallback path still dispatches).
    {
        TargetingDispatchInputs in;
        in.has_controller = false; in.has_manager = true; in.game_speed_mode = 2; in.default_mode = 1;
        CHECK(targeting_dispatch_tick(in) == TargetingImpl::Mode2);
    }
    // no controller, no manager, default 1 -> Mode1.
    {
        TargetingDispatchInputs in;
        in.has_controller = false; in.has_manager = false; in.default_mode = 1;
        CHECK(targeting_dispatch_tick(in) == TargetingImpl::Mode1);
    }
}

static void test_controller_shadows_fallback() {
    std::printf("test_controller_shadows_fallback\n");
    // A present controller reporting a NoOp mode must NOT fall through to a manager/default that would
    // have dispatched — the dispatcher reads exactly one source.
    TargetingDispatchInputs in;
    in.has_controller = true;  in.controller_mode = 0;   // NoOp
    in.has_manager = true;     in.game_speed_mode = 1;  in.default_mode = 2;
    CHECK(targeting_dispatch_tick(in) == TargetingImpl::NoOp);
}

int main() {
    std::printf("targeting_dispatch_test\n");
    test_resolve_source_selection();
    test_dispatch_mapping();
    test_dispatch_tick_end_to_end();
    test_controller_shadows_fallback();
    if (g_fail == 0) std::printf("ALL PASS\n");
    else             std::printf("%d FAILURES\n", g_fail);
    return g_fail ? 1 : 0;
}
