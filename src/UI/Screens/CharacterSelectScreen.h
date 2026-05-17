// ============================================================================
// Dionite — UI: Character Selection screen (roster of saved characters)
// ============================================================================
#pragma once
#include "UIScreen.h"
#include "Progression/Classes/ClassRegistry.h"
#include <string>
#include <vector>
#include <functional>

namespace dionite::ui {

struct CharacterSummary {
    std::string id;
    std::string name;
    progression::ClassId classId;
    int level;
    int paragonLevel;
    std::string biome;
    int spireBest;
    int playSeconds;
    std::string lastSeenISO;
};

class CharacterSelectScreen : public UIScreen {
public:
    std::vector<CharacterSummary> characters;
    int  selectedIdx = 0;
    std::function<void(const std::string& charId)> onPlay;
    std::function<void()> onCreate;
    std::function<void(const std::string& charId)> onDelete;

    bool blocksGameplay() const override { return true; }
    std::string id() const override { return "char_select"; }
    void update(float) override {}
    void render() override {}
};

} // namespace dionite::ui
