// ============================================================================
// Dionite — Progression: XP / Level system
// ============================================================================
#pragma once
#include <cmath>

namespace dionite::progression {

class LevelSystem {
public:
    static int xpForLevel(int lvl) { return (int)(50 + lvl * lvl * 18); }

    bool addXp(int amount, int& level, int& xp, int& skillPoints) {
        xp += amount;
        bool dingedAny = false;
        while (xp >= xpForLevel(level)) {
            xp -= xpForLevel(level);
            level += 1;
            skillPoints += 1;
            dingedAny = true;
            if (level > 100) { level = 100; xp = 0; break; }
        }
        return dingedAny;
    }
};

} // namespace dionite::progression
