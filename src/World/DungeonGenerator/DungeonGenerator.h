// ============================================================================
// Dionite — World: BSP-based procedural dungeon generator with sigil affixes
// ============================================================================
#pragma once
#include "Core/Math/Random.h"
#include "Core/Math/Vector.h"
#include <vector>
#include <string>
#include <memory>

namespace dionite::world {

struct Room {
    int x, y, w, h;
    bool isBoss = false;
    bool isTreasure = false;
    int cx() const { return x + w / 2; }
    int cy() const { return y + h / 2; }
};

struct Corridor {
    int x1, y1, x2, y2;
};

enum class Tile : uint8_t { Void = 0, Floor = 1, Wall = 2, Door = 3, Chest = 4, BossPad = 5, Spawn = 6 };

struct SigilAffix {
    std::string id;
    std::string name;
    std::string description;
    float damageMult = 1.f;
    float enemyHpMult = 1.f;
    float lootBoost = 1.f;
    float playerSpeedMult = 1.f;
};

struct Dungeon {
    int width;
    int height;
    std::vector<Tile> tiles;
    std::vector<Room> rooms;
    std::vector<Corridor> corridors;
    std::vector<SigilAffix> affixes;
    int spawnX = 0, spawnY = 0;
    Tile at(int x, int y) const { return tiles[y * width + x]; }
    void set(int x, int y, Tile t) { tiles[y * width + x] = t; }
};

class DungeonGenerator {
public:
    DungeonGenerator(uint64_t seed, int width = 96, int height = 96)
        : rng_(seed), width_(width), height_(height) {}

    Dungeon generate(int numRooms = 10, int floor = 1) {
        Dungeon d;
        d.width = width_; d.height = height_;
        d.tiles.assign((size_t)width_ * height_, Tile::Void);

        // 1) Carve rooms
        for (int i = 0; i < numRooms; ++i) {
            int w = rng_.range(6, 14);
            int h = rng_.range(6, 14);
            int x = rng_.range(2, width_ - w - 2);
            int y = rng_.range(2, height_ - h - 2);
            Room r{x, y, w, h};
            bool overlap = false;
            for (auto& o : d.rooms)
                if (x < o.x + o.w + 2 && x + w + 2 > o.x &&
                    y < o.y + o.h + 2 && y + h + 2 > o.y) { overlap = true; break; }
            if (overlap) continue;
            d.rooms.push_back(r);
            for (int yy = y; yy < y + h; ++yy)
                for (int xx = x; xx < x + w; ++xx)
                    d.set(xx, yy, Tile::Floor);
        }

        // 2) Connect rooms with L-shaped corridors
        for (size_t i = 1; i < d.rooms.size(); ++i) {
            auto& a = d.rooms[i - 1];
            auto& b = d.rooms[i];
            int x1 = a.cx(), y1 = a.cy();
            int x2 = b.cx(), y2 = b.cy();
            if (rng_.chance(0.5f)) {
                for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x) d.set(x, y1, Tile::Floor);
                for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y) d.set(x2, y, Tile::Floor);
            } else {
                for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y) d.set(x1, y, Tile::Floor);
                for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x) d.set(x, y2, Tile::Floor);
            }
            d.corridors.push_back({x1, y1, x2, y2});
        }

        // 3) Walls around floor tiles
        for (int y = 0; y < height_; ++y) {
            for (int x = 0; x < width_; ++x) {
                if (d.at(x, y) != Tile::Floor) continue;
                for (int dy = -1; dy <= 1; ++dy)
                    for (int dx = -1; dx <= 1; ++dx) {
                        int nx = x + dx, ny = y + dy;
                        if (nx < 0 || ny < 0 || nx >= width_ || ny >= height_) continue;
                        if (d.at(nx, ny) == Tile::Void) d.set(nx, ny, Tile::Wall);
                    }
            }
        }

        // 4) Designate spawn, boss, treasure
        if (!d.rooms.empty()) {
            d.spawnX = d.rooms.front().cx();
            d.spawnY = d.rooms.front().cy();
            d.set(d.spawnX, d.spawnY, Tile::Spawn);
            auto& boss = d.rooms.back();
            boss.isBoss = true;
            d.set(boss.cx(), boss.cy(), Tile::BossPad);
            if (d.rooms.size() >= 3) {
                auto& chest = d.rooms[d.rooms.size() / 2];
                chest.isTreasure = true;
                d.set(chest.cx(), chest.cy(), Tile::Chest);
            }
        }

        // 5) Apply sigil affixes (escalating from floor)
        d.affixes = rollAffixes(floor);
        return d;
    }

private:
    std::vector<SigilAffix> rollAffixes(int floor) {
        std::vector<SigilAffix> pool = {
            {"sigil_pain", "Sigil of Pain", "Enemies deal +35% damage.", 1.0f, 1.0f, 1.0f, 1.0f},
            {"sigil_might", "Sigil of Might", "Enemies have +50% HP.", 1.0f, 1.5f, 1.0f, 1.0f},
            {"sigil_swiftness", "Sigil of Swiftness", "+25% player move speed, enemies +20% damage.", 1.0f, 1.0f, 1.0f, 1.25f},
            {"sigil_avarice", "Sigil of Avarice", "Loot quality +75%, enemies +30% HP.", 1.0f, 1.3f, 1.75f, 1.0f},
            {"sigil_inferno", "Sigil of Inferno", "All damage is fire. +20% damage to all.", 1.2f, 1.0f, 1.0f, 1.0f},
            {"sigil_frost", "Sigil of Frost", "Enemies move 30% slower. +10% damage.", 1.1f, 1.0f, 1.0f, 1.0f},
        };
        int count = 1 + floor / 5;
        std::vector<SigilAffix> out;
        for (int i = 0; i < count && !pool.empty(); ++i) {
            int idx = rng_.range(0, (int)pool.size() - 1);
            out.push_back(pool[idx]);
            pool.erase(pool.begin() + idx);
        }
        return out;
    }

    math::Random rng_;
    int width_, height_;
};

} // namespace dionite::world
