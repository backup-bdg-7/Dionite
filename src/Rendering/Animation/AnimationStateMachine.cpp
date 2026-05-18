#include "AnimationStateMachine.h"
#include <algorithm>
#include <cmath>

namespace dionite::anim {

static float saturate(float v) { return std::min(std::max(v, 0.f), 1.f); }

void AnimationStateMachine::tick(float dt, Skeleton& skel) {
    // -------- 1. Locomotion blend tree (idle ↔ walk ↔ run) --------
    locomotion.blendT += dt;
    float s = locomotion.speed;
    if (locomotion.idle && locomotion.walk && locomotion.run) {
        if (s < locomotion.walkToRun) {
            // idle ↔ walk
            float a = saturate((s - locomotion.idleToWalk) /
                               std::max(0.001f, (locomotion.walkToRun - locomotion.idleToWalk)));
            AnimationSampler::blend(*locomotion.idle, locomotion.blendT,
                                    *locomotion.walk, locomotion.blendT, a, skel);
        } else {
            float a = saturate((s - locomotion.walkToRun) /
                               std::max(0.001f, (locomotion.maxRunSpeed - locomotion.walkToRun)));
            AnimationSampler::blend(*locomotion.walk, locomotion.blendT,
                                    *locomotion.run,  locomotion.blendT, a, skel);
        }
    } else if (locomotion.idle) {
        AnimationSampler::sample(*locomotion.idle, locomotion.blendT, skel);
    }

    // -------- 2. One-shot actions overlay --------
    for (auto it = actions.begin(); it != actions.end(); ) {
        ActionState& a = *it->second;
        if (!a.clip) { it = actions.erase(it); continue; }
        a.t += dt;
        // weight ramp
        if (a.active && a.t < a.clip->duration - a.blendOutDuration)
            a.weight = std::min(1.f, a.weight + dt / std::max(0.001f, a.blendInDuration));
        else
            a.weight = std::max(0.f, a.weight - dt / std::max(0.001f, a.blendOutDuration));

        // fire events
        for (size_t i = 0; i < a.clip->events.size(); ++i) {
            const auto& ev = a.clip->events[i];
            if (ev.t <= a.t && (int)i > a.lastFiredEventIndex) {
                if (a.onEvent) a.onEvent(ev);
                a.lastFiredEventIndex = (int)i;
            }
        }

        if (a.weight > 0.f) AnimationSampler::applyAdditive(*a.clip, a.t, a.weight, skel);
        if (a.t > a.clip->duration && a.weight <= 0.0001f) it = actions.erase(it);
        else ++it;
    }

    // -------- 3. Additive overlays (e.g., aim-offset, breathing) --------
    for (auto& [name, clip] : additives) {
        float w = additiveWeights[name];
        if (clip && w > 0.f) AnimationSampler::applyAdditive(*clip, locomotion.blendT, w, skel);
    }
}

} // namespace dionite::anim
