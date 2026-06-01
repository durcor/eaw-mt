// Host-side behavioral test for DamageTrackingBehaviorClass::vfunc_6 (RVA 0x58bd80), the first
// in-slice IN sim behavior. Validates:
//   * effect durations decay by the per-tick `decay` amount each tick,
//   * an effect is removed when its new duration is NOT > 0 (exactly-0 removed),
//   * the channel-1 0x2d (kSigDamageEffectsCleared) signal is emitted EXACTLY on the empty
//     transition tick — never on an already-empty list, never while ≥1 effect survives,
//   * the rate readout = (tick_hz * accum) / interval and the accumulator resets each tick,
//   * the emit goes through the CommandSink seam (RecordingCommandSink) as a parameterless command
//     on the entity itself.
#include "../damage_tracking.h"
#include "../recording_command_sink.h"
#include <cstdio>
#include <cmath>

using namespace sim;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

static bool feq(float a, float b) { return std::fabs(a - b) <= 1e-5f * (1.0f + std::fabs(b)); }

// Count 0x2d emits in a recording sink, all attributed to `entity`, parameterless.
static int count_cleared(const RecordingCommandSink& s, void* entity) {
    int n = 0;
    for (const RecordedCommand& c : s.commands) {
        CHECK(c.sig_id == kSigDamageEffectsCleared);  // the only signal this behavior emits
        CHECK(c.emitter == entity);
        CHECK(!c.has_payload);                          // 0x2d is parameterless
        ++n;
    }
    return n;
}

static void test_decay_and_keep() {
    std::printf("test_decay_and_keep\n");
    int dummy = 0; void* ent = &dummy;
    DamageTrackingState st;
    st.effects = {{1, 10.0f}, {2, 4.0f}, {3, 7.0f}};
    RecordingCommandSink sink;
    damage_tracking_tick(st, ent, 3.0f, 5, 30.0f, sink);   // decay 3
    // all survive (10->7, 4->1, 7->4), order preserved, no emit.
    CHECK(st.effects.size() == 3);
    CHECK(st.effects[0].key == 1 && feq(st.effects[0].duration, 7.0f));
    CHECK(st.effects[1].key == 2 && feq(st.effects[1].duration, 1.0f));
    CHECK(st.effects[2].key == 3 && feq(st.effects[2].duration, 4.0f));
    CHECK(count_cleared(sink, ent) == 0);
}

static void test_partial_expiry_no_emit() {
    std::printf("test_partial_expiry_no_emit\n");
    int dummy = 0; void* ent = &dummy;
    DamageTrackingState st;
    st.effects = {{1, 2.0f}, {2, 9.0f}, {3, 3.0f}};
    RecordingCommandSink sink;
    damage_tracking_tick(st, ent, 3.0f, 5, 30.0f, sink);   // decay 3
    // key1 2->-1 removed; key3 3->0 removed (NOT >0); key2 9->6 survives.
    CHECK(st.effects.size() == 1);
    CHECK(st.effects[0].key == 2 && feq(st.effects[0].duration, 6.0f));
    CHECK(count_cleared(sink, ent) == 0);                  // list not empty → no emit
}

static void test_exactly_zero_removed() {
    std::printf("test_exactly_zero_removed\n");
    int dummy = 0; void* ent = &dummy;
    DamageTrackingState st;
    st.effects = {{7, 3.0f}};
    RecordingCommandSink sink;
    damage_tracking_tick(st, ent, 3.0f, 5, 30.0f, sink);   // 3->0, NOT >0 → removed → empty → emit
    CHECK(st.effects.empty());
    CHECK(count_cleared(sink, ent) == 1);
}

static void test_empty_transition_emits_once() {
    std::printf("test_empty_transition_emits_once\n");
    int dummy = 0; void* ent = &dummy;
    DamageTrackingState st;
    st.effects = {{1, 2.0f}, {2, 2.5f}};
    RecordingCommandSink sink;
    damage_tracking_tick(st, ent, 3.0f, 5, 30.0f, sink);   // both expire same tick → ONE emit
    CHECK(st.effects.empty());
    CHECK(count_cleared(sink, ent) == 1);
}

static void test_already_empty_no_emit() {
    std::printf("test_already_empty_no_emit\n");
    int dummy = 0; void* ent = &dummy;
    DamageTrackingState st;                                 // no effects
    RecordingCommandSink sink;
    damage_tracking_tick(st, ent, 3.0f, 5, 30.0f, sink);
    CHECK(st.effects.empty());
    CHECK(count_cleared(sink, ent) == 0);                  // already-empty list never enters the loop
}

static void test_emit_is_edge_triggered() {
    std::printf("test_emit_is_edge_triggered\n");
    int dummy = 0; void* ent = &dummy;
    DamageTrackingState st;
    st.effects = {{1, 5.0f}};
    RecordingCommandSink sink;
    damage_tracking_tick(st, ent, 3.0f, 5, 30.0f, sink);   // 5->2 survive, no emit
    CHECK(count_cleared(sink, ent) == 0);
    damage_tracking_tick(st, ent, 3.0f, 5, 30.0f, sink);   // 2->-1 expire → empty → ONE emit
    CHECK(st.effects.empty());
    CHECK(count_cleared(sink, ent) == 1);
    damage_tracking_tick(st, ent, 3.0f, 5, 30.0f, sink);   // already empty → no further emit
    CHECK(count_cleared(sink, ent) == 1);
}

static void test_rate_readout_and_accum_reset() {
    std::printf("test_rate_readout_and_accum_reset\n");
    int dummy = 0; void* ent = &dummy;
    DamageTrackingState st;
    st.damage_accum = 12.0f;
    RecordingCommandSink sink;
    damage_tracking_tick(st, ent, 1.0f, 4, 30.0f, sink);   // rate = 30*12/4 = 90; accum -> 0
    CHECK(feq(st.damage_rate, 90.0f));
    CHECK(feq(st.damage_accum, 0.0f));
    // next tick with a fresh accumulation
    st.damage_accum = 8.0f;
    damage_tracking_tick(st, ent, 1.0f, 4, 30.0f, sink);   // rate = 30*8/4 = 60
    CHECK(feq(st.damage_rate, 60.0f));
    CHECK(feq(st.damage_accum, 0.0f));
}

int main() {
    test_decay_and_keep();
    test_partial_expiry_no_emit();
    test_exactly_zero_removed();
    test_empty_transition_emits_once();
    test_already_empty_no_emit();
    test_emit_is_edge_triggered();
    test_rate_readout_and_accum_reset();
    if (g_fail == 0) std::printf("All damage-tracking checks passed.\n");
    else             std::printf("%d damage-tracking CHECK(s) FAILED.\n", g_fail);
    return g_fail ? 1 : 0;
}
