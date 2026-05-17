// ============================================================================
// Dionite — Skill catalogue (Monk). ~25 skills.
// ============================================================================
#include "SkillLibrary.h"
using C = dionite::progression::SkillCategory;
using D = dionite::progression::DamageType;
using CL = dionite::progression::ClassId;

namespace dionite::progression {

void SkillLibrary::loadMonk() {
    // BASIC
    add({ "mon_crippling_wave","Crippling Wave",CL::Monk, C::Basic, D::Physical, "3-hit combo; generates 8 Spirit each.",                12.f, 0.f, 8.f, 0.f, 0.3f, 2.0f, 1.5f, 0.f });
    add({ "mon_fists_of_thunder","Fists of Thunder",CL::Monk,C::Basic,D::Shock,    "Teleporting punches; +10 Spirit.",                     16.f, 0.f, 10.f, 0.f, 0.25f, 0.f, 6.f,  0.f });
    add({ "mon_deadly_reach", "Deadly Reach",  CL::Monk, C::Basic, D::Physical, "Long-reach kick; +9 Spirit.",                          14.f, 0.f, 9.f, 0.f, 0.35f, 0.f, 4.f,  0.f });

    // CORE
    add({ "mon_seven_sided_strike","Seven-Sided Strike",CL::Monk,C::Core,D::Holy, "Strike 7 times across nearby foes.",                  280.f, 50.f, 0.f, 0.f, 0.4f, 8.0f, 0.f, 0.f });
    add({ "mon_dashing_strike","Dashing Strike",CL::Monk, C::Core, D::Physical, "Dash through target dealing damage.",                  55.f, 25.f, 0.f, 0.f, 0.2f, 0.f, 14.f, 0.f });
    add({ "mon_wave_of_light","Wave of Light", CL::Monk, C::Core, D::Holy,      "Crashing bell of holy damage in front.",                90.f, 35.f, 0.f, 0.f, 0.4f, 4.5f, 0.f, 0.f });
    add({ "mon_exploding_palm","Exploding Palm",CL::Monk, C::Core, D::Physical, "Bleed; on death, target explodes.",                    75.f, 30.f, 0.f, 0.f, 0.3f, 0.f, 2.0f, 0.f });

    // DEFENSIVE
    add({ "mon_serenity",     "Serenity",      CL::Monk, C::Defensive, D::None,  "Immune to damage and CC for 3s.",                      0.f, 0.f, 0.f, 30.f, 0.f, 0.f, 0.f, 3.f });
    add({ "mon_inner_sanctuary","Inner Sanctuary",CL::Monk,C::Defensive,D::Holy, "Ground heals + reduces dmg taken 50% (5s).",          0.f, 25.f, 0.f, 20.f, 0.f, 4.0f, 0.f, 5.f });
    add({ "mon_breath_of_heaven","Breath of Heaven",CL::Monk,C::Defensive,D::Holy,"Heal yourself and allies in 8m.",                    0.f, 20.f, 0.f, 15.f, 0.f, 8.0f, 0.f, 0.f });

    // MOBILITY
    add({ "mon_tempest_rush", "Tempest Rush",  CL::Monk, C::Mobility, D::Physical,"Channel: charge forward dealing damage.",            20.f, 10.f, 0.f, 0.f, 0.f, 2.0f, 0.f, 0.f });
    add({ "mon_epiphany",     "Epiphany",      CL::Monk, C::Mobility, D::None,    "Free dashes for 8s; gain spirit on hit.",            0.f, 0.f, 0.f, 30.f, 0.f, 0.f, 0.f, 8.f });

    // UTILITY
    add({ "mon_mantra_salvation","Mantra of Salvation",CL::Monk,C::Utility,D::None,"+20% dmg reduction for party (15s).",                0.f, 50.f, 0.f, 30.f, 0.f, 10.0f, 0.f, 15.f });
    add({ "mon_mantra_conviction","Mantra of Conviction",CL::Monk,C::Utility,D::None,"+20% damage for party (15s).",                    0.f, 50.f, 0.f, 30.f, 0.f, 10.0f, 0.f, 15.f });
    add({ "mon_cyclone_strike","Cyclone Strike",CL::Monk, C::Utility, D::Shock,   "Pull all foes within 15m to you; AoE shock.",        60.f, 30.f, 0.f, 12.f, 0.f, 6.0f, 0.f, 0.f });
    add({ "mon_blinding_flash","Blinding Flash",CL::Monk, C::Utility, D::Holy,    "Blind foes in 8m for 3s.",                            0.f, 15.f, 0.f, 10.f, 0.f, 6.0f, 0.f, 3.f });

    // ULTIMATE
    add({ "mon_sunwuko",      "Way of the Sunwuko",CL::Monk,C::Ultimate,D::Holy,  "Summon 3 mirror clones for 15s (each casts ults).",  90.f, 0.f, 0.f, 100.f, 0.f, 0.f, 0.f, 15.f });
    add({ "mon_heavens_wrath","Heaven's Wrath",CL::Monk, C::Ultimate, D::Holy,   "Pillars of light strike around you (8s).",            300.f, 0.f, 0.f, 90.f, 0.f, 9.0f, 0.f, 8.f });

    // PASSIVES
    add({ "mon_pas_chant_of_resonance","Chant of Resonance",CL::Monk,C::Passive,D::None,"Mantras last 50% longer.",                       0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f });
    add({ "mon_pas_seize_the_initiative","Seize the Initiative",CL::Monk,C::Passive,D::None,"+30% armor for 4s on hit.",                 0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f });
    add({ "mon_pas_one_with_everything","One With Everything",CL::Monk,C::Passive,D::None,"All resistances equal to highest.",          0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f });
    add({ "mon_pas_combination_strike","Combination Strike",CL::Monk,C::Passive,D::None,"+10% dmg per different basic used (5s).",      0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f });
    add({ "mon_pas_resolve",  "Resolve",       CL::Monk, C::Passive, D::None,    "Enemies you damage deal -20% dmg for 4s.",            0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f });
    add({ "mon_pas_beacon_of_ytar","Beacon of Ytar",CL::Monk,C::Passive,D::None, "Cooldowns -15%.",                                     0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f });
    add({ "mon_pas_unity",    "Unity",         CL::Monk, C::Passive, D::None,    "Allies in 12m share 25% of damage taken.",            0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f });
}

} // namespace dionite::progression
