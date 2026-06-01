// sim/sfx_channel.h — Phase-3 channel-2: the SFX (sound-effect) emission side-effect + its drain.
//
// CORRECTION of the earlier "scheduled-event channel 2" framing. FUN_140387400's second emission
// (387400.c line 99) is:
//
//     FUN_1402d5290(&DAT_140b27e60, DAT_140b301c8, 0, 0, 0)
//       == SFXEventManagerClass::Start_SFX_Event_Internally(manager, sfx, ...)   (decomp 2d5290->2d5440)
//
// — a PRESENTATION audio cue (fired when an ordered fire-order completes), NOT a gameplay
// scheduled event. Three independent lines of evidence pin it as presentation, fully decoupled
// from the lockstep sim:
//
//   1. 2d5440 is literally SFXEventManagerClass::Start_SFX_Event_Internally — its body handles
//      ActiveSFXEventClass, SFX presets ("...which is a preset!"), looping events, and voice limits.
//   2. Its probability gate (2d5440:153) draws from &DAT_140a13e20 — a SEPARATE LCG state word
//      from the sim RNG &DAT_140a13e24. Across the whole decomp corpus, a13e24 is read only by core
//      sim fns (6236b0 locomotor, 387400 hardpoint, 3a6b80/3825b0 update spine); a13e20 is read only
//      by audio (2d5440) and is INDEPENDENTLY seeded in init (5d990:95 = `DAT_140a13e20 =
//      FUN_140222fa0()`). Two distinct streams: drawing an SFX probability never perturbs the sim RNG.
//   3. Its internal scheduling queue (2d72c0 over &DAT_140b27e60) sorts active events by WALL-CLOCK
//      timeGetTime() (the fire-time passed at 2d5290:21 is `timeGetTime() - _DAT_140a7bec0`,
//      comparator FUN_1402d3d90). A lockstep sim queue can never key on real time.
//
// ── DETERMINISM CONTRACT (the OPPOSITE of channel-1's) ──────────────────────────────────────────
// This channel mutates NO sim state and draws NO sim RNG, so its ordering is NOT lockstep-relevant.
// The drain may run on a presentation thread, in any order, or be dropped entirely (e.g. audio
// disabled, or a headless dedicated server), with zero effect on sim determinism — two MP clients
// schedule their own audio independently and still stay in lockstep. Contrast channel-1
// (signal_dispatch.h), where SignalListClass insertion order × GOM tick order IS lockstep-critical.
//
// In the parallel rewrite a per-entity update (Phase A) cannot call the shared SFXEventManager from
// N worker threads, so 387400's inline Start_SFX_Event becomes a recorded SfxCommand on the
// per-thread CommandSink (CommandSink::emit_sfx_event). After Phase A, drain_sfx replays the buffer
// serially to an SfxSink (the presentation boundary; the real impl calls Start_SFX_Event). Buffer
// order is preserved for reproducible playback only — it carries no lockstep weight.
//
// NOTE: the genuine OutgoingEventQueueClass (DAT_140b2ed18, "OutgoingEventQueueClass::Add") IS a
// real timed/scheduled queue ordered by (fire_time, insertion) — but it lives at the
// galactic/strategic layer (28d400: PENDING_TACTICAL_BATTLE_VOTE), outside this tactical sim slice.
// 387400 does not touch it. See CommandSink::schedule_event.
#pragma once
#include "command_sink.h"
#include <vector>

namespace sim {

// The presentation boundary. The real implementation forwards to SFXEventManagerClass::Start_SFX_Event
// (FUN_1402d5290). Host/test implementations just record. An impl MUST NOT read or write any sim
// state and MUST NOT draw the sim RNG — this is the whole point of the channel-2 split.
struct SfxSink {
    virtual ~SfxSink() = default;
    virtual void play(void* emitter, uint32_t sfx_id) = 0;
};

// Serial presentation drain: route each recorded cue to the audio boundary in buffer order. No
// sim-state read/write, no sim-RNG draw. The order is preserved only for reproducible playback; it
// carries no lockstep weight (a conforming engine may reorder or drop these).
void drain_sfx(const std::vector<SfxCommand>& sfx, SfxSink& out);

} // namespace sim
