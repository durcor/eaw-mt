// Host-side behavioral test for EnergyPoolBehaviorClass::vfunc_6 (RVA 0x56c030), the second
// in-slice IN sim behavior. Validates:
//   * normal regen: value' = clamp(value + rate*dt, 0, max) with the engine's two-compare clamp,
//   * regen never exceeds max and never drops below 0 (the clamp branches),
//   * the disabled / no-pool guards return Ok with NO write and NO ui flag,
//   * the drain gate: SourceMissing returns NoDrainSource without writing; Draining force-zeroes the
//     pool; NotDraining behaves as regen; NoCapability behaves as regen,
//   * the UI-dirty flag is set only on a WRITE path and only when the object is locally selected.
#include "../energy_pool.h"
#include <cstdio>
#include <cmath>

using namespace sim;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

static bool feq(float a, float b) { return std::fabs(a - b) <= 1e-5f * (1.0f + std::fabs(b)); }

static void test_normal_regen() {
    std::printf("test_normal_regen\n");
    EnergyPoolState st; st.value = 10.0f;
    // value + rate*dt = 10 + 2*1.5 = 13, max 100 → 13
    EnergyResult r = energy_pool_tick(st, DrainDisposition::NoCapability, 2.0f, 1.5f, 100.0f, false);
    CHECK(r == EnergyResult::Ok);
    CHECK(feq(st.value, 13.0f));
    CHECK(st.ui_dirty == false);          // not locally selected → no UI flag even though we wrote
}

static void test_regen_caps_at_max() {
    std::printf("test_regen_caps_at_max\n");
    EnergyPoolState st; st.value = 98.0f;
    // 98 + 5*2 = 108 > max 100 → clamp to 100
    energy_pool_tick(st, DrainDisposition::NotDraining, 5.0f, 2.0f, 100.0f, false);
    CHECK(feq(st.value, 100.0f));
}

static void test_regen_floors_at_zero() {
    std::printf("test_regen_floors_at_zero\n");
    EnergyPoolState st; st.value = 1.0f;
    // negative rate (e.g. ambient drain): 1 + (-5)*1 = -4 < 0 → clamp to 0
    energy_pool_tick(st, DrainDisposition::NoCapability, -5.0f, 1.0f, 100.0f, false);
    CHECK(feq(st.value, 0.0f));
}

static void test_disabled_no_write() {
    std::printf("test_disabled_no_write\n");
    EnergyPoolState st; st.value = 42.0f; st.disabled = true;
    EnergyResult r = energy_pool_tick(st, DrainDisposition::NoCapability, 9.0f, 1.0f, 100.0f, true);
    CHECK(r == EnergyResult::Ok);
    CHECK(feq(st.value, 42.0f));          // unchanged
    CHECK(st.ui_dirty == false);          // guarded no-op never reaches the UI flag
}

static void test_no_pool_no_write() {
    std::printf("test_no_pool_no_write\n");
    EnergyPoolState st; st.value = 7.0f; st.has_pool = false;
    EnergyResult r = energy_pool_tick(st, DrainDisposition::NoCapability, 9.0f, 1.0f, 100.0f, true);
    CHECK(r == EnergyResult::Ok);
    CHECK(feq(st.value, 7.0f));
    CHECK(st.ui_dirty == false);
}

static void test_drain_source_missing() {
    std::printf("test_drain_source_missing\n");
    EnergyPoolState st; st.value = 55.0f;
    EnergyResult r = energy_pool_tick(st, DrainDisposition::SourceMissing, 9.0f, 1.0f, 100.0f, true);
    CHECK(r == EnergyResult::NoDrainSource);
    CHECK(feq(st.value, 55.0f));          // bails before any write
    CHECK(st.ui_dirty == false);
}

static void test_draining_zeroes_pool() {
    std::printf("test_draining_zeroes_pool\n");
    EnergyPoolState st; st.value = 80.0f;
    // Draining forces the pool to min(max,0) = 0 (max>=0).
    EnergyResult r = energy_pool_tick(st, DrainDisposition::Draining, 9.0f, 1.0f, 100.0f, true);
    CHECK(r == EnergyResult::Ok);
    CHECK(feq(st.value, 0.0f));
    CHECK(st.ui_dirty == true);           // drain IS a write path → UI flag (locally selected)
}

static void test_draining_no_pool() {
    std::printf("test_draining_no_pool\n");
    EnergyPoolState st; st.value = 80.0f; st.has_pool = false;
    EnergyResult r = energy_pool_tick(st, DrainDisposition::Draining, 9.0f, 1.0f, 100.0f, true);
    CHECK(r == EnergyResult::Ok);
    CHECK(feq(st.value, 80.0f));          // no pool → no write
    CHECK(st.ui_dirty == false);
}

static void test_not_draining_regens() {
    std::printf("test_not_draining_regens\n");
    EnergyPoolState st; st.value = 20.0f;
    // drain-capable but predicate true → normal regen: 20 + 3*1 = 23
    energy_pool_tick(st, DrainDisposition::NotDraining, 3.0f, 1.0f, 100.0f, false);
    CHECK(feq(st.value, 23.0f));
}

static void test_ui_flag_on_select() {
    std::printf("test_ui_flag_on_select\n");
    EnergyPoolState st; st.value = 10.0f;
    energy_pool_tick(st, DrainDisposition::NoCapability, 1.0f, 1.0f, 100.0f, true);  // selected
    CHECK(st.ui_dirty == true);           // wrote AND selected → flag set
}

int main() {
    test_normal_regen();
    test_regen_caps_at_max();
    test_regen_floors_at_zero();
    test_disabled_no_write();
    test_no_pool_no_write();
    test_drain_source_missing();
    test_draining_zeroes_pool();
    test_draining_no_pool();
    test_not_draining_regens();
    test_ui_flag_on_select();
    if (g_fail == 0) std::printf("All energy-pool checks passed.\n");
    else             std::printf("%d energy-pool CHECK(s) FAILED.\n", g_fail);
    return g_fail ? 1 : 0;
}
