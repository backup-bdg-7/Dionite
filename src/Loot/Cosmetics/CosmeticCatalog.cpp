#include "CosmeticItem.h"

namespace dionite::cosmetics {

void CosmeticCatalog::add(const CosmeticItem& c) { items[c.id] = c; }

const CosmeticItem* CosmeticCatalog::find(const std::string& id) const {
    auto it = items.find(id);
    return it == items.end() ? nullptr : &it->second;
}

std::vector<const CosmeticItem*> CosmeticCatalog::bySlot(CosmeticSlot s) const {
    std::vector<const CosmeticItem*> out;
    for (auto& [k, v] : items) if (v.slot == s) out.push_back(&v);
    return out;
}

std::vector<const CosmeticItem*> CosmeticCatalog::bySeason(int n) const {
    std::vector<const CosmeticItem*> out;
    for (auto& [k, v] : items) if (v.seasonNumber == n) out.push_back(&v);
    return out;
}

} // namespace dionite::cosmetics
