// ============================================================================
// Dionite — World: Dynamic events (invasions, blood moons, treasure goblins)
// ============================================================================
#pragma once
#include <string>
#include <functional>
#include <vector>
#include "Core/Math/Random.h"

namespace dionite::world {

struct DynamicEvent {
    std::string id;
    std::string name;
    std::string description;
    float weight; // selection weight
    float durationSec;
    std::function<void()> onStart;
    std::function<void(float)> onTick;
    std::function<void()> onEnd;
};

class EventScheduler {
public:
    EventScheduler(uint64_t seed) : rng_(seed) {}

    void registerEvent(DynamicEvent e) { pool_.push_back(std::move(e)); }

    void update(float dt) {
        cooldown_ -= dt;
        if (active_) {
            elapsed_ += dt;
            if (active_->onTick) active_->onTick(dt);
            if (elapsed_ >= active_->durationSec) {
                if (active_->onEnd) active_->onEnd();
                active_ = nullptr;
                cooldown_ = rng_.rangeF(60.f, 180.f);
            }
        } else if (cooldown_ <= 0 && !pool_.empty()) {
            // weighted pick
            float total = 0.f;
            for (auto& e : pool_) total += e.weight;
            float r = rng_.rangeF(0.f, total);
            for (auto& e : pool_) {
                r -= e.weight;
                if (r <= 0.f) { active_ = &e; elapsed_ = 0.f; if (e.onStart) e.onStart(); break; }
            }
        }
    }

    const DynamicEvent* active() const { return active_; }

private:
    math::Random rng_;
    std::vector<DynamicEvent> pool_;
    DynamicEvent* active_ = nullptr;
    float elapsed_ = 0.f;
    float cooldown_ = 30.f;
};

} // namespace dionite::world
