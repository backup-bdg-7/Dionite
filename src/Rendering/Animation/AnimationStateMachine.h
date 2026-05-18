// AnimationStateMachine.h — Mecanim-style state machine for character motion.
// Provides locomotion blend tree (idle ↔ walk ↔ run), action one-shots (attack/dodge/cast),
// and upper-body additive overlays (firing, channeling, taunting).
#pragma once
#include "AnimationClip.h"
#include "AnimationSampler.h"
#include "Skeleton.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace dionite::anim {

struct LocomotionBlend {
    const AnimationClip* idle = nullptr;
    const AnimationClip* walk = nullptr;
    const AnimationClip* run  = nullptr;
    float speed = 0.f;              // 0..maxSpeed metres/sec
    float maxRunSpeed = 7.0f;       // sprint threshold
    float idleToWalk  = 0.05f;      // m/s
    float walkToRun   = 3.5f;
    float blendT      = 0.f;        // sampler time accumulators
};

struct ActionState {
    const AnimationClip* clip = nullptr;
    float t = 0.f;
    float blendInDuration  = 0.15f;
    float blendOutDuration = 0.20f;
    float weight = 0.f;             // ramped in/out
    bool  active = false;
    std::function<void(const AnimationClip::Event&)> onEvent;
    int   lastFiredEventIndex = -1;
};

class AnimationStateMachine {
public:
    LocomotionBlend                       locomotion;
    std::unordered_map<std::string, std::unique_ptr<ActionState>> actions;
    std::unordered_map<std::string, const AnimationClip*>         additives; // weight per name
    std::unordered_map<std::string, float>                        additiveWeights;

    void setSpeed(float metresPerSec) { locomotion.speed = metresPerSec; }

    void playAction(const std::string& name, const AnimationClip* clip,
                    std::function<void(const AnimationClip::Event&)> onEvent = nullptr) {
        auto s = std::make_unique<ActionState>();
        s->clip = clip; s->t = 0.f; s->weight = 0.f; s->active = true;
        s->onEvent = std::move(onEvent);
        actions[name] = std::move(s);
    }

    void setAdditive(const std::string& name, const AnimationClip* clip, float w) {
        additives[name] = clip;
        additiveWeights[name] = w;
    }

    void tick(float dt, Skeleton& skel);
};

} // namespace dionite::anim
