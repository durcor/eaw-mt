// Host-side differential/behavioral test for the lifted tick clock.
// Replays deterministic input streams and asserts the scheduler/counter semantics recovered from
// FUN_14027c360/2f0/330/5f0. This is the fast inner-loop oracle; the in-game DIFFTRACE stream
// (DAT_140b0a320) is the end-to-end check.
#include "../tick_clock.h"
#include <cstdio>
#include <cstdlib>

using namespace eaw;

static int g_fail = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::printf("  FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++g_fail; } } while (0)

// Drive the SP loop the way FUN_14005d990 does: each frame, ask should_run_sim_tick(now); if it
// says run, advance the tick. Returns how many ticks were advanced over the window.
static u32 run_sp_window(FrameClock& c, u32 cap, u32 start_ms, u32 end_ms, u32 frame_dt_ms) {
    c.fps_cap = cap;
    u32 ticks = 0;
    for (u32 now = start_ms; now <= end_ms; now += frame_dt_ms) {
        TickDecision d = should_run_sim_tick(c, now, SessionMode::Single, nullptr);
        if (d.run_tick) { tick_advance(c, now); ++ticks; }
    }
    return ticks;
}

static void test_fixed_timestep_cadence() {
    std::printf("test_fixed_timestep_cadence\n");
    // At cap=60 (interval 16ms) over 1000ms with a fast 1ms frame loop, we expect ~ 1000/16 = 62
    // ticks (floor cadence; first tick fires immediately since last_tick_time starts at 0 < now).
    FrameClock c;
    u32 ticks = run_sp_window(c, 60, 1, 1001, 1);
    // interval = 1000/60 = 16ms. Ticks fire when now-last >= 16. Allow the exact arithmetic.
    CHECK(ticks >= 61 && ticks <= 63);
    CHECK(c.sim_tick == ticks);

    // cap=30 -> interval 33ms -> ~ 1000/33 = 30 ticks over the same window.
    FrameClock c2;
    u32 t2 = run_sp_window(c2, 30, 1, 1001, 1);
    CHECK(t2 >= 29 && t2 <= 31);
}

static void test_cap_zero_uncapped() {
    std::printf("test_cap_zero_uncapped\n");
    // fps_cap == 0 => no cadence gate => one tick per frame (the loop runs flat-out).
    FrameClock c;
    u32 ticks = run_sp_window(c, 0, 1, 100, 1);  // 100 frames
    CHECK(ticks == 100);
    CHECK(c.sim_tick == 100);
}

static void test_ff_and_slow_clamp() {
    std::printf("test_ff_and_slow_clamp\n");
    // fast-forward clamps cadence to 120/s (interval 8ms) regardless of fps_cap.
    FrameClock c; c.ff_fast = true;
    u32 ff = run_sp_window(c, 60, 1, 1001, 1);
    CHECK(ff >= 124 && ff <= 126);            // ~1000/8

    // slow takes precedence (applied after ff in source) -> 30/s.
    FrameClock c2; c2.ff_fast = true; c2.slow = true;
    u32 sl = run_sp_window(c2, 60, 1, 1001, 1);
    CHECK(sl >= 29 && sl <= 31);              // ~1000/33
}

static void test_pause_blocks() {
    std::printf("test_pause_blocks\n");
    FrameClock c; c.fps_cap = 60; c.paused = true;
    u32 ticks = 0;
    for (u32 now = 1; now <= 1000; now += 1) {
        TickDecision d = should_run_sim_tick(c, now, SessionMode::Single, nullptr);
        if (d.run_tick) { tick_advance(c, now); ++ticks; }
    }
    CHECK(ticks == 0);
    CHECK(c.sim_tick == 0);
}

static void test_wait_ms_reported() {
    std::printf("test_wait_ms_reported\n");
    // After a tick at t=100 (cap 60, interval 16), a query at t=105 must report run=false and a
    // wait of ~11ms (last+interval - now = 116 - 105).
    FrameClock c; c.fps_cap = 60;
    TickDecision d0 = should_run_sim_tick(c, 100, SessionMode::Single, nullptr);
    CHECK(d0.run_tick);
    tick_advance(c, 100);
    TickDecision d1 = should_run_sim_tick(c, 105, SessionMode::Single, nullptr);
    CHECK(!d1.run_tick);
    CHECK(d1.wait_ms == 11);
}

static void test_ticks_per_second_meter() {
    std::printf("test_ticks_per_second_meter\n");
    // Run uncapped one tick/ms for ~2.5s; the meter should read ~1000 after each 1s window closes.
    FrameClock c;
    for (u32 now = 1; now <= 2500; ++now) tick_advance(c, now);
    // sim_fps is the count from the most recently closed window; expect close to 1000.
    CHECK(c.sim_fps >= 990 && c.sim_fps <= 1001);
    CHECK(c.sim_tick == 2500);
}

// Multiplayer lockstep: the scheduler must defer entirely to the synchronizer.
struct FakeSync : IFrameSynchronizer {
    u32 ceiling;   // may advance only while current_tick < ceiling
    explicit FakeSync(u32 c) : ceiling(c) {}
    bool may_advance(u32 current_tick, u32 /*max_ahead*/) override {
        return current_tick < ceiling;
    }
};

static void test_mp_lockstep_gate() {
    std::printf("test_mp_lockstep_gate\n");
    FrameClock c; c.fps_cap = 60; c.max_ahead = 4;
    FakeSync sync(5);  // peers allow up to tick 5
    u32 ticks = 0;
    for (u32 now = 1; now <= 1000; now += 1) {
        TickDecision d = should_run_sim_tick(c, now, SessionMode::NetGame, &sync);
        if (d.run_tick) { tick_advance(c, now); ++ticks; }
    }
    // Lockstep: advances 0->5 then stalls (peers gate it), ignoring the SP cadence entirely.
    CHECK(ticks == 5);
    CHECK(c.sim_tick == 5);

    // With no synchronizer supplied, an MP session must never advance (fail-safe).
    FrameClock c2; c2.fps_cap = 60;
    TickDecision d = should_run_sim_tick(c2, 50, SessionMode::NetGame, nullptr);
    CHECK(!d.run_tick);
}

static void test_render_frame_independent() {
    std::printf("test_render_frame_independent\n");
    // Render frame counter is independent of sim tick.
    FrameClock c;
    for (u32 now = 1; now <= 500; ++now) render_frame_advance(c, now);
    CHECK(c.render_frame == 500);
    CHECK(c.sim_tick == 0);  // no sim ticks advanced
    realtime_accumulate(c, 250);
    realtime_accumulate(c, 250);
    CHECK(c.realtime_seconds > 0.49f && c.realtime_seconds < 0.51f);
}

int main() {
    std::printf("== tick_clock host validation ==\n");
    test_fixed_timestep_cadence();
    test_cap_zero_uncapped();
    test_ff_and_slow_clamp();
    test_pause_blocks();
    test_wait_ms_reported();
    test_ticks_per_second_meter();
    test_mp_lockstep_gate();
    test_render_frame_independent();
    if (g_fail) { std::printf("\nFAILED: %d check(s)\n", g_fail); return 1; }
    std::printf("\nAll tick_clock checks passed.\n");
    return 0;
}
