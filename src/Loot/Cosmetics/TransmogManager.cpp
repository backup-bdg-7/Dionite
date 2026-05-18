#include "TransmogManager.h"

namespace dionite::cosmetics {

void TransmogManager::unlockAppearance(const std::string& id) { unlockedIds.insert(id); }
bool TransmogManager::isUnlocked(const std::string& id) const { return unlockedIds.count(id) > 0; }

bool TransmogManager::applyTransmog(TransmogLoadout& l, CosmeticSlot s, const std::string& id) {
    if (!isUnlocked(id)) return false;
    l.appearance[s] = id;
    return true;
}

bool TransmogManager::applyDye(TransmogLoadout& l, CosmeticSlot s, int ch, const DyeChannel& d) {
    if (ch < 0 || ch >= 4) return false;
    auto& arr = l.dyes[s];
    arr[ch] = d;
    return true;
}

void TransmogManager::clearTransmog(TransmogLoadout& l, CosmeticSlot s) {
    l.appearance.erase(s);
    l.dyes.erase(s);
}

} // namespace dionite::cosmetics
