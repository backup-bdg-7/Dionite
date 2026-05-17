// ============================================================================
// Dionite — C++ ↔ Swift bridge implementation
// These extern "C" functions are called by DioniteBridge.swift via @_silgen_name.
// ============================================================================
#include "Core/Logging/Logger.h"
#include "Player/Controller/PlayerController.h"
#include "Player/Camera/GameCamera.h"
#include "World/DungeonGenerator/DungeonGenerator.h"
#include "GameSystems/InfinitySpire.h"
#include <memory>

using namespace dionite;

namespace {
    struct GameContext {
        player::PlayerController player;
        player::GameCamera camera;
        player::InputState input;
        std::unique_ptr<world::DungeonGenerator> dgen;
        bool paused = false;
        int width = 0, height = 0;
    };
    std::unique_ptr<GameContext> g_ctx;
}

extern "C" {

void dionite_boot() {
    core::Logger::instance().setLevel(core::LogLevel::Info);
    DLOG_INFO("bridge", "dionite_boot");
    g_ctx = std::make_unique<GameContext>();
    g_ctx->dgen = std::make_unique<world::DungeonGenerator>(0xC0FFEEBEEF);
}

void dionite_shutdown() {
    DLOG_INFO("bridge", "dionite_shutdown");
    g_ctx.reset();
}

void dionite_tick(float dt) {
    if (!g_ctx || g_ctx->paused) return;
    g_ctx->player.update(dt, g_ctx->input);
    g_ctx->camera.follow(g_ctx->player.position(), dt);
}

void dionite_resize(int32_t w, int32_t h) {
    if (!g_ctx) return;
    g_ctx->width = w; g_ctx->height = h;
}

void dionite_set_move(float x, float y) { if (g_ctx) g_ctx->input.move = { x, y }; }
void dionite_set_aim(float x, float y)  { if (g_ctx) g_ctx->input.aim  = { x, y }; }
void dionite_set_fire(bool b)           { if (g_ctx) { g_ctx->input.firePressed = b && !g_ctx->input.fire; g_ctx->input.fire = b; } }
void dionite_set_ability(int32_t idx, bool b) {
    if (!g_ctx) return;
    if (idx == 0) g_ctx->input.ability1 = b;
    else g_ctx->input.ability2 = b;
}
void dionite_set_dash(bool b) { if (g_ctx) g_ctx->input.dashPressed = b; }
void dionite_pause()  { if (g_ctx) g_ctx->paused = true; }
void dionite_resume() { if (g_ctx) g_ctx->paused = false; }

}
