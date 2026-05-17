// ============================================================================
// Dionite — Player: Diablo III/IV-style controller with hybrid input modes.
//
// Two simultaneous movement modes (player can use either at any time):
//   1) Click-to-move  — sets a `navTarget`; controller follows the path.
//   2) WASD / left-stick — direct screen-space movement, transformed by the
//      camera yaw so "forward" always means "away from the camera".
//
// Aim direction follows the cursor (mouse world position) or right-stick.
// Dash bursts the player toward `aimDir` and grants brief invulnerability.
// ============================================================================
#pragma once
#include "Core/Math/Vector.h"
#include <optional>
#include <vector>

namespace dionite::player {

struct InputState {
    math::Vec2 move;             // -1..1 (left stick / WASD)
    math::Vec2 aim;              // -1..1 (right stick)
    math::Vec3 mouseWorld;       // ray-cast ground point of cursor
    bool       hasMouseWorld = false;
    bool       fire = false;
    bool       firePressed = false;
    bool       clickToMove = false; // RMB / single-finger tap
    bool       ability1 = false;
    bool       ability2 = false;
    bool       dashPressed = false;
    bool       interactPressed = false;
    bool       jumpPressed = false;
    float      cameraYawDeg = 0.f; // active camera yaw (used to rotate WASD)
};

struct PlayerStats {
    float maxHealth = 100.f, health = 100.f;
    float maxMana   = 50.f,  mana   = 50.f;
    float moveSpeed = 5.5f;      // m/s, Diablo-ish
    float dashCooldown = 1.6f;
    float dashTimer = 0.f;
    float invulnTimer = 0.f;
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
        if (stats_.dashTimer  > 0) stats_.dashTimer  -= dt;
        if (stats_.invulnTimer > 0) stats_.invulnTimer -= dt;

        // -- Click-to-move target update ----------------------------------
        if (in.clickToMove && in.hasMouseWorld) {
            navTarget_ = in.mouseWorld;
        }

        // -- Compute movement vector --------------------------------------
        math::Vec3 desiredMove{};
        // Mode 1: navigate toward target if we have one and player isn't using stick
        bool usingStick = (math::Vec2(in.move.x, in.move.y).length() > 0.18f);
        if (navTarget_ && !usingStick) {
            math::Vec3 to = *navTarget_ - position_;
            to.y = 0;
            if (to.length() < 0.25f) navTarget_.reset();
            else desiredMove = to.normalized();
        }
        // Mode 2: WASD/stick rotated by camera yaw so "up" is away from camera
        if (usingStick) {
            navTarget_.reset();
            float yr = in.cameraYawDeg * 0.01745329f;
            float cs = std::cos(yr), sn = std::sin(yr);
            math::Vec3 fwd{  sn, 0,  cs };
            math::Vec3 right{ cs, 0, -sn };
            desiredMove = fwd * (-in.move.y) + right * in.move.x;
            if (desiredMove.length() > 1.f) desiredMove = desiredMove.normalized();
        }

        position_ += desiredMove * stats_.moveSpeed * dt;

        // -- Aim direction (mouse > right stick > movement) ---------------
        if (in.hasMouseWorld) {
            math::Vec3 a = in.mouseWorld - position_; a.y = 0;
            if (a.length() > 0.1f) aimDir_ = a.normalized();
        } else if (math::Vec2(in.aim.x, in.aim.y).length() > 0.18f) {
            float yr = in.cameraYawDeg * 0.01745329f;
            float cs = std::cos(yr), sn = std::sin(yr);
            math::Vec3 fwd{  sn, 0,  cs };
            math::Vec3 right{ cs, 0, -sn };
            aimDir_ = (fwd * (-in.aim.y) + right * in.aim.x).normalized();
        } else if (desiredMove.length() > 0.1f) {
            aimDir_ = desiredMove.normalized();
        }

        // -- Dash ---------------------------------------------------------
        if (in.dashPressed && stats_.dashTimer <= 0.f) {
            position_ += aimDir_ * 6.5f;
            stats_.dashTimer = stats_.dashCooldown;
            stats_.invulnTimer = 0.35f;
        }

        // -- Mana regen ---------------------------------------------------
        if (stats_.mana < stats_.maxMana)
            stats_.mana = std::min(stats_.maxMana, stats_.mana + 6.f * dt);
    }

    math::Vec3 position() const { return position_; }
    math::Vec3 aimDir()   const { return aimDir_; }
    void setPosition(const math::Vec3& p) { position_ = p; }
    PlayerStats& stats()       { return stats_; }
    const PlayerStats& stats() const { return stats_; }

    bool invulnerable() const { return stats_.invulnTimer > 0; }
    void takeDamage(float dmg) {
        if (invulnerable()) return;
        stats_.health = std::max(0.f, stats_.health - dmg);
    }
    void heal(float amt) { stats_.health = std::min(stats_.maxHealth, stats_.health + amt); }
    bool dead() const { return stats_.health <= 0.f; }

    const std::optional<math::Vec3>& navTarget() const { return navTarget_; }
    void clearNavTarget() { navTarget_.reset(); }

private:
    math::Vec3 position_{0, 0, 0};
    math::Vec3 aimDir_{0, 0, 1};
    std::optional<math::Vec3> navTarget_;
    PlayerStats stats_;
};

} // namespace dionite::player
