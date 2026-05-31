// Tick clock / scheduler — lifted from StarWarsG.exe.
//
// Source functions (all operate on the global FrameClock singleton at DAT_140b0a320):
//   FUN_14027c360  sim-tick advance + ticks/sec meter            -> tick_advance()
//   FUN_14027c2f0  render-frame advance + frames/sec meter       -> render_frame_advance()
//   FUN_14027c330  real-time (wall-clock) accumulator            -> realtime_accumulate()
//   FUN_14027c5f0  per-frame scheduler: "should a sim tick run?" -> should_run_sim_tick()
// Driven once per main-loop iteration by FUN_14005d990 (WinMain loop).
//
// DAT_140b0a320 is the authoritative simulation tick: it is what the DIFFTRACE oracle keys on, and
// it is handed to the game-service / gsvc path as the tick argument. Render frame (+0x04) is a
// SEPARATE counter advanced on the present path; it does NOT affect sim state.
//
// This is a functional re-implementation (decomp_plan.md §1): clean fields, original offsets in
// comments for traceability. The real singleton is a ~0xc88-byte "frame manager"; FrameClock models
// only the clock + scheduler view of it.
#pragma once
#include "eaw_types.h"

namespace eaw {

// DAT_140b153f8 — session/networking mode. SP = {Single, Replay}; the others run the lockstep
// FrameSynchronizer path in should_run_sim_tick().
enum class SessionMode : u32 {
    Single   = 0,   // single-player / skirmish
    LanGame  = 1,   // \                                     .
    NetGame  = 2,   //  > multiplayer lockstep (1,2,4)
    Replay   = 3,   // replay playback — scheduled like SP
    InetGame = 4,   // /
};

// The lockstep frame-synchronizer (the MP branch of FUN_14027c5f0). It owns the per-player frame
// and command tables (original: the player list at DAT_140a16fd0 + the per-player arrays based at
// clock+0x54 / clock+0xc68). Lifting its body is Phase-4 (determinism surface) work — it is the
// hardest determinism constraint (lockstep MP / replay). For now the SP scheduler is complete and
// the MP path defers to this interface so the unit is honest about what is and isn't lifted.
struct IFrameSynchronizer {
    virtual ~IFrameSynchronizer() = default;
    // Returns true iff this client may advance to (current_tick + 1): i.e. it is not more than
    // MaxAhead frames ahead of the slowest peer AND every player's commands for the target frame
    // have been received. Mirrors the DAT_140b153f8 in {1,2,4} branch of FUN_14027c5f0.
    virtual bool may_advance(u32 current_tick, u32 max_ahead) = 0;
};

// Clock + scheduler view of the FrameClock singleton (global &DAT_140b0a320).
struct FrameClock {
    // --- counters ---
    u32 sim_tick        = 0;   // +0x00  DAT_140b0a320 — authoritative sim tick
    u32 render_frame    = 0;   // +0x04  render/present frame counter (sim-irrelevant)
    u32 max_ahead       = 0;   // +0x08  MP lockstep MaxAhead (frames)

    // --- scheduling ---
    bool ticked_this_frame = false; // +0x14  set true each frame a sim tick is advanced
    u32  fps_cap        = 0;   // +0x18  target tick rate (ticks/sec); 0 = uncapped
    u32  rate           = 1;   // +0x20  DAT_140b0a340 — ticks/sec used for tick->seconds conversion
    bool paused         = false; // +0x34 (low byte of [0xd]) — hard pause; no ticks
    u32  last_tick_time = 0;   // +0x50  wall-clock ms at the last advanced tick (cadence anchor)

    // --- fast-forward / speed flags (read by the SP scheduler) ---
    bool ff_fast        = false; // +0xb0b5 — fast-forward: clamp cap to 120 ticks/sec
    bool slow           = false; // +0x37   — slow mode: clamp cap to 30 ticks/sec

    // --- pause-edge / single-step debug facility (FUN_14027c5f0 +0xb0b0..b3) ---
    bool pause_pending       = false; // +0xb0b0 — paused, waiting for an explicit resume request
    bool resume_request      = false; // +0xb0b1 — one-shot resume (consumed each scheduler call)
    bool single_step_enabled = false; // +0xb0b2 — frame-advance debug mode
    bool single_step_armed   = false; // +0xb0b3 — armed within single-step

    // --- ticks/sec meter (FUN_14027c360) ---
    u32 sim_fps              = 0;   // +0x1c  ticks advanced in the last ~1s window
    u32 sim_fps_anchor_tick  = 0;   // +0x38  sim_tick at the last meter sample
    u32 sim_fps_anchor_time  = 0;   // +0x3c  wall-clock ms at the last meter sample

    // --- frames/sec meter (FUN_14027c2f0) ---
    u32 render_fps             = 0; // +0x40
    u32 render_fps_anchor_frm  = 0; // +0x48
    u32 render_fps_anchor_time = 0; // +0x4c

    // --- real-time accumulator (FUN_14027c330) ---
    f32 realtime_seconds = 0.0f;    // +0xb09c  sum of frame dt / 1000.0
};

// Result of the per-frame scheduling decision (FUN_14027c5f0 out-params).
struct TickDecision {
    bool run_tick   = false; // should the sim advance one tick this frame?
    u32  wait_ms    = 0;     // if not, ms until the next tick is due (SP cadence; *param_3)
    bool step_block = false; // single-step waiting on next frame's commands (*param_4)
};

// FUN_14027c5f0 — decide whether to run a sim tick this frame. `now` is wall-clock ms.
// `mode` selects SP fixed-timestep vs MP lockstep (the latter consults `sync`, which may be null
// for SP). On a positive decision, stamps last_tick_time = now.
TickDecision should_run_sim_tick(FrameClock& c, u32 now, SessionMode mode,
                                 IFrameSynchronizer* sync);

// FUN_14027c360 — advance the sim tick and update the ticks/sec meter. `now` is wall-clock ms.
void tick_advance(FrameClock& c, u32 now);

// FUN_14027c2f0 — advance the render frame and update the frames/sec meter.
void render_frame_advance(FrameClock& c, u32 now);

// FUN_14027c330 — accumulate wall-clock seconds. `dt_ms` is elapsed ms since the previous frame;
// `ms_per_second` is the original DAT_1408007f8 divisor (1000.0).
void realtime_accumulate(FrameClock& c, u32 dt_ms, f32 ms_per_second = 1000.0f);

} // namespace eaw
