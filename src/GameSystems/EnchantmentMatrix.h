// ============================================================================
// Dionite — Game Systems: Enchantment Matrix (grid-based slot synergies)
// ============================================================================
#pragma once
#include "Loot/Items/ItemBase.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace dionite::game {

struct EnchantmentSlot {
    std::string id;
    std::string name;
    std::string stat;
    float amount;
    bool used = false;
};

// 5x5 grid; certain rows or columns trigger synergies.
class EnchantmentMatrix {
public:
    static constexpr int W = 5, H = 5;

    void resetForItem(const loot::Item& it) {
        slots_.assign(W * H, {});
        // Pre-populate based on affixes
        int idx = 0;
        for (auto& a : it.affixes) {
            if (idx >= W * H) break;
            slots_[idx] = { a.id, a.name, a.stat, a.amount, true };
            idx += 6; // spread out for synergies
        }
    }

    bool place(int x, int y, const EnchantmentSlot& s) {
        if (x < 0 || y < 0 || x >= W || y >= H) return false;
        auto& cur = slots_[y * W + x];
        if (cur.used) return false;
        cur = s; cur.used = true;
        return true;
    }

    // Returns the aggregated synergy bonuses (full row or column of same-stat = +25%).
    std::unordered_map<std::string, float> aggregate() const {
        std::unordered_map<std::string, float> out;
        // Base
        for (auto& s : slots_) if (s.used) out[s.stat] += s.amount;
        // Row synergy
        for (int y = 0; y < H; ++y) {
            std::string stat = slots_[y * W].stat;
            bool full = !stat.empty();
            for (int x = 0; x < W; ++x) if (slots_[y*W+x].stat != stat || !slots_[y*W+x].used) { full=false; break; }
            if (full) out[stat] *= 1.25f;
        }
        // Column synergy
        for (int x = 0; x < W; ++x) {
            std::string stat = slots_[x].stat;
            bool full = !stat.empty();
            for (int y = 0; y < H; ++y) if (slots_[y*W+x].stat != stat || !slots_[y*W+x].used) { full=false; break; }
            if (full) out[stat] *= 1.25f;
        }
        return out;
    }

    const std::vector<EnchantmentSlot>& slots() const { return slots_; }

private:
    std::vector<EnchantmentSlot> slots_ = std::vector<EnchantmentSlot>(W * H);
};

} // namespace dionite::game
