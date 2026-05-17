// ============================================================================
// Dionite — UI: HUD (orbs, ability hotbar, minimap, buffs/debuffs)
// ============================================================================
#pragma once
#include "UIScreen.h"
#include <string>

namespace dionite::ui {

struct HUDState {
    float healthPct = 1.f;
    float manaPct = 1.f;
    int   level = 1;
    int   xp = 0;
    int   xpForNext = 50;
    int   gold = 0;
    int   spireFloor = 0;
    std::string biomeName = "Verdant Wilds";
    std::string activeRuneWord;
};

class HUD : public UIScreen {
public:
    void update(float /*dt*/) override {}
    void render() override { /* rendering handled by platform renderer */ }
    std::string id() const override { return "hud"; }
    HUDState state;
};

} // namespace dionite::ui
