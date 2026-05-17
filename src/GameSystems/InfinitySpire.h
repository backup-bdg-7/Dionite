// ============================================================================
// Dionite — Game Systems: Infinity Spire endgame (escalating floors + affixes + leaderboard)
// ============================================================================
#pragma once
#include "Core/Math/Random.h"
#include "World/DungeonGenerator/DungeonGenerator.h"
#include <string>
#include <vector>

namespace dionite::game {

struct SpireRun {
    int   floor = 1;
    float elapsed = 0.f;
    int   killCount = 0;
    int   score = 0;
    std::string biome;
    std::vector<world::SigilAffix> currentAffixes;
};

class InfinitySpire {
public:
    explicit InfinitySpire(uint64_t seed) : rng_(seed) {}

    SpireRun startRun(int startingFloor = 1) {
        SpireRun r;
        r.floor = startingFloor;
        r.biome = pickBiome();
        r.currentAffixes = world::DungeonGenerator(rng_.next()).generate(8, startingFloor).affixes;
        return r;
    }

    // Advance one floor; difficulty escalates exponentially.
    void advance(SpireRun& r) {
        r.floor += 1;
        r.score += r.floor * 100;
        r.biome = pickBiome();
        r.currentAffixes = world::DungeonGenerator(rng_.next()).generate(8, r.floor).affixes;
    }

    float enemyHpMult(int floor) const { return 1.f + floor * 0.18f; }
    float enemyDmgMult(int floor) const { return 1.f + floor * 0.12f; }
    int   goldDropMult(int floor) const { return 1 + floor / 4; }

private:
    std::string pickBiome() {
        static const char* biomes[] = {"verdant_wilds","ashen_wastes","frozen_spire","sunken_crypts","sky_citadel"};
        return biomes[rng_.range(0, 4)];
    }
    math::Random rng_;
};

} // namespace dionite::game
