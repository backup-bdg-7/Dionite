// ============================================================================
// Dionite — World: Biome definitions (5 biomes)
// ============================================================================
#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace dionite::world {

enum class BiomeId {
    VerdantWilds,
    AshenWastes,
    FrozenSpire,
    SunkenCrypts,
    SkyCitadel
};

struct BiomeDef {
    BiomeId id;
    std::string name;
    std::string fogColor;     // hex
    std::string ambientLight; // hex
    std::string musicId;
    float baseDifficulty;
    std::vector<std::string> enemyPool;
    std::vector<std::string> lootBoost; // rarities or loot tables
};

class BiomeRegistry {
public:
    static BiomeRegistry& instance() { static BiomeRegistry r; r.init(); return r; }

    const BiomeDef& get(BiomeId id) const { return defs_.at(id); }

    const std::unordered_map<BiomeId, BiomeDef>& all() const { return defs_; }

private:
    void init() {
        if (!defs_.empty()) return;
        defs_[BiomeId::VerdantWilds] = {
            BiomeId::VerdantWilds, "Verdant Wilds", "#0a1a10", "#34d399", "forest",
            1.0f, {"shade", "wraith", "archer"}, {"common", "magic"}
        };
        defs_[BiomeId::AshenWastes] = {
            BiomeId::AshenWastes, "Ashen Wastes", "#1a0d05", "#f97316", "embers",
            1.5f, {"brute", "wraith", "shade"}, {"magic", "rare"}
        };
        defs_[BiomeId::FrozenSpire] = {
            BiomeId::FrozenSpire, "Frozen Spire", "#08121f", "#38bdf8", "wind",
            2.0f, {"ironclad", "archer"}, {"rare", "epic"}
        };
        defs_[BiomeId::SunkenCrypts] = {
            BiomeId::SunkenCrypts, "Sunken Crypts", "#061d22", "#2dd4bf", "abyss",
            2.5f, {"wraith", "ironclad", "brute"}, {"epic", "legendary"}
        };
        defs_[BiomeId::SkyCitadel] = {
            BiomeId::SkyCitadel, "Sky Citadel", "#1a1208", "#fde047", "thunder",
            3.5f, {"ironclad", "brute", "archer"}, {"legendary", "mythic"}
        };
    }
    std::unordered_map<BiomeId, BiomeDef> defs_;
};

} // namespace dionite::world
