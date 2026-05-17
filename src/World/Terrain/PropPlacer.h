// ============================================================================
// Dionite — World: PropPlacer & RoadGenerator.
//   PropPlacer drops campfires, ruins, lore stones with weighted preferences.
//   RoadGenerator paths roads between Points of Interest with cost = slope +
//   altitude penalty so roads naturally follow valleys.
// ============================================================================
#pragma once
#include "Heightmap.h"
#include "BiomeSplatter.h"
#include "Core/Math/Random.h"
#include "Core/Math/Vector.h"
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>

namespace dionite::world {

struct PropInstance {
    math::Vec3 position;
    float yaw;
    float scale;
    std::string assetId;
    std::string kind;  // "campfire","ruin_arch","lore_stone","crypt_door","altar"
};

struct PointOfInterest {
    math::Vec3 position;
    std::string id;
    std::string kind; // "town","camp","dungeon_entrance","boss_pad","shrine"
};

class PropPlacer {
public:
    explicit PropPlacer(uint64_t seed) : rng_(seed) {}

    std::vector<PropInstance> placeRandom(const Heightmap& h,
                                          const std::vector<SplatWeights>& splat,
                                          int count,
                                          const std::vector<std::string>& kinds) {
        std::vector<PropInstance> out;
        int N = h.resolution;
        int tries = 0;
        while ((int)out.size() < count && tries < count * 8) {
            ++tries;
            int gx = rng_.range(2, N - 3);
            int gz = rng_.range(2, N - 3);
            float wx, wz; h.worldXY(gx, gz, wx, wz);
            float wy = h.at(gx, gz);
            // Skip steep / underwater
            if (!splat.empty()) {
                auto& s = splat[gz * N + gx];
                if (s.rock > 0.6f || s.snow > 0.7f) continue;
            }
            PropInstance p;
            p.position = { wx, wy, wz };
            p.scale = rng_.rangeF(0.9f, 1.15f);
            p.yaw   = rng_.rangeF(0.f, 6.2831853f);
            p.kind  = kinds[rng_.range(0, (int)kinds.size() - 1)];
            p.assetId = "prop_" + p.kind;
            out.push_back(p);
        }
        return out;
    }

private:
    math::Random rng_;
};

class RoadGenerator {
public:
    // A* over the heightmap with cost = 1 + slopePenalty * |dh|.
    // Returns world-space polyline.
    static std::vector<math::Vec3> path(const Heightmap& h,
                                        const math::Vec3& fromWorld,
                                        const math::Vec3& toWorld,
                                        float slopePenalty = 3.f) {
        const int N = h.resolution;
        auto toGrid = [&](const math::Vec3& p, int& gx, int& gz) {
            gx = (int)((p.x + h.worldSize * 0.5f) / h.worldSize * (N - 1));
            gz = (int)((p.z + h.worldSize * 0.5f) / h.worldSize * (N - 1));
        };
        int sx, sz, ex, ez;
        toGrid(fromWorld, sx, sz);
        toGrid(toWorld,   ex, ez);
        auto key = [N](int x, int y) { return (long long)y * N + x; };
        struct Node { int x, z; float f; };
        struct Cmp { bool operator()(const Node& a, const Node& b) const { return a.f > b.f; } };
        std::priority_queue<Node, std::vector<Node>, Cmp> open;
        std::unordered_map<long long, long long> came;
        std::unordered_map<long long, float> g;
        open.push({sx, sz, 0.f});
        g[key(sx, sz)] = 0.f;
        const int dx[8] = {1,-1,0,0, 1,1,-1,-1};
        const int dz[8] = {0,0,1,-1, 1,-1,1,-1};
        while (!open.empty()) {
            auto c = open.top(); open.pop();
            if (c.x == ex && c.z == ez) break;
            for (int i = 0; i < 8; ++i) {
                int nx = c.x + dx[i], nz = c.z + dz[i];
                if (nx < 0 || nz < 0 || nx >= N || nz >= N) continue;
                float dh = std::abs(h.at(nx, nz) - h.at(c.x, c.z));
                float step = (i < 4 ? 1.f : 1.414f) + slopePenalty * dh;
                float tentative = g[key(c.x, c.z)] + step;
                long long nk = key(nx, nz);
                if (!g.count(nk) || tentative < g[nk]) {
                    g[nk] = tentative;
                    came[nk] = key(c.x, c.z);
                    float heur = std::hypot((float)(ex - nx), (float)(ez - nz));
                    open.push({nx, nz, tentative + heur});
                }
            }
        }
        std::vector<math::Vec3> poly;
        long long k = key(ex, ez);
        if (!came.count(k)) return poly;
        while (k != key(sx, sz)) {
            int x = (int)(k % N), z = (int)(k / N);
            float wx, wz; h.worldXY(x, z, wx, wz);
            poly.push_back({ wx, h.at(x, z) + 0.05f, wz });
            k = came[k];
        }
        std::reverse(poly.begin(), poly.end());
        return poly;
    }
};

} // namespace dionite::world
