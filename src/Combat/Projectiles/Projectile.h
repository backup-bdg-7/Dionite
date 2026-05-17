// ============================================================================
// Dionite — Combat: Projectile entity (gravity-less, lifetime, on-hit callback)
// ============================================================================
#pragma once
#include "Core/Math/Vector.h"
#include <string>
#include <vector>
#include <functional>

namespace dionite::combat {

struct Projectile {
    math::Vec3 position;
    math::Vec3 velocity;
    float damage = 0.f;
    float radius = 6.f;
    float lifetime = 2.0f;
    int   pierce = 0;
    bool  alive = true;
    std::string color;
    std::string ownerTag = "player"; // "player" or "enemy"
};

class ProjectilePool {
public:
    Projectile* spawn() {
        for (auto& p : pool_) if (!p.alive) { p = Projectile{}; p.alive = true; return &p; }
        pool_.emplace_back();
        auto& p = pool_.back();
        p.alive = true;
        return &p;
    }

    void update(float dt) {
        for (auto& p : pool_) {
            if (!p.alive) continue;
            p.position += p.velocity * dt;
            p.lifetime -= dt;
            if (p.lifetime <= 0) p.alive = false;
        }
    }

    template <typename Pred>
    void forEachAlive(Pred fn) {
        for (auto& p : pool_) if (p.alive) fn(p);
    }

    void clear() { pool_.clear(); }

private:
    std::vector<Projectile> pool_;
};

} // namespace dionite::combat
