// ============================================================================
// Dionite — World: NavMesh (grid-based A* for click-to-move pathfinding)
// Operates on the Dungeon's tile grid. Returns waypoints in world space.
// ============================================================================
#pragma once
#include "DungeonGenerator.h"
#include "Core/Math/Vector.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>

namespace dionite::world {

class NavGrid {
public:
    NavGrid(const Dungeon& d, float tileSize = 2.0f) : d_(d), tile_(tileSize) {}

    bool walkable(int x, int y) const {
        if (x < 0 || y < 0 || x >= d_.width || y >= d_.height) return false;
        Tile t = d_.at(x, y);
        return t == Tile::Floor || t == Tile::Spawn || t == Tile::Chest || t == Tile::BossPad || t == Tile::Door;
    }

    // Find a path of world-space waypoints from `startWorld` to `goalWorld`.
    // Empty result means no path (or already there).
    std::vector<math::Vec3> findPath(const math::Vec3& startWorld, const math::Vec3& goalWorld) const {
        const int sx = (int)std::round(startWorld.x / tile_);
        const int sy = (int)std::round(startWorld.z / tile_);
        const int gx = (int)std::round(goalWorld.x / tile_);
        const int gy = (int)std::round(goalWorld.z / tile_);
        if (sx == gx && sy == gy) return {};

        auto h = [&](int x, int y) {
            return std::hypot((float)(gx - x), (float)(gy - y));
        };
        struct Node { int x, y; float f; };
        struct Cmp  { bool operator()(const Node& a, const Node& b) const { return a.f > b.f; } };
        std::priority_queue<Node, std::vector<Node>, Cmp> open;
        std::unordered_map<long long, long long> came;
        std::unordered_map<long long, float> g;
        auto key = [&](int x, int y) { return (long long)y * d_.width + x; };

        open.push({sx, sy, 0});
        g[key(sx, sy)] = 0;

        const int dx[8] = {1,-1,0,0, 1,1,-1,-1};
        const int dy[8] = {0,0,1,-1, 1,-1,1,-1};

        while (!open.empty()) {
            auto cur = open.top(); open.pop();
            if (cur.x == gx && cur.y == gy) break;
            for (int i = 0; i < 8; ++i) {
                int nx = cur.x + dx[i], ny = cur.y + dy[i];
                if (!walkable(nx, ny)) continue;
                // No corner-cutting through walls
                if (i >= 4 && (!walkable(cur.x + dx[i], cur.y) || !walkable(cur.x, cur.y + dy[i]))) continue;
                float step = (i < 4) ? 1.f : 1.414f;
                float tentative = g[key(cur.x, cur.y)] + step;
                long long nk = key(nx, ny);
                if (!g.count(nk) || tentative < g[nk]) {
                    g[nk] = tentative;
                    came[nk] = key(cur.x, cur.y);
                    open.push({nx, ny, tentative + h(nx, ny)});
                }
            }
        }
        // Reconstruct
        std::vector<math::Vec3> path;
        long long k = key(gx, gy);
        if (!came.count(k)) return path; // unreachable
        while (k != key(sx, sy)) {
            int x = (int)(k % d_.width), y = (int)(k / d_.width);
            path.push_back({ x * tile_, 0, y * tile_ });
            k = came[k];
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

private:
    const Dungeon& d_;
    float tile_;
};

} // namespace dionite::world
