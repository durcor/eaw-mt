// Tick clock / scheduler — implementation. See tick_clock.h for the source-function map.
#include "tick_clock.h"

namespace eaw {

// FUN_14027c360
void tick_advance(FrameClock& c, u32 now) {
    const u32 t = c.sim_tick + 1;          // +0x00  ++
    c.ticked_this_frame = true;            // +0x14
    c.sim_tick = t;

    // ticks/sec meter: on the first call seed the window; then every >1000ms emit ticks-this-window.
    if (c.sim_fps_anchor_time == 0) {      // +0x3c == 0  (first sample)
        c.sim_fps_anchor_tick = t;         // +0x38
        c.sim_fps_anchor_time = now;       // +0x3c
    }
    if (now - c.sim_fps_anchor_time > 1000) {
        c.sim_fps_anchor_time = now;                  // +0x3c
        c.sim_fps = t - c.sim_fps_anchor_tick;        // +0x1c = ticks in the last window
        c.sim_fps_anchor_tick = t;                    // +0x38
    }
    // (original also emits the "____Frame %d____" debug line when DAT_140a7c080 != 0; omitted.)
}

// FUN_14027c2f0 — structurally identical to tick_advance but on the render-frame counter.
void render_frame_advance(FrameClock& c, u32 now) {
    const u32 f = c.render_frame + 1;      // +0x04  ++
    c.render_frame = f;

    if (c.render_fps_anchor_time == 0) {   // +0x4c == 0
        c.render_fps_anchor_frm  = f;      // +0x48
        c.render_fps_anchor_time = now;    // +0x4c
    }
    if (now - c.render_fps_anchor_time > 1000) {
        c.render_fps_anchor_time = now;                       // +0x4c
        c.render_fps = f - c.render_fps_anchor_frm;           // +0x40
        c.render_fps_anchor_frm = f;                          // +0x48
    }
}

// FUN_14027c330
void realtime_accumulate(FrameClock& c, u32 dt_ms, f32 ms_per_second) {
    c.realtime_seconds += static_cast<f32>(dt_ms) / ms_per_second;  // +0xb09c
}

// FUN_14027c5f0
TickDecision should_run_sim_tick(FrameClock& c, u32 now, SessionMode mode,
                                 IFrameSynchronizer* sync) {
    TickDecision d;

    // Hard pause: never advance.  if ((char)param_1[0xd] != 0) return 0;
    if (c.paused)
        return d;

    const bool sp = (mode == SessionMode::Single || mode == SessionMode::Replay);

    if (!sp) {
        // --- Multiplayer lockstep (DAT_140b153f8 in {1,2,4}) ---
        // The full body walks the player list (DAT_140a16fd0) and the per-player frame/command
        // tables, refusing to advance past MaxAhead or before all peers' commands for the target
        // frame arrive (the "FrameSynchronizer waiting for player…" logs). Lifting that table walk
        // is Phase-4 (determinism surface) work; here we delegate the gate to the synchronizer.
        if (sync == nullptr || !sync->may_advance(c.sim_tick, c.max_ahead))
            return d;
        c.last_tick_time = now;            // +0x50
        d.run_tick = true;
        return d;
    }

    // --- Single-player / replay: fixed-timestep cadence ---
    // cap = fps_cap, clamped to 120 under fast-forward or 30 under slow mode (FF/speed flags).
    if (c.fps_cap != 0) {                  // +0x18 != 0
        u32 cap = c.fps_cap;
        if (c.ff_fast) cap = 120;          // +0xb0b5 -> 0x78
        if (c.slow)    cap = 30;           // +0x37   -> 0x1e
        const u32 interval = 1000u / cap;  // ms per tick
        if (now - c.last_tick_time < interval) {
            // not due yet; report ms remaining (matches *param_3 = (last - now) + interval)
            d.wait_ms = (c.last_tick_time - now) + interval;
            return d;
        }
    }

    // Pause-edge gate: if a soft-pause is latched and no resume was requested, stay paused;
    // otherwise consume the one-shot resume.   (+0xb0b0 / +0xb0b1)
    if (c.pause_pending && !c.resume_request)
        return d;
    c.resume_request = false;

    // Single-step (frame-advance) debug: block ONLY when armed AND the next frame's commands are
    // already buffered (original: FUN_1402c1250(&DAT_140b15690, sim_tick+1)); otherwise arm and
    // fall through to tick. The command buffer is not lifted yet, so `commands_ready` is a stub
    // returning false — faithful to the not-ready case (arm + tick). Wire the real check when the
    // command/turn manager (DAT_140b15690) is lifted.   (+0xb0b2 / +0xb0b3)
    if (c.single_step_enabled) {
        const bool commands_ready = false; // TODO(Phase-B): command_buffer_has(c.sim_tick + 1)
        if (c.single_step_armed && commands_ready) {
            d.step_block = true;
            return d;
        }
        c.single_step_armed = true;
    }

    c.last_tick_time = now;                // +0x50
    d.run_tick = true;
    return d;
}

} // namespace eaw
