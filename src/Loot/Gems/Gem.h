// ============================================================================
// Dionite — Loot: Gems (6 types) + upgrade tiers
// ============================================================================
#pragma once
#include <string>
#include <vector>

namespace dionite::loot {

struct Gem {
    std::string id;
    std::string name;
    std::string colorHex;
    std::string stat;
    float       amount;
    int         tier = 1; // 1..5 upgrade tiers
};

class GemLibrary {
public:
    static GemLibrary& instance() { static GemLibrary g; g.init(); return g; }
    const std::vector<Gem>& all() const { return gems_; }
    const Gem* find(const std::string& id) const {
        for (auto& g : gems_) if (g.id == id) return &g;
        return nullptr;
    }

    // Returns the result of fusing 3 gems of the same id+tier into one of tier+1
    static Gem fuse(const Gem& a) {
        Gem r = a;
        r.tier = a.tier + 1;
        r.amount *= 2.2f;
        return r;
    }

private:
    void init() {
        if (!gems_.empty()) return;
        gems_ = {
            {"gem_ruby",     "Ruby",     "#dc2626", "dmgFlat",      4, 1},
            {"gem_sapphire", "Sapphire", "#3b82f6", "manaMax",     15, 1},
            {"gem_emerald",  "Emerald",  "#16a34a", "lifesteal",    0.04f, 1},
            {"gem_topaz",    "Topaz",    "#fbbf24", "fireRatePct",  0.10f, 1},
            {"gem_amethyst", "Amethyst", "#a855f7", "critChance",   0.06f, 1},
            {"gem_diamond",  "Diamond",  "#e0f2fe", "allRes",      12,    1},
        };
    }
    std::vector<Gem> gems_;
};

} // namespace dionite::loot
