// ============================================================================
// Dionite — UI: Loading screen with biome lore tips and progress bar.
// ============================================================================
#pragma once
#include "UIScreen.h"
#include <string>
#include <vector>
#include <functional>

namespace dionite::ui {

class LoadingScreen : public UIScreen {
public:
    float progress = 0.f;       // 0..1
    std::string biomeId;        // active biome for art/lore
    std::string tipText;        // randomized tip
    std::function<void()> onComplete;

    static std::vector<std::string> tipsFor(const std::string& biome) {
        if (biome == "verdant_wilds") return {
            "Wraiths exhale soulfire. Dodge sideways — never backward.",
            "Echoing rune-words activate only in the exact socket order.",
            "Sell duplicate gems to fund rune fusing.",
        };
        if (biome == "ashen_wastes") return {
            "Tyrant of Ashes channels meteors at low HP; break line of sight.",
            "Ember Staff stacks 5 burns per target. Then it explodes.",
            "Vampiric affixes pair brutally with Necromancer's Blood Lance.",
        };
        if (biome == "frozen_spire") return {
            "Frostlord telegraphs his slam — you have 0.8 s to roll.",
            "Cold + Shock = Brittle status; +30% crit chance against affected foes.",
            "Sapphires in a 4-socket weapon are worth more than gold.",
        };
        if (biome == "sunken_crypts") return {
            "Drowned King summons two doppelgangers at 50% HP — only the original drops loot.",
            "Resistance gear matters here. Diamonds in every socket.",
            "Bone Storm reflects projectiles. Don't shoot it.",
        };
        if (biome == "sky_citadel") return {
            "Sky Sovereign's whirlwind ignores armor — use Diamond Skin or Serenity.",
            "Mythic drops here can roll 6 sockets. Save your favorite Rune Word.",
            "Falling Sword + Earthquake combo can one-shot bosses below 30%.",
        };
        return { "Press Tab to open Inventory at any time.",
                 "Use 'F' to interact with NPCs and chests.",
                 "Two-finger drag to pan the camera left or right." };
    }

    bool blocksGameplay() const override { return true; }
    std::string id() const override { return "loading"; }
    void update(float dt) override {
        progress = std::min(1.f, progress + dt * 0.6f);
        if (progress >= 1.f && onComplete) { onComplete(); onComplete = nullptr; }
    }
    void render() override {}
};

} // namespace dionite::ui
