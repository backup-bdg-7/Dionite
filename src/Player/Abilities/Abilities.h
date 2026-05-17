// ============================================================================
// Dionite — Player: Soul Abilities (campaign-unlocked powers)
// ============================================================================
#pragma once
#include <string>
#include <vector>
#include <functional>

namespace dionite::player {

struct SoulAbility {
    std::string id;
    std::string name;
    std::string description;
    float manaCost;
    float cooldown;
    float timer = 0.f;
    bool unlocked = false;
    std::function<void()> cast; // bound by combat system
};

class AbilityRegistry {
public:
    AbilityRegistry() {
        abilities_ = {
            {"soul_dash", "Soul Dash", "Phase forward, becoming invulnerable.", 10, 0.6f, 0.f, true, {}},
            {"soul_grasp", "Soul Grasp", "Pull a distant enemy toward you.", 20, 4.f, 0.f, false, {}},
            {"soul_blast", "Soul Blast", "AoE explosion at cursor.", 35, 6.f, 0.f, false, {}},
            {"soul_veil", "Soul Veil", "Invisibility for 3s.", 30, 14.f, 0.f, false, {}},
            {"soul_avatar", "Soul Avatar", "Double damage & speed for 8s.", 60, 60.f, 0.f, false, {}},
        };
    }
    std::vector<SoulAbility>& list() { return abilities_; }
    SoulAbility* find(const std::string& id) {
        for (auto& a : abilities_) if (a.id == id) return &a;
        return nullptr;
    }
    void tick(float dt) {
        for (auto& a : abilities_) if (a.timer > 0) a.timer -= dt;
    }
private:
    std::vector<SoulAbility> abilities_;
};

} // namespace dionite::player
