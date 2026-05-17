// ============================================================================
// Dionite — UI: Animator utility for tween-driven UI animations
// ============================================================================
#pragma once
#include <functional>
#include <vector>

namespace dionite::ui {

struct Tween {
    float duration;
    float elapsed = 0.f;
    std::function<void(float t)> update; // t in 0..1
    std::function<void()> onComplete;
    bool done = false;
};

class UIAnimator {
public:
    void add(Tween t) { tweens_.push_back(std::move(t)); }
    void update(float dt) {
        for (auto& t : tweens_) {
            t.elapsed += dt;
            float u = std::min(1.f, t.elapsed / t.duration);
            if (t.update) t.update(u);
            if (u >= 1.f && !t.done) { t.done = true; if (t.onComplete) t.onComplete(); }
        }
        tweens_.erase(std::remove_if(tweens_.begin(), tweens_.end(),
            [](const Tween& t) { return t.done; }), tweens_.end());
    }
private:
    std::vector<Tween> tweens_;
};

} // namespace dionite::ui
