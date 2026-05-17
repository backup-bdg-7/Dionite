// ============================================================================
// Dionite — Combat: Spell base (mana-cost magic abilities)
// ============================================================================
#pragma once
#include "Core/Math/Vector.h"
#include <string>
#include <functional>

namespace dionite::combat {

enum class SpellType { Projectile, AoE, Channel, Buff, Teleport };

struct SpellBase {
    std::string id;
    std::string name;
    std::string description;
    SpellType   type;
    float       manaCost;
    float       cooldown;
    float       damage;
    float       radius;
    float       castTime;
    float       cdTimer = 0.f;
};

class SpellSystem {
public:
    bool cast(SpellBase& s, float& mana,
              std::function<void(const SpellBase&)> onCast) {
        if (s.cdTimer > 0 || mana < s.manaCost) return false;
        mana -= s.manaCost;
        s.cdTimer = s.cooldown;
        onCast(s);
        return true;
    }
    void tick(SpellBase& s, float dt) { if (s.cdTimer > 0) s.cdTimer -= dt; }
};

} // namespace dionite::combat
