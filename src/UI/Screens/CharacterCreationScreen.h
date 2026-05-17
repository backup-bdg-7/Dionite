// ============================================================================
// Dionite — UI: Character Creation screen (class pick + cosmetic preview)
// ============================================================================
#pragma once
#include "UIScreen.h"
#include "Progression/Classes/ClassRegistry.h"
#include "Player/Customization/Customization.h"
#include <string>
#include <functional>

namespace dionite::ui {

struct CharacterCreationData {
    std::string name;
    progression::ClassId classId = progression::ClassId::Crusader;
    bool hardcore = false;
    player::Customization look;
};

class CharacterCreationScreen : public UIScreen {
public:
    CharacterCreationData draft;
    std::function<void(const CharacterCreationData&)> onConfirm;
    std::function<void()> onBack;

    bool blocksGameplay() const override { return true; }
    std::string id() const override { return "char_create"; }
    void update(float) override {}
    void render() override {}

    bool validate() const {
        if (draft.name.size() < 2 || draft.name.size() > 20) return false;
        // ASCII printable only
        for (char c : draft.name) if (c < 32 || c == 127) return false;
        return true;
    }
};

} // namespace dionite::ui
