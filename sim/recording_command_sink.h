// sim/recording_command_sink.h — a trivial CommandSink that records emissions in order.
//
// This is the faithful Phase-A stand-in described in command_sink.h: a unit's emission DECISIONS
// are deterministic given its inputs and independent of any connected slot's behaviour, so a sink
// that merely buffers (emitter, sig_id, payload) in emit order lets host tests assert exactly what
// a lifted unit would dispatch — without modelling the Phase-B SignalDispatcherClass fan-out.
//
// Reusable across unit tests; kept out of command_sink.h so that header stays a pure interface.
#pragma once
#include "command_sink.h"
#include <vector>

namespace sim {

// One recorded emission. For kSigOpportunityTargetAcquired the 3-ptr payload is copied into `opp`
// (the lifted unit owns the payload only until emit returns, so the sink must copy, not borrow).
struct RecordedCommand {
    void*                          emitter     = nullptr;
    uint32_t                       sig_id      = 0;
    bool                           has_payload = false;
    OpportunityTargetAcquiredData  opp{};        // valid iff sig_id == kSigOpportunityTargetAcquired
};

struct RecordingCommandSink : CommandSink {
    std::vector<RecordedCommand> commands;
    std::vector<std::pair<void*, uint64_t>> scheduled; // (event, fire_time) for channel 2

    void emit_signal(void* emitter, uint32_t sig_id,
                     const void* payload, size_t payload_size) override {
        RecordedCommand rc;
        rc.emitter     = emitter;
        rc.sig_id      = sig_id;
        rc.has_payload = (payload != nullptr);
        if (sig_id == kSigOpportunityTargetAcquired && payload &&
            payload_size >= sizeof(OpportunityTargetAcquiredData)) {
            rc.opp = *static_cast<const OpportunityTargetAcquiredData*>(payload);
        }
        commands.push_back(rc);
    }

    void schedule_event(void* event, uint64_t fire_time) override {
        scheduled.emplace_back(event, fire_time);
    }
};

} // namespace sim
