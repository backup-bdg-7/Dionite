// ============================================================================
// Dionite — World: Foliage scatter using Poisson-disc sampling per density layer.
// Outputs world-space instance transforms for trees, rocks, grass, mushrooms,
// etc., that the renderer instances via GPU instancing.
// ============================================================================
#pragma once
#include "Heightmap.h"
#include "BiomeSplatter.h"
#include "Core/Math/Random.h"
#include "Core/Math/Vector.h"
#include <vector>
#include <string>

namespace dionite::world {

struct FoliageRule {
    std::string assetId;     // mesh id in renderer
    float minSpacing;        // Poisson radius (m)
    float minAlt = -1e9f, maxAlt = 1e9f;
    float minGrassWeight = 0.f;
    float maxSlope = 1.f;
    float minScale = 0.85f, maxScale = 1.20f;
    float density = 1.f;     // 0..1 multiplier
    bool  alignToNormal = true;
};

struct FoliageInstance {
    math::Vec3 position;
    math::Vec3 normal;
    float scale;
    float yaw;          // radians
    std::string assetId;
};

class FoliageScatter {
public:
    explicit FoliageScatter(uint64_t seed) : rng_(seed) {}

    std::vector<FoliageInstance>
    scatter(const Heightmap& h, const std::vector<SplatWeights>& splat,
            const std::vector<FoliageRule>& rules) {
        std::vector<FoliageInstance> out;
        const int N = h.resolution;
        for (const auto& rule : rules) {
            // Sample cell size based on min spacing
            float cell = rule.minSpacing;
            int cellsX = (int)(h.worldSize / cell);
            int cellsZ = (int)(h.worldSize / cell);
            for (int cz = 0; cz < cellsZ; ++cz) {
                for (int cx = 0; cx < cellsX; ++cx) {
                    if (!rng_.chance(rule.density)) continue;
                    float jx = rng_.rangeF(0.f, cell) - cell * 0.5f;
                    float jz = rng_.rangeF(0.f, cell) - cell * 0.5f;
                    float wx = -h.worldSize * 0.5f + cx * cell + cell * 0.5f + jx;
                    float wz = -h.worldSize * 0.5f + cz * cell + cell * 0.5f + jz;
                    // Convert back to heightmap index
                    int gx = (int)((wx + h.worldSize * 0.5f) / h.worldSize * (N - 1));
                    int gz = (int)((wz + h.worldSize * 0.5f) / h.worldSize * (N - 1));
                    if (gx < 0 || gx >= N || gz < 0 || gz >= N) continue;
                    float wy = h.at(gx, gz);
                    if (wy < rule.minAlt || wy > rule.maxAlt) continue;
                    // Splat weight check
                    if (!splat.empty()) {
                        const auto& s = splat[gz * N + gx];
                        if (s.grass < rule.minGrassWeight) continue;
                    }
                    math::Vec3 n = computeNormal(h, gx, gz);
                    float slope = 1.f - n.y;
                    if (slope > rule.maxSlope) continue;
                    FoliageInstance fi;
                    fi.position = { wx, wy, wz };
                    fi.normal   = rule.alignToNormal ? n : math::Vec3(0, 1, 0);
                    fi.scale    = rng_.rangeF(rule.minScale, rule.maxScale);
                    fi.yaw      = rng_.rangeF(0.f, 6.2831853f);
                    fi.assetId  = rule.assetId;
                    out.push_back(fi);
                }
            }
        }
        return out;
    }

    // Convenience presets per biome
    static std::vector<FoliageRule> rulesFor(BiomeId b) {
        switch (b) {
            case BiomeId::VerdantWilds:
                return {
                    {"tree_oak",    5.0f, 5.f, 60.f, 0.45f, 0.30f, 0.85f, 1.30f, 0.85f, true},
                    {"tree_birch",  4.0f, 8.f, 50.f, 0.50f, 0.30f, 0.80f, 1.15f, 0.55f, true},
                    {"rock_mossy",  2.4f, 4.f, 80.f, 0.10f, 0.55f, 0.80f, 1.40f, 0.40f, false},
                    {"bush_fern",   1.6f, 5.f, 30.f, 0.55f, 0.25f, 0.80f, 1.10f, 0.85f, true},
                    {"grass_clump", 0.7f, 5.f, 45.f, 0.55f, 0.20f, 0.80f, 1.10f, 1.00f, true},
                    {"mushroom",    3.0f, 5.f, 30.f, 0.40f, 0.30f, 0.80f, 1.20f, 0.20f, true},
                };
            case BiomeId::AshenWastes:
                return {
                    {"tree_charred",6.0f, 4.f, 70.f, 0.05f, 0.40f, 0.80f, 1.40f, 0.45f, true},
                    {"rock_obsidian",3.0f,3.f,120.f, 0.00f, 0.70f, 0.80f, 1.50f, 0.65f, false},
                    {"ember_brazier",8.0f,5.f,60.f, 0.00f, 0.35f, 0.95f, 1.10f, 0.10f, false},
                    {"bone_pile",   4.0f, 3.f, 50.f, 0.05f, 0.45f, 0.90f, 1.20f, 0.25f, false},
                };
            case BiomeId::FrozenSpire:
                return {
                    {"tree_pine",   4.5f, 8.f, 60.f, 0.30f, 0.30f, 0.85f, 1.30f, 0.85f, true},
                    {"rock_ice",    2.6f, 6.f,120.f, 0.00f, 0.70f, 0.85f, 1.40f, 0.50f, false},
                    {"icicle_cluster",3.5f,2.f,40.f,0.05f, 0.40f, 0.85f, 1.20f, 0.35f, true},
                    {"snow_drift",  6.0f, 0.f, 50.f, 0.05f, 0.30f, 1.00f, 1.40f, 0.40f, false},
                };
            case BiomeId::SunkenCrypts:
                return {
                    {"tree_dead",   5.0f, 0.f, 30.f, 0.10f, 0.40f, 0.80f, 1.20f, 0.55f, true},
                    {"rock_coral",  2.4f, 0.f, 25.f, 0.05f, 0.60f, 0.85f, 1.30f, 0.50f, false},
                    {"reed_cluster",0.8f, 0.f, 10.f, 0.50f, 0.20f, 0.85f, 1.10f, 0.95f, true},
                    {"ruin_pillar", 8.0f, 0.f, 22.f, 0.00f, 0.40f, 1.00f, 1.10f, 0.20f, false},
                };
            case BiomeId::SkyCitadel:
                return {
                    {"cloud_shrub", 3.0f,20.f, 90.f, 0.30f, 0.30f, 0.80f, 1.20f, 0.55f, true},
                    {"rock_gold",   3.2f,15.f,150.f, 0.00f, 0.65f, 0.85f, 1.40f, 0.45f, false},
                    {"banner",      9.0f,20.f,100.f, 0.05f, 0.30f, 0.95f, 1.05f, 0.10f, false},
                };
        }
        return {};
    }

private:
    static math::Vec3 computeNormal(const Heightmap& h, int x, int y) {
        float l = h.at(x - 1, y), r = h.at(x + 1, y);
        float d = h.at(x, y - 1), u = h.at(x, y + 1);
        float scale = h.worldSize / (h.resolution - 1);
        math::Vec3 dx{ 2 * scale, r - l, 0 };
        math::Vec3 dz{ 0,         u - d, 2 * scale };
        return dx.cross(dz).normalized();
    }

    math::Random rng_;
};

} // namespace dionite::world
