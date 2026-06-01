// Host-side behavioral test for UnitAIBehaviorClass::vfunc_6 (RVA 0x4f6070), the fourth in-slice IN
// sim behavior — the fog-of-war / sensor-visibility ticker. Validates:
//   * the sim-inactive guard (game->vfunc_0xf0 false → whole tick no-ops),
//   * BLOCK A throttle: refresh only when next_due <= now; next_due advances by a FIXED 300 (not now+300),
//   * BLOCK A transform-change detection: unchanged 12-float transform → no object_moved, cache untouched;
//     changed → object_moved fires once and the cache is recached,
//   * the first-pass init builds/zeroes the bit array exactly once,
//   * sensor-absent gating: Block A skipped, Block C notifies suppressed but the vis bits still update,
//   * BLOCK C edges: hidden->visible fires became_visible, visible->hidden fires became_hidden, a
//     steady state fires neither, and the bit is always written to the predicate result,
//   * fog-off clears the whole visibility array,
//   * null objects are skipped and the idx >= object_count guard early-returns.
#include "../unit_ai.h"
#include <cstdio>
#include <vector>

using namespace sim;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

// Recording env: drives every gate/predicate and logs the cross-boundary acts.
struct RecEnv : UnitAiEnv {
    bool active = true, sensor = true, fog = true, reveal_ok = false;
    std::vector<void*> objects;          // the global object list this env exposes
    std::vector<void*> visible_set;      // which of `objects` the predicate reports visible
    int  init_calls = 0;
    std::vector<void*> moved, vis_evt, hid_evt, reveal_evt;

    bool sim_active() override { return active; }
    bool sensor_present() override { return sensor; }
    bool fog_active() override { return fog; }
    int  object_count() override { return (int)objects.size(); }
    void* object_at(int idx) override { return (idx >= 0 && idx < (int)objects.size()) ? objects[idx] : nullptr; }
    bool visible(void*, void* obj) override {
        for (void* v : visible_set) if (v == obj) return true;
        return false;
    }
    void ensure_initialized(UnitAiState& st, int n) override {
        ++init_calls;
        st.vis.assign((size_t)n, 0);     // build + zero, matching FUN_1404f5cb0 + the zero loop
    }
    bool reveal_due(void*) override { return reveal_ok; }
    void reveal(void* o) override { reveal_evt.push_back(o); }
    void object_moved(void* o) override { moved.push_back(o); }
    void became_visible(void* o, void*) override { (void)o; vis_evt.push_back(o); }
    void became_hidden(void* o, void*) override { (void)o; hid_evt.push_back(o); }
};

static int g_owner_tag = 0;
static void* owner() { return &g_owner_tag; }

// An identity-ish 12-float transform; index 0 is the "knob" tests wiggle to force a change.
static void fill(eaw::f32 t[12], float knob) {
    for (int i = 0; i < 12; ++i) t[i] = (float)i;
    t[0] = knob;
}

static void test_sim_inactive_noop() {
    std::printf("test_sim_inactive_noop\n");
    UnitAiState st; st.next_due = 0;
    RecEnv env; env.active = false;
    eaw::f32 tf[12]; fill(tf, 0);
    unit_ai_tick(st, owner(), 1000, tf, env);
    CHECK(st.next_due == 0);            // throttle untouched
    CHECK(env.init_calls == 0);
    CHECK(env.moved.empty());
}

static void test_throttle_not_due() {
    std::printf("test_throttle_not_due\n");
    UnitAiState st; st.next_due = 500; st.initialized = true;
    RecEnv env; eaw::f32 tf[12]; fill(tf, 0);
    unit_ai_tick(st, owner(), 100, tf, env);   // now < next_due → Block A skipped
    CHECK(st.next_due == 500);
    CHECK(env.moved.empty());
}

static void test_throttle_fixed_phase_and_change() {
    std::printf("test_throttle_fixed_phase_and_change\n");
    UnitAiState st; st.next_due = 50; st.initialized = true;
    // cache currently all-zero; transform differs → change path.
    RecEnv env; eaw::f32 tf[12]; fill(tf, 7.0f);
    unit_ai_tick(st, owner(), 1000, tf, env);  // due (50 <= 1000)
    CHECK(st.next_due == 350);                 // 50 + 300 (FIXED phase, NOT 1000+300)
    CHECK(env.moved.size() == 1);              // transform changed → object_moved
    CHECK(st.cache[0] == 7.0f && st.cache[11] == 11.0f);   // recached

    // Same transform again next due tick → no change, no moved, just advance.
    st.next_due = 350;
    unit_ai_tick(st, owner(), 1000, tf, env);
    CHECK(st.next_due == 650);
    CHECK(env.moved.size() == 1);              // unchanged → no second moved
}

static void test_first_pass_init_once() {
    std::printf("test_first_pass_init_once\n");
    UnitAiState st; st.next_due = 0; st.initialized = false;
    RecEnv env; env.objects = {(void*)1, (void*)2, (void*)3};
    eaw::f32 tf[12]; fill(tf, 0);
    unit_ai_tick(st, owner(), 10, tf, env);
    CHECK(env.init_calls == 1);
    CHECK(st.initialized == true);
    CHECK(st.vis.size() == 3);
    // Second due tick: already initialized → no rebuild.
    st.next_due = 0;
    unit_ai_tick(st, owner(), 10, tf, env);
    CHECK(env.init_calls == 1);
}

static void test_visibility_edges() {
    std::printf("test_visibility_edges\n");
    void* A = (void*)0xA; void* B = (void*)0xB;
    UnitAiState st; st.next_due = 100000; st.initialized = true;  // keep Block A out of the way
    st.vis = {0, 0};
    RecEnv env; env.objects = {A, B};
    // Tick 1: A visible, B hidden → A is a 0->1 edge (became_visible), B steady-hidden (no notify).
    env.visible_set = {A};
    eaw::f32 tf[12]; fill(tf, 0);
    unit_ai_tick(st, owner(), 5, tf, env);
    CHECK(st.vis[0] == 1 && st.vis[1] == 0);
    CHECK(env.vis_evt.size() == 1);
    CHECK(env.hid_evt.empty());
    // Tick 2: A now hidden, B now visible → A 1->0 (became_hidden), B 0->1 (became_visible).
    env.visible_set = {B};
    unit_ai_tick(st, owner(), 6, tf, env);
    CHECK(st.vis[0] == 0 && st.vis[1] == 1);
    CHECK(env.vis_evt.size() == 2);
    CHECK(env.hid_evt.size() == 1);
    // Tick 3: unchanged (B visible, A hidden) → no new notifies, bits stable.
    unit_ai_tick(st, owner(), 7, tf, env);
    CHECK(env.vis_evt.size() == 2 && env.hid_evt.size() == 1);
    CHECK(st.vis[0] == 0 && st.vis[1] == 1);
}

static void test_sensor_absent_suppresses_notifies_but_sets_bits() {
    std::printf("test_sensor_absent_suppresses_notifies_but_sets_bits\n");
    void* A = (void*)0xA;
    UnitAiState st; st.next_due = 0; st.initialized = true; st.vis = {0};
    RecEnv env; env.sensor = false; env.objects = {A}; env.visible_set = {A};
    eaw::f32 tf[12]; fill(tf, 9.0f);
    unit_ai_tick(st, owner(), 10, tf, env);
    CHECK(st.next_due == 0);            // Block A gated off (no sensor) → no throttle advance
    CHECK(env.moved.empty());
    CHECK(st.vis[0] == 1);             // Block C still writes the bit
    CHECK(env.vis_evt.empty());        // but the edge notify is suppressed
}

static void test_fog_off_clears_array() {
    std::printf("test_fog_off_clears_array\n");
    UnitAiState st; st.next_due = 100000; st.initialized = true;
    st.vis = {1, 1, 1};
    RecEnv env; env.fog = false; env.objects = {(void*)1, (void*)2, (void*)3};
    eaw::f32 tf[12]; fill(tf, 0);
    unit_ai_tick(st, owner(), 5, tf, env);
    CHECK(st.vis[0] == 0 && st.vis[1] == 0 && st.vis[2] == 0);
    CHECK(env.vis_evt.empty() && env.hid_evt.empty());
}

static void test_null_skip_and_count_guard() {
    std::printf("test_null_skip_and_count_guard\n");
    void* A = (void*)0xA;
    UnitAiState st; st.next_due = 100000; st.initialized = true;
    st.vis = {0, 0, 0};                 // array sized 3
    RecEnv env;
    env.objects = {A, nullptr};         // only 2 objects, idx 1 is null
    env.visible_set = {A};
    eaw::f32 tf[12]; fill(tf, 0);
    unit_ai_tick(st, owner(), 5, tf, env);
    CHECK(st.vis[0] == 1);             // A processed
    CHECK(st.vis[1] == 0);             // null skipped (bit left as-is)
    CHECK(st.vis[2] == 0);             // idx 2 >= object_count(2) → early return, never touched
    CHECK(env.vis_evt.size() == 1);
}

static void test_reveal_gate() {
    std::printf("test_reveal_gate\n");
    UnitAiState st; st.next_due = 100000; st.initialized = true; st.vis.clear();
    RecEnv env; env.reveal_ok = true;
    eaw::f32 tf[12]; fill(tf, 0);
    unit_ai_tick(st, owner(), 5, tf, env);
    CHECK(env.reveal_evt.size() == 1);
    env.reveal_ok = false;
    unit_ai_tick(st, owner(), 6, tf, env);
    CHECK(env.reveal_evt.size() == 1);  // gate closed → no reveal
}

int main() {
    test_sim_inactive_noop();
    test_throttle_not_due();
    test_throttle_fixed_phase_and_change();
    test_first_pass_init_once();
    test_visibility_edges();
    test_sensor_absent_suppresses_notifies_but_sets_bits();
    test_fog_off_clears_array();
    test_null_skip_and_count_guard();
    test_reveal_gate();
    if (g_fail == 0) std::printf("All unit-ai checks passed.\n");
    else             std::printf("%d unit-ai CHECK(s) FAILED.\n", g_fail);
    return g_fail ? 1 : 0;
}
