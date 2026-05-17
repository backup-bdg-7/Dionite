// ============================================================================
// Dionite — Player: top-level controller (input -> intent)
// ============================================================================
#pragma once
#include "Core/Math/Vector.h"
#include <functional>

namespace dionite::player {

struct InputState {
    math::Vec2 move;   // -1..1 (left stick / WASD / touch)
    math::Vec2 aim;    // -1..1 (right stick / mouse / touch)
    bool fire = false;
    bool firePressed = false;
    bool ability1 = false;
    bool ability2 = false;
    bool dashPressed = false;
    bool interactPressed = false;
    bool jumpPressed = false;
};

struct PlayerStats {
    float maxHealth = 100.f;
    float health = 100.f;
    float maxMana = 50.f;
    float mana = 50.f;
    float moveSpeed = 360.f;
    float dashCooldown = 1.6f;
    float dashTimer = 0.f;
    float damageMult = 1.f;
    float fireRateMult = 1.f;
    float critChance = 0.05f;
    float critMult = 1.5f;
    float lifesteal = 0.f;
    float dodge = 0.f;
};

class PlayerController {
public:
    void update(float dt, const InputState& in) {
        // Tick cooldowns
        if (stats_.dashTimer > 0) stats_.dashTimer -= dt;
        // Movement
        position_ += math::Vec3(in.move.x, 0, in.move.y) * stats_.moveSpeed * dt;
        // Aim direction
        if (in.aim.length() > 0.18f) {
            aimDir_ = math::Vec3(in.aim.x, 0, in.aim.y).normalized();
        }
        // Dash
        if (in.dashPressed && stats_.dashTimer <= 0.f) {
            position_ += aimDir_ * 280.f;
            stats_.dashTimer = stats_.dashCooldown;
        }
        // Mana regen
        if (stats_.mana < stats_.maxMana) stats_.mana = std::min(stats_.maxMana, stats_.mana + 6.f * dt);
    }

    math::Vec3 position() const { return position_; }
    math::Vec3 aimDir()  const { return aimDir_; }
    PlayerStats& stats()       { return stats_; }
    const PlayerStats& stats() const { return stats_; }

    void takeDamage(float dmg) { stats_.health = std::max(0.f, stats_.health - dmg); }
    void heal(float amt)       { stats_.health = std::min(stats_.maxHealth, stats_.health + amt); }
    bool dead() const          { return stats_.health <= 0.f; }

private:
    math::Vec3 position_{0, 0, 0};
    math::Vec3 aimDir_{1, 0, 0};
    PlayerStats stats_;
};

} // namespace dionite::player
