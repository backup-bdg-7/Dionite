// ============================================================================
// Dionite — World: 3D dungeon mesh emitter.
// Takes the BSP `Dungeon` (tile grid) and produces structured 3D draw data:
//   - floor quads (one per Floor tile)
//   - wall quads (one per Wall tile, height ~3.2m)
//   - prop anchors (chests, boss pads, spawn marker)
//   - point-light anchors (one per room, randomized warm/cool)
// The platform renderer translates these into Mesh / Texture / DrawCommand.
// ============================================================================
#pragma once
#include "DungeonGenerator.h"
#include "Core/Math/Vector.h"
#include <vector>
#include <string>

namespace dionite::world {

struct FloorTile  { math::Vec3 worldPos; float size; std::string biomeMatId; };
struct WallSegment { math::Vec3 worldPos; float width; float height; std::string biomeMatId; };
struct PropAnchor { math::Vec3 worldPos; std::string kind; }; // "chest","boss_pad","spawn","torch"
struct PointLight { math::Vec3 worldPos; float radius; float intensity; float r, g, b; };

struct DungeonMesh {
    std::vector<FloorTile> floors;
    std::vector<WallSegment> walls;
    std::vector<PropAnchor> props;
    std::vector<PointLight> lights;
    math::Vec3 spawnWorld;
};

class DungeonMeshBuilder {
public:
    static constexpr float TILE = 2.0f;
    static constexpr float WALL_HEIGHT = 3.2f;

    DungeonMesh build(const Dungeon& d, const std::string& biomeMatId) const {
        DungeonMesh m;
        m.spawnWorld = { d.spawnX * TILE, 0, d.spawnY * TILE };
        for (int y = 0; y < d.height; ++y) {
            for (int x = 0; x < d.width; ++x) {
                Tile t = d.at(x, y);
                math::Vec3 wp{ x * TILE, 0, y * TILE };
                if (t == Tile::Floor || t == Tile::Spawn || t == Tile::Chest || t == Tile::BossPad) {
                    m.floors.push_back({ wp, TILE, biomeMatId });
                }
                if (t == Tile::Wall) {
                    m.walls.push_back({ wp, TILE, WALL_HEIGHT, biomeMatId });
                }
                if (t == Tile::Chest)   m.props.push_back({ wp, "chest" });
                if (t == Tile::BossPad) m.props.push_back({ wp, "boss_pad" });
                if (t == Tile::Spawn)   m.props.push_back({ wp, "spawn" });
            }
        }
        // One ambient point light per room (alternating warm/cool)
        int i = 0;
        for (const auto& r : d.rooms) {
            math::Vec3 c{ r.cx() * TILE, 1.8f, r.cy() * TILE };
            bool warm = (i++ % 2 == 0);
            m.lights.push_back({ c, 8.0f, 1.4f,
                                  warm ? 1.0f : 0.6f, warm ? 0.78f : 0.78f, warm ? 0.45f : 1.0f });
            // Torches at room corners
            m.props.push_back({ { (float)r.x        * TILE, 1.6f, (float)r.y        * TILE }, "torch" });
            m.props.push_back({ { (float)(r.x+r.w-1)* TILE, 1.6f, (float)(r.y+r.h-1)* TILE }, "torch" });
        }
        return m;
    }
};

} // namespace dionite::world
