// sim/unit_ai.cpp — UnitAIBehaviorClass::vfunc_6 (RVA 0x4f6070).
// See unit_ai.h for the full per-tick algorithm and the (cross-entity) determinism notes.
#include "unit_ai.h"

namespace sim {

void unit_ai_tick(UnitAiState& st, void* owner, int now,
                  const eaw::f32 transform[UnitAiState::kCacheFloats], UnitAiEnv& env) {
    // 4f6070: game->vfunc_0xf0() — if the sim is not simulating this frame, the whole tick no-ops.
    if (!env.sim_active()) return;

    // ── BLOCK A: throttled self-refresh (every 300 ticks), gated by the shared sensor mgr existing.
    if (st.next_due <= now && env.sensor_present()) {
        // First pass builds + zeroes the per-object bit array (the engine's init routine sets +0x18).
        if (!st.initialized) {
            env.ensure_initialized(st, env.object_count());
            st.initialized = true;
        }
        // 12-float exact compare of the observer's world transform against the cached copy.
        bool changed = false;
        for (int i = 0; i < UnitAiState::kCacheFloats; ++i) {
            if (st.cache[i] != transform[i]) { changed = true; break; }
        }
        if (!changed) {
            // Unchanged: just advance the fixed-phase schedule (next_due += 300, NOT now + 300).
            st.next_due += UnitAiState::kRefreshTicks;
        } else {
            // Moved: push the observer's footprint into the shared grid, advance, then recache.
            env.object_moved(owner);
            st.next_due += UnitAiState::kRefreshTicks;
            for (int i = 0; i < UnitAiState::kCacheFloats; ++i) st.cache[i] = transform[i];
        }
    }

    // ── BLOCK B: gated reveal hook (pure env side effect).
    if (env.reveal_due(owner)) env.reveal(owner);

    // ── BLOCK C: per-object visibility matrix.
    if (!env.fog_active()) {
        // Fog off / observer hidden: nothing is visible — clear the whole array.
        for (eaw::u8& b : st.vis) b = 0;
        return;
    }
    const int n = static_cast<int>(st.vis.size());   // ai+0x60 loop bound
    const int objn = env.object_count();
    const bool sensor = env.sensor_present();
    for (int idx = 0; idx < n; ++idx) {
        if (idx >= objn) return;                      // live global list shrank under us → stop
        void* obj = env.object_at(idx);
        if (!obj) continue;
        if (!env.visible(owner, obj)) {               // not visible
            if (st.vis[idx] != 0 && sensor) env.became_hidden(owner, obj);   // edge 1->0
            st.vis[idx] = 0;
        } else {                                      // visible
            if (st.vis[idx] == 0 && sensor) env.became_visible(owner, obj);  // edge 0->1
            st.vis[idx] = 1;
        }
    }
}

} // namespace sim
