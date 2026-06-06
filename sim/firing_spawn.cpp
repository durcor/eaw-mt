// sim/firing_spawn.cpp — b3 projectile create+init restructure. See firing_spawn.h for the model.
#include "firing_spawn.h"
#include <cmath>

namespace sim {

ProjectileInit firing_build_projectile_init(const ProjectileFiringInputs& in) {
    ProjectileInit p;
    p.present       = true;
    p.firer_id      = in.firer_id;
    p.target        = in.target;
    p.target_sub_id = in.target_sub_id;

    // damage: owner override else template, then optional charge-scale (3825b0:292-303).
    // operand order is mod*dmg (the binary's *(charge+0x17c) * *(rec+0x64)).
    float dmg = (in.owner_damage <= 0.0f) ? in.template_damage : in.owner_damage;
    if (in.apply_charge) dmg = in.charge_mod * dmg;
    p.damage = dmg;

    // lifetime/range: owner if nonzero else template (3825b0:304-311).
    p.lifetime = (in.owner_lifetime == 0) ? in.template_lifetime : in.owner_lifetime;

    // muzzle speed: base override, then accumulate extents + |Δz| with the addend on the LEFT
    // (3825b0:318/324/329 are all `newterm + acc`).
    float ms = (in.base_speed <= 0.0f) ? in.template_speed : in.base_speed;
    ms = in.target_extent + ms;
    if (in.add_subextent) ms = in.target_subextent + ms;
    ms = std::fabs(in.aim_z - in.aimpoint_z) + ms;
    p.muzzle_speed = ms;

    // visibility frame (3825b0:330-340): only when owner_type+0x4a4 > 0.
    p.has_vis_frame = in.has_vis_frame;
    if (in.has_vis_frame) p.vis_frame = in.vis_base + in.vis_offset;
    p.shroud = in.shroud;

    // launch geometry: guided computes a lead offset = target_rot · delta (3825b0:374-382); ballistic
    // copies the post-spread aim direction (3825b0:398-400).
    p.guided = in.guided;
    if (in.guided) {
        const float* m = in.target_rot;       // row-major rows post-22d410
        const eaw::vec3& d = in.guided_delta;
        p.guided_lead[0] = m[0]*d.x + m[1]*d.y + m[2]*d.z;
        p.guided_lead[1] = m[3]*d.x + m[4]*d.y + m[5]*d.z;
        p.guided_lead[2] = m[6]*d.x + m[7]*d.y + m[8]*d.z;
    } else {
        p.launch_dir[0] = in.launch_dir.x;
        p.launch_dir[1] = in.launch_dir.y;
        p.launch_dir[2] = in.launch_dir.z;
    }
    return p;
}

SpawnCommand firing_make_spawn(const ProjectileFiringInputs& in) {
    SpawnCommand c;
    c.requester   = in.requester;
    c.template_id = in.template_id;
    c.team        = in.team;
    c.pos[0]      = in.spawn_pos.x;
    c.pos[1]      = in.spawn_pos.y;
    c.pos[2]      = in.spawn_pos.z;
    c.flags       = in.flags;
    c.projectile  = firing_build_projectile_init(in);
    return c;
}

void firing_apply_projectile_record(ProjectileRecord& rec, const ProjectileInit& p) {
    rec.firer_id      = p.firer_id;        // +0x58
    rec.target        = p.target;          // +0x08
    rec.target_sub_id = p.target_sub_id;   // +0x10
    rec.damage        = p.damage;          // +0x64
    rec.lifetime      = p.lifetime;        // +0x68
    rec.muzzle_speed  = p.muzzle_speed;    // +0x6c
    if (p.has_vis_frame) rec.vis_frame = p.vis_frame;  // +0x60 (else left as initialized)
    rec.shroud        = p.shroud;          // +0xb0
    rec.guided        = p.guided;
    if (p.guided) {
        rec.guided_lead[0] = p.guided_lead[0];
        rec.guided_lead[1] = p.guided_lead[1];
        rec.guided_lead[2] = p.guided_lead[2];
    } else {
        rec.launch_dir[0] = p.launch_dir[0];
        rec.launch_dir[1] = p.launch_dir[1];
        rec.launch_dir[2] = p.launch_dir[2];
    }
}

bool ProjectileRecord::operator==(const ProjectileRecord& o) const {
    return firer_id == o.firer_id && target == o.target && target_sub_id == o.target_sub_id &&
           damage == o.damage && lifetime == o.lifetime && muzzle_speed == o.muzzle_speed &&
           vis_frame == o.vis_frame && shroud == o.shroud && guided == o.guided &&
           launch_dir[0] == o.launch_dir[0] && launch_dir[1] == o.launch_dir[1] &&
           launch_dir[2] == o.launch_dir[2] && guided_lead[0] == o.guided_lead[0] &&
           guided_lead[1] == o.guided_lead[1] && guided_lead[2] == o.guided_lead[2];
}

} // namespace sim
