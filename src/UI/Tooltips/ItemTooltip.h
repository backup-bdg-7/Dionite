// ============================================================================
// Dionite — UI: Item tooltip data
// ============================================================================
#pragma once
#include "Loot/Items/ItemBase.h"
#include "Loot/RaritySystem/Rarity.h"
#include <string>
#include <sstream>

namespace dionite::ui {

inline std::string buildItemTooltip(const loot::Item& it) {
    std::ostringstream s;
    s << it.name << "\n";
    s << "ilvl " << it.ilvl << " · " << loot::rarityName(it.rarity) << "\n";
    s << "Damage: " << (int)it.damage << "\n";
    for (auto& a : it.affixes) s << "• " << a.name << " (" << a.stat << " +" << a.amount << ")\n";
    if (!it.sockets.empty()) {
        s << "Sockets: ";
        for (auto& sid : it.sockets) s << (sid.empty() ? "□ " : "■ ");
        s << "\n";
    }
    return s.str();
}

} // namespace dionite::ui
