// ============================================================================
// Dionite — Combat: Ammo types (effects applied on hit)
// ============================================================================
#pragma once
#include <string>

namespace dionite::combat {

enum class AmmoType { Standard, Incendiary, Cryo, Shock, Soul, Void, Vampiric, Explosive };

struct AmmoEffect {
    AmmoType type;
    float intensity = 1.f;
    float duration = 2.f;
};

} // namespace dionite::combat
