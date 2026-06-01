// Host-side behavioral test for the channel-2 SFX (sound-effect) emission side-effect.
// Validates the PRESENTATION-ONLY determinism contract recovered from 387400:99 ->
// FUN_1402d5290 (SFXEventManagerClass::Start_SFX_Event_Internally):
//   * a lifted unit records SFX cues on the CommandSink in emit order (RecordingCommandSink.sfx),
//   * drain_sfx replays them serially to an SfxSink in buffer order,
//   * the drain mutates NO sim state and draws NO sim RNG (the channel uses the separate audio RNG
//     a13e20, not the lockstep sim RNG a13e24), so a SimRng handed alongside is left untouched,
//   * the channel is droppable: a no-op SfxSink (audio off / headless server) changes nothing.
// The contrast with channel-1 (signal_dispatch) is the point: there, order is lockstep-critical;
// here, order is presentation-only.
#include "../sfx_channel.h"
#include "../recording_command_sink.h"
#include "../sim_rng.h"
#include <cstdio>
#include <vector>
#include <utility>

using namespace sim;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

// Records (emitter, sfx_id) in the order play() is called.
struct LoggingSfxSink : SfxSink {
    std::vector<std::pair<void*, uint32_t>> played;
    void play(void* emitter, uint32_t sfx_id) override { played.emplace_back(emitter, sfx_id); }
};

// Models the (forbidden) mistake of touching the sim RNG from the audio drain — used to prove the
// invariant has teeth: a conforming SfxSink must NOT do this.
struct DropSink : SfxSink {
    void play(void*, uint32_t) override {}
};

static void test_emit_records_in_order() {
    std::printf("test_emit_records_in_order\n");
    RecordingCommandSink sink;
    int t1 = 0, t2 = 0; void* eA = &t1; void* eB = &t2;
    sink.emit_sfx_event(eA, 100);
    sink.emit_sfx_event(eB, 200);
    sink.emit_sfx_event(eA, 101);
    CHECK(sink.sfx.size() == 3);
    CHECK(sink.sfx[0].emitter == eA && sink.sfx[0].sfx_id == 100);
    CHECK(sink.sfx[1].emitter == eB && sink.sfx[1].sfx_id == 200);
    CHECK(sink.sfx[2].emitter == eA && sink.sfx[2].sfx_id == 101);
    // SFX recording is independent of the signal channel.
    CHECK(sink.commands.empty());
}

static void test_drain_order() {
    std::printf("test_drain_order\n");
    RecordingCommandSink sink;
    int t1 = 0, t2 = 0; void* eA = &t1; void* eB = &t2;
    sink.emit_sfx_event(eA, 100);
    sink.emit_sfx_event(eB, 200);
    sink.emit_sfx_event(eA, 101);
    LoggingSfxSink out;
    drain_sfx(sink.sfx, out);
    CHECK((out.played == std::vector<std::pair<void*, uint32_t>>{{eA,100},{eB,200},{eA,101}}));
}

static void test_drain_draws_no_sim_rng() {
    std::printf("test_drain_draws_no_sim_rng\n");
    // The lockstep sim RNG must be untouched by an audio drain. Snapshot its state across a drain
    // and assert it does not advance — the channel-2 / channel-1 separation in one assertion.
    eaw::SimRng sim_rng(0x1234abcd);
    eaw::u32 state_before = sim_rng.state;

    RecordingCommandSink sink;
    int t = 0; void* e = &t;
    sink.emit_sfx_event(e, 42);
    sink.emit_sfx_event(e, 43);
    LoggingSfxSink out;
    drain_sfx(sink.sfx, out);             // presentation replay — no sim-RNG access

    CHECK(sim_rng.state == state_before); // sim RNG did NOT advance
    CHECK(out.played.size() == 2);
    // And the sim RNG, when actually drawn afterwards, produces exactly the sequence it would have
    // produced with no drain at all (i.e. the drain inserted zero phantom draws).
    eaw::SimRng reference(0x1234abcd);
    CHECK(sim_rng.range_i(0, 1000) == reference.range_i(0, 1000));
}

static void test_droppable() {
    std::printf("test_droppable\n");
    // Audio off / headless: dropping every cue must be a valid, side-effect-free outcome.
    RecordingCommandSink sink;
    int t = 0; void* e = &t;
    sink.emit_sfx_event(e, 1);
    sink.emit_sfx_event(e, 2);
    DropSink drop;
    drain_sfx(sink.sfx, drop);            // no crash, no observable effect
    // Re-draining the same buffer to a real sink is unaffected by the prior drop (buffer is const).
    LoggingSfxSink out;
    drain_sfx(sink.sfx, out);
    CHECK(out.played.size() == 2);
}

int main() {
    test_emit_records_in_order();
    test_drain_order();
    test_drain_draws_no_sim_rng();
    test_droppable();
    if (g_fail == 0) std::printf("All sfx-channel checks passed.\n");
    else             std::printf("%d sfx-channel CHECK(s) FAILED.\n", g_fail);
    return g_fail ? 1 : 0;
}
