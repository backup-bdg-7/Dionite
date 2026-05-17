// ============================================================================
// Dionite — Skill catalogue (Sorcerer). ~25 skills.
// ============================================================================
#include "SkillLibrary.h"
using C = dionite::progression::SkillCategory;
using D = dionite::progression::DamageType;
using CL = dionite::progression::ClassId;

namespace dionite::progression {

void SkillLibrary::loadSorcerer() {
    // BASIC
    add({ "sor_frost_bolt",     "Frost Bolt",     CL::Sorcerer, C::Basic, D::Cold,   "Lobs a frost bolt. Slow 25%. Generates 10 Mana.",  16.f, 0.f, 10.f, 0.f, 0.35f, 0.f, 18.f, 0.f });
    add({ "sor_arc_lash",       "Arc Lash",       CL::Sorcerer, C::Basic, D::Shock,  "Whip of lightning hits front cone. +12 Mana.",      14.f, 0.f, 12.f, 0.f, 0.32f, 3.0f, 5.f,  0.f });
    add({ "sor_fire_bolt",      "Fire Bolt",      CL::Sorcerer, C::Basic, D::Fire,   "Burn target for 6s. +9 Mana.",                       12.f, 0.f, 9.f,  0.f, 0.30f, 0.f, 20.f, 6.f });

    // CORE
    add({ "sor_fireball",       "Fireball",       CL::Sorcerer, C::Core, D::Fire,    "Explosive flame orb with AoE.",                      90.f, 30.f, 0.f, 0.f, 0.5f, 3.5f, 22.f, 0.f });
    add({ "sor_chain_lightning","Chain Lightning",CL::Sorcerer, C::Core, D::Shock,   "Bolt arcs to 5 nearest enemies.",                    70.f, 35.f, 0.f, 0.f, 0.4f, 0.f, 20.f, 0.f });
    add({ "sor_ice_shards",     "Ice Shards",     CL::Sorcerer, C::Core, D::Cold,    "Volley of 5 icicles spread forward.",                55.f, 25.f, 0.f, 0.f, 0.4f, 0.f, 18.f, 0.f });
    add({ "sor_meteor",         "Meteor",         CL::Sorcerer, C::Core, D::Fire,    "Call a meteor on target after 1s.",                  140.f, 50.f, 0.f, 0.f, 1.0f, 5.0f, 20.f, 0.f });

    // DEFENSIVE
    add({ "sor_ice_armor",      "Ice Armor",      CL::Sorcerer, C::Defensive, D::None,"Frozen barrier absorbs damage for 6s.",             0.f, 0.f, 0.f, 24.f, 0.f, 0.f, 0.f, 6.f });
    add({ "sor_diamond_skin",   "Diamond Skin",   CL::Sorcerer, C::Defensive, D::None,"Become immune to damage for 3s.",                   0.f, 0.f, 0.f, 35.f, 0.f, 0.f, 0.f, 3.f });
    add({ "sor_storm_armor",    "Storm Armor",    CL::Sorcerer, C::Defensive, D::Shock,"Lightning auto-strikes attackers (12s).",          18.f, 15.f, 0.f, 18.f, 0.f, 4.0f, 0.f, 12.f });

    // MOBILITY
    add({ "sor_teleport",       "Teleport",       CL::Sorcerer, C::Mobility, D::None, "Blink 14m; 3 charges, 8s recharge each.",           0.f, 15.f, 0.f, 8.f, 0.f, 0.f, 14.f, 0.f });
    add({ "sor_phase_walk",     "Phase Walk",     CL::Sorcerer, C::Mobility, D::None, "Become incorporeal for 2s, +60% speed.",            0.f, 0.f, 0.f, 18.f, 0.f, 0.f, 0.f, 2.f });

    // UTILITY
    add({ "sor_frost_nova",     "Frost Nova",     CL::Sorcerer, C::Utility, D::Cold,  "Freeze all enemies in radius for 2s.",              25.f, 20.f, 0.f, 12.f, 0.f, 5.0f, 0.f, 2.f });
    add({ "sor_hydra",          "Hydra",          CL::Sorcerer, C::Utility, D::Fire,  "Summon a 3-headed hydra (15s).",                    35.f, 35.f, 0.f, 18.f, 0.f, 0.f, 0.f, 15.f });
    add({ "sor_familiar",       "Familiar",       CL::Sorcerer, C::Utility, D::Fire,  "Permanent magical pet attacks foes.",               20.f, 50.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "sor_blizzard",       "Blizzard",       CL::Sorcerer, C::Utility, D::Cold,  "Icy storm slows + damages an area for 8s.",         55.f, 30.f, 0.f, 16.f, 0.f, 6.5f, 0.f, 8.f });

    // ULTIMATE
    add({ "sor_inferno",        "Inferno",        CL::Sorcerer, C::Ultimate, D::Fire, "Channel a city-melting beam of fire.",              420.f, 0.f, 0.f, 110.f, 0.f, 4.0f, 30.f, 8.f });
    add({ "sor_unstable_currents","Unstable Currents",CL::Sorcerer,C::Ultimate,D::Shock,"For 10s, all spells auto-trigger free arcs.",      0.f, 0.f, 0.f, 90.f, 0.f, 0.f, 0.f, 10.f });
    add({ "sor_deep_freeze",    "Deep Freeze",    CL::Sorcerer, C::Ultimate, D::Cold, "Channel; explodes for massive Cold damage.",        380.f, 0.f, 0.f, 90.f, 1.5f, 7.0f, 0.f, 1.5f });

    // PASSIVES
    add({ "sor_pas_elemental_attune","Elemental Attunement",CL::Sorcerer,C::Passive,D::None,"Hitting with an element resets cd of others.",0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f });
    add({ "sor_pas_glass_cannon","Glass Cannon",  CL::Sorcerer, C::Passive, D::None,  "+20% damage, -10% armor.",                          0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "sor_pas_arcane_dynamo","Arcane Dynamo",CL::Sorcerer, C::Passive, D::None,  "Every 5 basics empower next core +100%.",          0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "sor_pas_evocation",  "Evocation",      CL::Sorcerer, C::Passive, D::None,  "-15% cooldowns.",                                    0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "sor_pas_paralysis",  "Paralysis",      CL::Sorcerer, C::Passive, D::None,  "Shock spells have 10% chance to stun.",              0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "sor_pas_cold_blooded","Cold Blooded",  CL::Sorcerer, C::Passive, D::None,  "Frozen/chilled enemies take +20% damage.",           0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "sor_pas_aether_walker","Aether Walker",CL::Sorcerer, C::Passive, D::None,  "Teleport gains +1 charge.",                          0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
}

} // namespace dionite::progression
