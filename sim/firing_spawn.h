// sim/firing_spawn.h — (b) firing-body lift, b3: the projectile create+init restructure.
//
// This is the KEYSTONE of b3 (firing_body_lift_scope.md §3/§5). The fire-control body FUN_1403825b0
// creates a projectile (29f810, 3825b0:266) and writes its motion/combat record at proj+0xe8
// (=plVar12[0x1d]) INLINE (3825b0:289-401). That inline-consume is the §0 deferral blocker.
//
// b3 splits the body along the write-class line:
//   PHASE A (per-firer, parallel — reads only firer own-state + the target SNAPSHOT, writes nothing
//            cross-entity): compute the projectile-init PAYLOAD (pure data) and emit ONE SpawnCommand.
//   PHASE B (serial drain, canonical (gom,rank,seq) order): the SpawnCommand applier runs the entire
//            inline-consumed block — 29f810 create → write proj+0xe8 from the payload → the post-create
//            transform copy (ballistic branch, reads the NEW proj's own transform) → the two Class-2b
//            edits (220e90 listener on target+0x38, 3a06a0 shot-register). Both Class-2b edits DEPEND
//            on the created projectile, so they belong in the applier (canonical order), not as
//            separately-buffered Phase-A signals — hence b3 needs NO new command op kind, just the
//            enriched SpawnCommand.projectile payload + a create_object that performs the post-create init.
//
// SNAPSHOT-READY: every cross-object read is hoisted to an explicit input field (the ENV stat getters
// 3857d0/397780/5400f0/3952a0/398010… are resolved by the caller — the same pattern as
// firing_intercept's magnitude inputs). The faithful FP grouping (per the #8-part-2/DTARG2 non-
// associativity lessons) is preserved: accumulations put the addend on the LEFT (`addend + acc`), and
// the charge scale is `mod * dmg` (3825b0 operand order). The b3 host gate validates DRAIN DETERMINISM
// (N-shard ≡ serial, schema §9); per-field bit-exactness vs the binary is the later in-game DTWA-
// structural oracle (§5).
#pragma once
#include "eaw_types.h"
#include "command_sink.h"

namespace sim {

// Resolved Phase-A inputs for one projectile shot (one 3825b0 success path). All cross-object reads
// are explicit; offsets in comments are into the firing body's locals / the source objects.
struct ProjectileFiringInputs {
    // ── create args (29f810, 3825b0:261-268) ─────────────────────────────────────────────────────
    void*     requester       = nullptr;  // firer owner GameObject* (param_1+0x10); mgr=*(requester+0x2b8)
    uint32_t  template_id     = 0;        // projectile template (lVar7)
    int32_t   team            = -1;       // *(team_entry+0x4c)
    eaw::vec3 spawn_pos       = {};       // &local_2b8 launch orientation/position (param_4)
    uint32_t  flags           = 0;        // spawn-kind packed

    // ── identity / linkage ───────────────────────────────────────────────────────────────────────
    uint32_t  firer_id        = 0;        // owner+0x50           → rec+0x58 (3825b0:290)
    void*     target          = nullptr;  // param_2              → rec+0x08 (3825b0:326)
    int32_t   target_sub_id   = -1;       // resolved-param_3+0x18 or -1 → rec+0x10 (3825b0:352-358).
                                          // NB: param_3 is the RESOLVED firing context — when the caller
                                          // passes NULL, 3825b0:107-110 reassigns it via 398440/394a80
                                          // (DTWA-B3 §8.9). The caller must pass the resolved sub-id here.

    // ── damage (3825b0:292-303) ──────────────────────────────────────────────────────────────────
    eaw::f32  owner_damage    = 0.0f;     // owner_type+0x478 (<=0 ⇒ use template)
    eaw::f32  template_damage = 0.0f;     // tmpl+0x474
    bool      apply_charge    = false;    // has_order && order_active<0 && charge!=0
    eaw::f32  charge_mod      = 1.0f;     // charge+0x17c (3952a0(owner,0x46))

    // ── lifetime/range (3825b0:304-311) ──────────────────────────────────────────────────────────
    uint32_t  owner_lifetime  = 0;        // 5400f0(owner_type) (0 ⇒ use template)
    uint32_t  template_lifetime = 0;      // tmpl+0x440

    // ── muzzle speed (3825b0:312-329) ────────────────────────────────────────────────────────────
    eaw::f32  base_speed      = 0.0f;     // 3857d0(param_1) (<=0 ⇒ use template)
    eaw::f32  template_speed  = 0.0f;     // tmpl+0x4bc
    eaw::f32  target_extent   = 0.0f;     // 397780(target)              (added on the LEFT)
    bool      add_subextent   = false;    // 39b1a0(target) && !372210(owner+0x298)
    eaw::f32  target_subextent= 0.0f;     // 397780(target[0x56])        (added on the LEFT)
    eaw::f32  aim_z           = 0.0f;     // local_2d0 (launch dir z)
    eaw::f32  aimpoint_z      = 0.0f;     // local_2b0 (|Δz| = ABS(aim_z - aimpoint_z), added on LEFT)

    // ── visibility frame (3825b0:330-340) ────────────────────────────────────────────────────────
    bool      has_vis_frame   = false;    // owner_type+0x4a4 > 0
    int32_t   vis_base        = 0;        // b15418+0x10           → rec+0x60 = vis_base + vis_offset
    int32_t   vis_offset      = 0;        // owner_type+0x4a4
    void*     shroud          = nullptr;  // local_res20           → rec+0xb0

    // ── launch geometry ──────────────────────────────────────────────────────────────────────────
    bool      guided          = false;    // tmpl+0x1fe8 == 1 (guided/missile vs ballistic)
    eaw::vec3 launch_dir      = {};       // local_2d8/2d4/2d0 post-spread aim dir → rec+0x3c/0x40/0x44
    eaw::f32  target_rot[9]   = {1,0,0, 0,1,0, 0,0,1}; // target rotation rows post-22d410 (row-major)
    eaw::vec3 guided_delta    = {};       // (dir - leadvec) per 3825b0:363-365 → rec+0x24/0x28/0x2c via rot
};

// Phase A: compute the projectile-init payload (pure data) from the resolved inputs. Faithful to the
// 3825b0 branch logic + FP grouping. No cross-entity write.
ProjectileInit firing_build_projectile_init(const ProjectileFiringInputs& in);

// Phase A: assemble the SpawnCommand the firer emits (create args + the init payload). The drain
// resolves the manager as *(requester+0x2b8) and runs the applier below in canonical order.
SpawnCommand firing_make_spawn(const ProjectileFiringInputs& in);

// The proj+0xe8 record the Phase-B applier writes — host stand-in for the live projectile motion record
// (offsets mirror ProjectileInit's field comments).
struct ProjectileRecord {
    uint32_t firer_id      = 0;        // +0x58
    void*    target        = nullptr;  // +0x08
    int32_t  target_sub_id = -1;       // +0x10
    float    damage        = 0.0f;     // +0x64
    uint32_t lifetime      = 0;        // +0x68
    float    muzzle_speed  = 0.0f;     // +0x6c
    int32_t  vis_frame     = 0;        // +0x60
    void*    shroud        = nullptr;  // +0xb0
    bool     guided        = false;
    float    launch_dir[3] = {0,0,0};  // +0x3c/0x40/0x44
    float    guided_lead[3]= {0,0,0};  // +0x24/0x28/0x2c

    bool operator==(const ProjectileRecord& o) const;
};

// Phase B (applier): write the init payload into the freshly-created projectile record. The pure-data
// half of 3825b0:289-401; the engine WorldApply runs CreateObject + this + the 2 Class-2b edits in
// canonical order.
void firing_apply_projectile_record(ProjectileRecord& rec, const ProjectileInit& p);

} // namespace sim
