// ============================================================================
// Dionite — Desktop entry point: headless 3D validation harness.
// Generates a dungeon, builds its 3D mesh, computes a path with NavGrid, and
// runs the game loop for ~240 frames to exercise every system end-to-end.
// ============================================================================
#include "Core/Logging/Logger.h"
#include "Core/Utils/Time.h"
#include "Player/Controller/PlayerController.h"
#include "Player/Camera/GameCamera.h"
#include "World/DungeonGenerator/DungeonGenerator.h"
#include "World/DungeonGenerator/DungeonMeshBuilder.h"
#include "World/DungeonGenerator/NavGrid.h"
#include "Combat/Weapons/WeaponBase.h"
#include "Combat/Projectiles/Projectile.h"
#include "Loot/Items/ItemBase.h"
#include "GameSystems/InfinitySpire.h"
#include "Rendering/Renderer.h"
#include <memory>

using namespace dionite;

int main(int /*argc*/, char** /*argv*/) {
    core::Logger::instance().setLevel(core::LogLevel::Debug);
    DLOG_INFO("boot", "Dionite — Shattered Wilds desktop bootstrap (3D)");

    auto renderer = std::make_unique<render::NullRenderer>();
    renderer->init(nullptr, 1920, 1080);

    // 1) Generate a dungeon and build its 3D mesh
    world::DungeonGenerator dgen(0xDEADBEEFCAFEULL);
    auto dungeon = dgen.generate(12, 1);
    world::DungeonMeshBuilder builder;
    auto mesh = builder.build(dungeon, "verdant_wilds");
    DLOG_INFO("dungeon", "Rooms=", dungeon.rooms.size(),
              " floors=", mesh.floors.size(), " walls=", mesh.walls.size(),
              " lights=", mesh.lights.size());

    // 2) Spawn player + camera
    player::PlayerController player;
    player.setPosition(mesh.spawnWorld);
    player::GameCamera camera;

    // 3) Click-to-move: pick the boss pad as destination, compute A* path
    math::Vec3 goal{};
    for (auto& p : mesh.props) if (p.kind == "boss_pad") { goal = p.worldPos; break; }
    world::NavGrid nav(dungeon);
    auto path = nav.findPath(mesh.spawnWorld, goal);
    DLOG_INFO("nav", "Path length=", path.size(), " waypoints");

    auto weapon = combat::WeaponSystem::fromTemplate("rifle");
    combat::ProjectilePool projectiles;
    combat::WeaponSystem weaponSys;

    loot::LootRoller roller(0x12345ULL);
    auto drop = roller.rollWeapon(1, 0.f);
    DLOG_INFO("loot", "Sample drop: ", drop.name, " (", loot::rarityName(drop.rarity),
              ", sockets=", drop.socketCount, ")");

    game::InfinitySpire spire(0xABCDEFULL);
    auto run = spire.startRun(1);
    DLOG_INFO("spire", "Spire floor=", run.floor, " biome=", run.biome,
              " affixes=", run.currentAffixes.size());

    core::FrameTimer timer;
    size_t pathIdx = 0;
    int frames = 0;
    while (frames < 240) {
        float dt = timer.tick();
        if (dt > 0.05f) dt = 0.05f;

        // Follow A* path
        player::InputState in{};
        in.cameraYawDeg = camera.config().yawDeg;
        if (!path.empty() && pathIdx < path.size()) {
            in.clickToMove = true;
            in.hasMouseWorld = true;
            in.mouseWorld = path[pathIdx];
            if ((player.position() - path[pathIdx]).length() < 0.4f) ++pathIdx;
        }
        player.update(dt, in);
        camera.follow(player.position(), dt);
        weaponSys.update(weapon, dt);
        projectiles.update(dt);

        renderer->beginFrame();
        render::CameraUniforms cu;
        cu.view = camera.viewMatrix();
        cu.proj = camera.projMatrix(1920.f / 1080.f);
        cu.cameraPos = camera.position();
        cu.sun.direction = {-0.4f, -1.f, -0.3f};
        cu.sun.color = {1.f, 0.95f, 0.85f};
        cu.sun.intensity = 1.2f;
        renderer->setCamera(cu);
        for (auto& l : mesh.lights) {
            render::PointLight pl{ l.worldPos, l.radius, {l.r, l.g, l.b}, l.intensity };
            renderer->submitLight(pl);
        }
        renderer->endFrame();
        frames++;
    }
    renderer->shutdown();
    DLOG_INFO("boot", "Player ended at (", player.position().x, ",", player.position().z, ")");
    DLOG_INFO("boot", "Clean shutdown");
    return 0;
}
