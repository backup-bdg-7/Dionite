// ============================================================================
// Dionite — Player: Customization (appearance, sigils, color tint)
// ============================================================================
#pragma once
#include <string>

namespace dionite::player {

struct Customization {
    std::string bodyMesh = "default_body";
    std::string headMesh = "default_head";
    std::string tintHex = "#A5854C";    // armor accent color
    std::string sigilId = "sigil_doom"; // emblem on cape
    int cosmeticLevel = 0;
};

} // namespace dionite::player
