// ============================================================================
// Dionite — Loot: Runes (10 types, Diablo-style) + ordered rune word detection
// ============================================================================
#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace dionite::loot {

struct Rune {
    std::string id;
    std::string name;
    std::string symbol;
    std::string colorHex;
    std::string stat;
    float amount;
};

class RuneLibrary {
public:
    static RuneLibrary& instance() { static RuneLibrary r; r.init(); return r; }
    const std::vector<Rune>& all() const { return runes_; }
    const Rune* find(const std::string& id) const {
        for (auto& r : runes_) if (r.id == id) return &r;
        return nullptr;
    }
private:
    void init() {
        if (!runes_.empty()) return;
        runes_ = {
            {"rune_el",   "El",   "ᛖ", "#fbbf24", "critChance",   0.03f},
            {"rune_eth",  "Eth",  "ᛇ", "#a855f7", "enemyDefPct", -0.15f},
            {"rune_tir",  "Tir",  "ᛏ", "#3b82f6", "manaOnKill",   4.f},
            {"rune_nef",  "Nef",  "ᚾ", "#94a3b8", "knockback",    1.f},
            {"rune_ral",  "Ral",  "ᚱ", "#f97316", "firePct",      0.15f},
            {"rune_ort",  "Ort",  "ᛟ", "#38bdf8", "shockPct",     0.15f},
            {"rune_thul", "Thul", "ᚦ", "#7dd3fc", "coldPct",      0.15f},
            {"rune_amn",  "Amn",  "ᚨ", "#dc2626", "lifesteal",    0.06f},
            {"rune_sol",  "Sol",  "ᛋ", "#fde047", "dmgPct",       0.20f},
            {"rune_shael","Shael","ᛊ", "#f59e0b", "fireRatePct",  0.20f},
        };
    }
    std::vector<Rune> runes_;
};

} // namespace dionite::loot
