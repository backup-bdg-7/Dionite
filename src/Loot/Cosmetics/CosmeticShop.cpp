#include "CosmeticShop.h"
#include <algorithm>

namespace dionite::cosmetics {

void CosmeticShop::refreshRotation(long long now) {
    // Server-driven; client just culls expired listings.
    currentRotation.erase(std::remove_if(currentRotation.begin(), currentRotation.end(),
        [&](const ShopListing& l) { return l.featuredUntilEpoch > 0 && l.featuredUntilEpoch < now; }),
        currentRotation.end());
}

} // namespace dionite::cosmetics
