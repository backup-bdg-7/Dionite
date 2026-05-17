// ============================================================================
// Dionite — Game Systems: Economy (gold, vendor pricing, exchange rates)
// ============================================================================
#pragma once
#include "Loot/Items/ItemBase.h"
#include "Loot/RaritySystem/Rarity.h"

namespace dionite::game {

class Economy {
public:
    static int sellPrice(const loot::Item& it) {
        return (int)(20 + it.damage * 2 * (1 + (int)it.rarity));
    }
    static int buyPrice(const loot::Item& it) {
        return (int)(80 + it.damage * 5 * (1 + (int)it.rarity));
    }
    static int gemCost(int tier) { return 100 * tier; }
    static int runeCost(int rarity) { return 250 * (rarity + 1); }
    static int spireEntryFee(int floor) { return floor * 50; }
};

} // namespace dionite::game
