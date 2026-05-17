// ============================================================================
// Dionite — C++ ↔ Swift bridge implementation (3D Diablo-style core).
// extern "C" functions called by DioniteBridge.swift via @_silgen_name.
// ============================================================================
#include "Core/Logging/Logger.h"
#include "Player/Controller/PlayerController.h"
#include "Player/Camera/GameCamera.h"
#include "World/DungeonGenerator/DungeonGenerator.h"
#include "World/DungeonGenerator/DungeonMeshBuilder.h"
#include "World/DungeonGenerator/NavGrid.h"
#include "GameSystems/InfinitySpire.h"
#include <memory>

using namespace dionite;

namespace {
    struct GameContext {
        player::PlayerController player;
        player::GameCamera camera;
        player::InputState input;
        std::unique_ptr<world::DungeonGenerator> dgen;
        world::Dungeon dungeon;
        world::DungeonMesh mesh;
        std::unique_ptr<world::NavGrid> nav;
        bool paused = false;
        int width = 1920, height = 1080;
    };
    std::unique_ptr<GameContext> g_ctx;
}

extern "C" {

void dionite_boot() {
    core::Logger::instance().setLevel(core::LogLevel::Info);
    DLOG_INFO("bridge", "dionite_boot (3D)");
    g_ctx = std::make_unique<GameContext>();
    g_ctx->dgen   = std::make_unique<world::DungeonGenerator>(0xC0FFEEBEEFULL);
    g_ctx->dungeon = g_ctx->dgen->generate(12, 1);
    g_ctx->mesh   = world::DungeonMeshBuilder().build(g_ctx->dungeon, "verdant_wilds");
    g_ctx->nav    = std::make_unique<world::NavGrid>(g_ctx->dungeon);
    g_ctx->player.setPosition(g_ctx->mesh.spawnWorld);
}

void dionite_shutdown() { DLOG_INFO("bridge", "dionite_shutdown"); g_ctx.reset(); }

void dionite_tick(float dt) {
    if (!g_ctx || g_ctx->paused) return;
    g_ctx->input.cameraYawDeg = g_ctx->camera.config().yawDeg;
    g_ctx->player.update(dt, g_ctx->input);
    g_ctx->camera.follow(g_ctx->player.position(), dt);
    g_ctx->input.firePressed = false;
    g_ctx->input.clickToMove = false;
}

void dionite_resize(int32_t w, int32_t h) {
    if (!g_ctx) return;
    g_ctx->width = w; g_ctx->height = h;
}

void dionite_set_move(float x, float y) { if (g_ctx) g_ctx->input.move = { x, y }; }
void dionite_set_aim(float x, float y)  { if (g_ctx) g_ctx->input.aim  = { x, y }; }
void dionite_set_fire(bool b)           { if (g_ctx) { g_ctx->input.firePressed = b && !g_ctx->input.fire; g_ctx->input.fire = b; } }

// Click-to-move: the platform layer projects screen-space NDC into a world-space
// ground point using GameCamera::groundFromScreen and passes the world position here.
void dionite_click_to_move(float wx, float wy, float wz) {
    if (!g_ctx) return;
    g_ctx->input.clickToMove = true;
    g_ctx->input.hasMouseWorld = true;
    g_ctx->input.mouseWorld = { wx, wy, wz };
}

void dionite_set_ability(int32_t idx, bool b) {
    if (!g_ctx) return;
    if (idx == 0) g_ctx->input.ability1 = b;
    else g_ctx->input.ability2 = b;
}
void dionite_set_dash(bool b) { if (g_ctx) g_ctx->input.dashPressed = b; }
void dionite_camera_pan(float dx) { if (g_ctx) g_ctx->camera.applyUserYaw(dx); }
void dionite_pause()  { if (g_ctx) g_ctx->paused = true; }
void dionite_resume() { if (g_ctx) g_ctx->paused = false; }

}
