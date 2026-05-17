// ============================================================================
// Dionite — Skill catalogue (Crusader). ~25 skills.
// ============================================================================
#include "SkillLibrary.h"
using C = dionite::progression::SkillCategory;
using D = dionite::progression::DamageType;
using CL = dionite::progression::ClassId;

namespace dionite::progression {

void SkillLibrary::loadCrusader() {
    // ---- BASIC (generators, no cost, no cd) ---------------------------------
    add({ "cru_smite",       "Smite",         CL::Crusader, C::Basic,     D::Holy,      "Strike a foe with holy might. Generates 12 Wrath.",     18.f, 0.f, 12.f, 0.f, 0.4f, 0.f, 1.6f, 0.f });
    add({ "cru_punish",      "Punish",        CL::Crusader, C::Basic,     D::Physical,  "Crushing hammer blow. Generates 15 Wrath, brief stun.", 22.f, 0.f, 15.f, 0.f, 0.45f, 0.f, 1.5f, 0.6f });
    add({ "cru_slash",       "Crusader's Cut", CL::Crusader, C::Basic,    D::Physical,  "Wide horizontal slash. Generates 10 Wrath, hits up to 3.", 14.f, 0.f, 10.f, 0.f, 0.35f, 2.2f, 1.5f, 0.f });

    // ---- CORE (spenders) ----------------------------------------------------
    add({ "cru_shield_bash", "Shield Bash",    CL::Crusader, C::Core,      D::Physical,  "Shield-charge forward; knockback + stun on contact.",   55.f, 25.f, 0.f, 0.f, 0.3f, 0.f, 4.0f, 1.0f });
    add({ "cru_holy_blast",  "Holy Blast",     CL::Crusader, C::Core,      D::Holy,      "AoE radiant detonation around you.",                    70.f, 30.f, 0.f, 0.f, 0.3f, 3.5f, 0.f, 0.f });
    add({ "cru_judgement",   "Judgement",      CL::Crusader, C::Core,      D::Holy,      "Beam of light strikes target and 2 nearest enemies.",   85.f, 40.f, 0.f, 0.f, 0.5f, 4.0f, 8.0f, 0.f });

    // ---- DEFENSIVE ----------------------------------------------------------
    add({ "cru_iron_skin",   "Iron Skin",     CL::Crusader, C::Defensive, D::None,      "+50% damage reduction for 4s.",                          0.f, 10.f, 0.f, 30.f, 0.f, 0.f, 0.f, 4.f });
    add({ "cru_consecration","Consecration",  CL::Crusader, C::Defensive, D::Holy,      "Ground beneath you heals allies, burns enemies (8s).",   8.f, 20.f, 0.f, 24.f, 0.f, 4.0f, 0.f, 8.f });
    add({ "cru_aegis",       "Aegis of Faith",CL::Crusader, C::Defensive, D::None,      "Absorb shield equal to 25% max HP for 6s.",              0.f, 15.f, 0.f, 35.f, 0.f, 0.f, 0.f, 6.f });

    // ---- MOBILITY -----------------------------------------------------------
    add({ "cru_charge",      "Charge",        CL::Crusader, C::Mobility,  D::Physical,  "Charge forward 12 m, damaging and knocking back foes.", 40.f, 0.f, 10.f, 10.f, 0.f, 1.5f, 12.f, 0.f });
    add({ "cru_steed_charge","Steed Charge",  CL::Crusader, C::Mobility,  D::Physical,  "Summon a celestial steed for 3s, doubling speed.",      0.f, 0.f, 0.f, 30.f, 0.f, 0.f, 0.f, 3.f });

    // ---- UTILITY ------------------------------------------------------------
    add({ "cru_provoke",     "Provoke",       CL::Crusader, C::Utility,   D::None,      "Taunt nearby enemies for 5s; +30% threat.",              0.f, 5.f, 0.f, 16.f, 0.f, 6.0f, 0.f, 5.f });
    add({ "cru_laws_of_valor","Laws of Valor",CL::Crusader, C::Utility,   D::None,      "Aura: +20% attack speed, +15% movement for 10s.",        0.f, 30.f, 0.f, 45.f, 0.f, 8.0f, 0.f, 10.f });
    add({ "cru_blessed_hammer","Blessed Hammer",CL::Crusader, C::Utility, D::Holy,      "Spinning hammer orbits you, hitting foes for 6s.",       30.f, 25.f, 0.f, 20.f, 0.f, 4.0f, 0.f, 6.f });

    // ---- ULTIMATE -----------------------------------------------------------
    add({ "cru_akarats_champion","Akarat's Champion",CL::Crusader,C::Ultimate,D::Holy,  "Become a champion of light for 20s. +50% dmg/AS, regen.",0.f, 0.f, 0.f, 120.f, 0.f, 0.f, 0.f, 20.f });
    add({ "cru_falling_sword",  "Falling Sword",   CL::Crusader, C::Ultimate, D::Holy,  "Leap into the sky, then crash down for massive AoE.",     350.f, 0.f, 0.f, 90.f, 0.6f, 7.0f, 25.f, 0.f });

    // ---- PASSIVES -----------------------------------------------------------
    add({ "cru_pas_zeal",      "Zeal",       CL::Crusader, C::Passive,   D::None,      "+10% attack speed.",                                       0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "cru_pas_bulwark",   "Bulwark",    CL::Crusader, C::Passive,   D::None,      "+15% armor while at full Wrath.",                          0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "cru_pas_holy_cause","Holy Cause", CL::Crusader, C::Passive,   D::None,      "+15% Holy damage.",                                        0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "cru_pas_indestructible","Indestructible",CL::Crusader,C::Passive,D::None,   "When you would die, become invulnerable for 5s (60s cd).", 0.f, 0.f, 0.f, 60.f, 0.f, 0.f, 0.f, 5.f });
    add({ "cru_pas_righteousness","Righteousness",CL::Crusader,C::Passive,D::None,     "Generate 2 Wrath/sec while moving toward enemies.",         0.f, 0.f, 2.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "cru_pas_finery",    "Finery",     CL::Crusader, C::Passive,   D::None,      "Each socketed gem grants +1% all stats.",                   0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "cru_pas_long_arm",  "Long Arm of the Law",CL::Crusader,C::Passive,D::None,  "Laws (utility) last 50% longer.",                           0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
    add({ "cru_pas_wrathful",  "Wrathful",   CL::Crusader, C::Passive,   D::None,      "Spenders refund 10% Wrath on kill.",                        0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f });
}

} // namespace dionite::progression
