// ============================================================================
// Dionite — Combat: Boss AI (two-phase, telegraphed attacks)
// ============================================================================
#pragma once
#include "EnemyAI.h"
#include "Core/Math/Random.h"
#include <vector>
#include <functional>

namespace dionite::combat {

enum class BossPhase { Intro, Phase1, Transition, Phase2, Death };

struct BossPattern {
    std::string id;
    float duration;
    std::function<void(EnemyInstance&, float dt, const math::Vec3& playerPos)> tick;
};

struct BossInstance {
    EnemyInstance base;
    BossPhase phase = BossPhase::Intro;
    float phaseTimer = 0;
    int currentPattern = 0;
    float patternTimer = 0;
    std::vector<BossPattern> phase1Patterns;
    std::vector<BossPattern> phase2Patterns;
    std::string name;
};

class BossAI {
public:
    void update(BossInstance& b, const math::Vec3& playerPos, float dt) {
        b.phaseTimer -= dt;
        b.patternTimer -= dt;
        if (b.base.stats.hp <= 0) { b.phase = BossPhase::Death; return; }
        if (b.phase == BossPhase::Intro && b.phaseTimer <= 0) { b.phase = BossPhase::Phase1; }
        if (b.phase == BossPhase::Phase1 && b.base.stats.hp < b.base.stats.maxHp * 0.5f) {
            b.phase = BossPhase::Transition; b.phaseTimer = 2.f;
        }
        if (b.phase == BossPhase::Transition && b.phaseTimer <= 0) {
            b.phase = BossPhase::Phase2;
            b.currentPattern = 0;
        }
        auto& patterns = (b.phase == BossPhase::Phase1) ? b.phase1Patterns : b.phase2Patterns;
        if (patterns.empty()) return;
        if (b.patternTimer <= 0) {
            b.currentPattern = (b.currentPattern + 1) % (int)patterns.size();
            b.patternTimer = patterns[b.currentPattern].duration;
        }
        if (patterns[b.currentPattern].tick)
            patterns[b.currentPattern].tick(b.base, dt, playerPos);
    }
};

} // namespace dionite::combat
