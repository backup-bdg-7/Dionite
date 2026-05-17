// ============================================================================
// Dionite — Skill catalogue (Ranger). ~25 skills.
// ============================================================================
#include "SkillLibrary.h"
using C = dionite::progression::SkillCategory;
using D = dionite::progression::DamageType;
using CL = dionite::progression::ClassId;

namespace dionite::progression {

void SkillLibrary::loadRanger() {
    // BASIC
    add({ "ran_quick_shot",   "Quick Shot",    CL::Ranger, C::Basic, D::Physical, "Rapid arrow. Generates 6 Discipline.",                10.f, 0.f, 6.f, 0.f, 0.18f, 0.f, 24.f, 0.f });
    add({ "ran_puncturing",   "Puncturing Arrow",CL::Ranger,C::Basic, D::Physical, "Pierces all foes in line; +9 Discipline.",            14.f, 0.f, 9.f, 0.f, 0.28f, 0.f, 22.f, 0.f });
    add({ "ran_throw_dagger", "Throw Dagger",  CL::Ranger, C::Basic, D::Physical, "Thrown blade marks foe for 8s. +8 Discipline.",       16.f, 0.f, 8.f, 0.f, 0.25f, 0.f, 16.f, 8.f });

    // CORE
    add({ "ran_multishot",    "Multishot",     CL::Ranger, C::Core, D::Physical, "Cone of 7 arrows.",                                    35.f, 30.f, 0.f, 0.f, 0.4f, 5.0f, 22.f, 0.f });
    add({ "ran_penetrating",  "Penetrating Shot",CL::Ranger,C::Core, D::Physical, "High-velocity bolt pierces every foe in line.",       110.f, 35.f, 0.f, 0.f, 0.35f, 0.f, 30.f, 0.f });
    add({ "ran_elemental_arrow","Elemental Arrow",CL::Ranger,C::Core, D::Fire,    "Choose element; AoE explosion at target.",            85.f, 30.f, 0.f, 0.f, 0.3f, 3.5f, 20.f, 0.f });
    add({ "ran_rapid_fire",   "Rapid Fire",    CL::Ranger, C::Core, D::Physical, "Channel hail of arrows on target (3s).",              250.f, 40.f, 0.f, 2.f, 0.f, 0.f, 22.f, 3.f });

    // DEFENSIVE
    add({ "ran_shadow_power", "Shadow Power",  CL::Ranger, C::Defensive, D::Shadow,"Heal 30% of dmg dealt for 6s.",                      0.f, 14.f, 0.f, 18.f, 0.f, 0.f, 0.f, 6.f });
    add({ "ran_smoke_screen", "Smoke Screen",  CL::Ranger, C::Defensive, D::None,  "Vanish for 1.5s; +invulnerable.",                    0.f, 14.f, 0.f, 14.f, 0.f, 0.f, 0.f, 1.5f });
    add({ "ran_caltrops",     "Caltrops",      CL::Ranger, C::Defensive, D::Physical,"Slow + bleed in area (10s).",                      15.f, 6.f, 0.f, 16.f, 0.f, 4.0f, 0.f, 10.f });

    // MOBILITY
    add({ "ran_vault",        "Vault",         CL::Ranger, C::Mobility, D::None,  "Backflip 10m + free 6m dash; 2 charges.",            0.f, 8.f, 0.f, 6.f, 0.f, 0.f, 10.f, 0.f });
    add({ "ran_shadow_step",  "Shadow Step",   CL::Ranger, C::Mobility, D::Shadow, "Vanish + reappear behind target, +crit.",            45.f, 12.f, 0.f, 10.f, 0.f, 0.f, 18.f, 0.f });

    // UTILITY
    add({ "ran_sentry",       "Sentry",        CL::Ranger, C::Utility, D::Physical,"Place a turret that fires for 30s.",                30.f, 20.f, 0.f, 8.f, 0.f, 0.f, 0.f, 30.f });
    add({ "ran_marked_target","Marked for Death",CL::Ranger,C::Utility, D::None,    "Mark target; +30% dmg taken by it (15s).",          0.f, 10.f, 0.f, 12.f, 0.f, 0.f, 24.f, 15.f });
    add({ "ran_companion",    "Wolf Companion",CL::Ranger, C::Utility, D::Physical,"Summon a wolf to fight beside you.",                25.f, 20.f, 0.f, 30.f, 0.f, 0.f, 0.f, 0.f });
    add({ "ran_explosive_trap","Explosive Trap",CL::Ranger,C::Utility, D::Fire,    "Trip-trap explodes after 1.5s.",                    140.f, 20.f, 0.f, 10.f, 0.f, 4.0f, 0.f, 1.5f });

    // ULTIMATE
    add({ "ran_rain_of_vengeance","Rain of Vengeance",CL::Ranger,C::Ultimate,D::Physical,"Arrows rain from sky for 6s.",                   60.f, 0.f, 0.f, 70.f, 0.4f, 8.0f, 30.f, 6.f });
    add({ "ran_companion_pack",  "Companion Pack",  CL::Ranger, C::Ultimate, D::Physical,"Summon a pack of 4 wolves for 12s.",            40.f, 0.f, 0.f, 90.f, 0.f, 0.f, 0.f, 12.f });

    // PASSIVES
    add({ "ran_pas_keen_eye",  "Keen Eye",      CL::Ranger, C::Passive, D::None,    "+15% critical chance vs distant foes.",             0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "ran_pas_softfoot",  "Softfoot",      CL::Ranger, C::Passive, D::None,    "+8% movement speed.",                                0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "ran_pas_archery",   "Archery",       CL::Ranger, C::Passive, D::None,    "+15% bow damage.",                                   0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "ran_pas_steady_aim","Steady Aim",    CL::Ranger, C::Passive, D::None,    "+20% dmg if no enemies are within 8m.",              0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "ran_pas_sharpshooter","Sharpshooter",CL::Ranger, C::Passive, D::None,    "+1% crit per second up to +30%; resets on crit.",   0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "ran_pas_cull",      "Cull the Weak", CL::Ranger, C::Passive, D::None,    "+15% damage vs slowed/marked enemies.",              0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "ran_pas_thrill_of_hunt","Thrill of the Hunt",CL::Ranger,C::Passive,D::None,"Crits restore 4 Discipline.",                    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
}

} // namespace dionite::progression
