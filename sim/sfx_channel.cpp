// sim/sfx_channel.cpp — channel-2 SFX drain (presentation side-effect replay).
// See sfx_channel.h for the full identification + determinism contract. The drain is deliberately
// trivial: a faithful reproduction of the live SFXEventManager's wall-clock-sorted audio scheduler
// would be pointless for a deterministic sim slice. What matters is the BOUNDARY — that this replay
// is presentation-only and touches neither sim state nor the sim RNG.
#include "sfx_channel.h"

namespace sim {

void drain_sfx(const std::vector<SfxCommand>& sfx, SfxSink& out) {
    for (const SfxCommand& c : sfx) {
        out.play(c.emitter, c.sfx_id); // <- FUN_1402d5290(&SFXEventManager, sfx, ...) at 387400:99
    }
}

} // namespace sim
