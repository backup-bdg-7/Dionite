// ============================================================================
// Dionite — Skill catalogue (Necromancer). ~25 skills.
// ============================================================================
#include "SkillLibrary.h"
using C = dionite::progression::SkillCategory;
using D = dionite::progression::DamageType;
using CL = dionite::progression::ClassId;

namespace dionite::progression {

void SkillLibrary::loadNecromancer() {
    // BASIC
    add({ "nec_bone_splinters", "Bone Splinters", CL::Necromancer, C::Basic, D::Physical, "Hurl 3 bone splinters. Generates 8 Essence each hit.",  10.f, 0.f, 8.f, 0.f, 0.3f, 0.f, 12.f, 0.f });
    add({ "nec_reap",           "Reap",            CL::Necromancer, C::Basic, D::Shadow,    "Sweep a scythe; generates 12 Essence.",                  18.f, 0.f, 12.f, 0.f, 0.45f, 2.4f, 1.5f, 0.f });
    add({ "nec_decompose",      "Decompose",       CL::Necromancer, C::Basic, D::Poison,    "Channeled decay; deals 10 dps, generates 4 Essence/s.",  10.f, 0.f, 4.f, 0.f, 0.f, 0.f, 8.f, 0.f });

    // CORE
    add({ "nec_corpse_explosion","Corpse Explosion",CL::Necromancer, C::Core, D::Shadow,    "Detonate a corpse for huge AoE damage.",                 110.f, 35.f, 0.f, 0.f, 0.2f, 4.0f, 12.f, 0.f });
    add({ "nec_blood_lance",    "Blood Lance",     CL::Necromancer, C::Core, D::Shadow,    "Pierce target dealing increasing dmg per pierce.",       85.f, 30.f, 0.f, 0.f, 0.3f, 0.f, 18.f, 0.f });
    add({ "nec_bone_spear",     "Bone Spear",      CL::Necromancer, C::Core, D::Physical,  "Hurl a spear that pierces all enemies.",                 70.f, 25.f, 0.f, 0.f, 0.3f, 0.f, 20.f, 0.f });

    // DEFENSIVE
    add({ "nec_bone_armor",     "Bone Armor",      CL::Necromancer, C::Defensive, D::None, "Spinning bones grant +35% dmg reduction (6s).",          0.f, 0.f, 0.f, 25.f, 0.f, 0.f, 0.f, 6.f });
    add({ "nec_blood_mist",     "Blood Mist",      CL::Necromancer, C::Defensive, D::Shadow,"Dissolve into mist; invuln 3s, draining nearby foes.",  15.f, 0.f, 0.f, 30.f, 0.f, 4.5f, 0.f, 3.f });
    add({ "nec_iron_maiden",    "Iron Maiden",     CL::Necromancer, C::Defensive, D::None, "Cursed foes reflect 40% of damage taken (8s).",          0.f, 15.f, 0.f, 20.f, 0.f, 6.0f, 0.f, 8.f });

    // MOBILITY
    add({ "nec_grim_step",      "Grim Step",       CL::Necromancer, C::Mobility, D::Shadow,"Teleport 10m, leaving a corpse.",                        0.f, 10.f, 0.f, 12.f, 0.f, 0.f, 10.f, 0.f });
    add({ "nec_wraith_form",    "Wraith Form",     CL::Necromancer, C::Mobility, D::None,  "Become incorporeal for 2.5s; +60% speed.",               0.f, 0.f, 0.f, 28.f, 0.f, 0.f, 0.f, 2.5f });

    // UTILITY
    add({ "nec_raise_skeleton", "Raise Skeleton",  CL::Necromancer, C::Utility, D::None,   "Summon a skeleton warrior. Stacks to 5.",                0.f, 20.f, 0.f, 4.f, 0.f, 0.f, 0.f, 0.f });
    add({ "nec_summon_golem",   "Summon Golem",    CL::Necromancer, C::Utility, D::None,   "Summon a flesh golem (tank). 1 active.",                 0.f, 40.f, 0.f, 30.f, 0.f, 0.f, 0.f, 0.f });
    add({ "nec_curse_decrepify","Decrepify",       CL::Necromancer, C::Utility, D::None,   "Curse foes: -25% speed and dmg dealt (10s).",            0.f, 10.f, 0.f, 18.f, 0.f, 5.5f, 0.f, 10.f });
    add({ "nec_bone_prison",    "Bone Prison",     CL::Necromancer, C::Utility, D::Physical,"Trap target inside a bone cage for 4s.",                0.f, 15.f, 0.f, 14.f, 0.f, 0.f, 12.f, 4.f });

    // ULTIMATE
    add({ "nec_army_of_dead",   "Army of the Dead",CL::Necromancer, C::Ultimate, D::Shadow, "Skeletons rain down on target area dealing massive dmg.",450.f, 0.f, 0.f, 90.f, 0.6f, 6.0f, 20.f, 0.f });
    add({ "nec_bone_storm",     "Bone Storm",      CL::Necromancer, C::Ultimate, D::Physical,"Surround yourself in a spinning bone tempest (12s).",  35.f, 0.f, 0.f, 100.f, 0.f, 5.0f, 0.f, 12.f });

    // PASSIVES
    add({ "nec_pas_decrepify",  "Death's Embrace",CL::Necromancer, C::Passive, D::None,   "Cursed foes take +20% damage.",                            0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "nec_pas_undeath",    "Undeath",        CL::Necromancer, C::Passive, D::None,   "Skeletons heal you for 15% of dmg they deal.",             0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "nec_pas_corpse_lord","Lord of the Dead",CL::Necromancer, C::Passive, D::None,  "+1 max skeleton; -10% summon cost.",                       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "nec_pas_shadowblight","Shadowblight",  CL::Necromancer, C::Passive, D::None,   "+25% Shadow damage.",                                      0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "nec_pas_serration",  "Serration",      CL::Necromancer, C::Passive, D::None,   "Bone skills crit chance +10%.",                            0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "nec_pas_grim_harvest","Grim Harvest",  CL::Necromancer, C::Passive, D::None,   "Killing a cursed foe drops a corpse.",                     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "nec_pas_kalans_edict","Kalan's Edict", CL::Necromancer, C::Passive, D::None,   "Curses last 50% longer.",                                  0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "nec_pas_memento_mori","Memento Mori",  CL::Necromancer, C::Passive, D::None,   "When a minion dies, recover 5% max HP.",                   0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
}

} // namespace dionite::progression
