// ============================================================================
// Dionite — Desktop entry point (links Game lib, opens GLFW window, runs loop)
// ============================================================================
#include "Core/Logging/Logger.h"
#include "Core/Utils/Time.h"
#include "Player/Controller/PlayerController.h"
#include "Player/Camera/GameCamera.h"
#include "World/DungeonGenerator/DungeonGenerator.h"
#include "Combat/Weapons/WeaponBase.h"
#include "Combat/Projectiles/Projectile.h"
#include "Combat/AI/EnemyAI.h"
#include "Loot/Items/ItemBase.h"
#include "GameSystems/InfinitySpire.h"
#include "Rendering/Renderer.h"
#include <memory>
#include <iostream>

using namespace dionite;

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    core::Logger::instance().setLevel(core::LogLevel::Debug);
    DLOG_INFO("boot", "Dionite — Shattered Wilds desktop bootstrap");

    // -- Platform window (GLFW) initialization would go here.
    // -- For now we run the game loop headless to validate systems.

    auto renderer = std::make_unique<render::NullRenderer>();
    renderer->init(nullptr, 1920, 1080);

    player::PlayerController player;
    player::GameCamera camera;
    world::DungeonGenerator dgen(0xDEADBEEFCAFE);
    auto dungeon = dgen.generate(12, 1);
    DLOG_INFO("dungeon", "Generated dungeon: ", dungeon.rooms.size(), " rooms, ", dungeon.affixes.size(), " affixes");

    auto weapon = combat::WeaponSystem::fromTemplate("rifle");
    combat::ProjectilePool projectiles;
    combat::WeaponSystem weaponSys;

    loot::LootRoller roller(0x12345);
    auto sampleDrop = roller.rollWeapon(1, 0.f);
    DLOG_INFO("loot", "Sample drop: ", sampleDrop.name, " (", loot::rarityName(sampleDrop.rarity), ")");

    game::InfinitySpire spire(0xABCDEF);
    auto run = spire.startRun(1);
    DLOG_INFO("spire", "Spire floor 1 biome=", run.biome, " affixes=", run.currentAffixes.size());

    core::FrameTimer timer;
    int frames = 0;
    while (frames < 240) { // run a couple seconds at 120fps simulated
        float dt = timer.tick();
        if (dt > 0.05f) dt = 0.05f;

        player::InputState in{};
        in.move = {0, 0};
        in.aim = {1, 0};
        player.update(dt, in);
        camera.follow(player.position(), dt);
        weaponSys.update(weapon, dt);
        projectiles.update(dt);

        renderer->beginFrame();
        renderer->endFrame();
        frames++;
    }
    renderer->shutdown();
    DLOG_INFO("boot", "Clean shutdown");
    return 0;
}
