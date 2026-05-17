// ============================================================================
// Dionite — Progression: Soul Fragments awarded by killing campaign bosses
// ============================================================================
#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace dionite::progression {

struct SoulFragment {
    std::string id;
    std::string bossId;
    std::string name;
    std::string unlocks; // ability id, biome id, or skill point
    int countToFuse = 3; // collect 3 to fuse into permanent buff
};

class SoulFragmentLedger {
public:
    SoulFragmentLedger() { seed(); }

    void award(const std::string& bossId) {
        auto it = byBoss_.find(bossId);
        if (it != byBoss_.end()) collected_[it->second.id]++;
    }

    int held(const std::string& fragmentId) const {
        auto it = collected_.find(fragmentId);
        return it == collected_.end() ? 0 : it->second;
    }

    bool canFuse(const std::string& fragmentId) const {
        auto it = byId_.find(fragmentId);
        if (it == byId_.end()) return false;
        return held(fragmentId) >= it->second.countToFuse;
    }

private:
    void seed() {
        std::vector<SoulFragment> seeds = {
            {"frag_verdant", "verdant_warden", "Verdant Fragment", "biome_verdant", 1},
            {"frag_ash",     "ash_tyrant",      "Ash Fragment",     "biome_ashen",   1},
            {"frag_frost",   "frost_lord",      "Frost Fragment",   "biome_frozen",  1},
            {"frag_drowned", "drowned_king",    "Drowned Fragment", "biome_sunken",  1},
            {"frag_sky",     "sky_sovereign",   "Sky Fragment",     "soul_avatar",   1},
        };
        for (auto& f : seeds) { byId_[f.id] = f; byBoss_[f.bossId] = f; }
    }
    std::unordered_map<std::string, SoulFragment> byId_;
    std::unordered_map<std::string, SoulFragment> byBoss_;
    std::unordered_map<std::string, int> collected_;
};

} // namespace dionite::progression
