// CosmeticShop.h — Storefront definition. Rotating featured items, evergreen catalog,
// season pass tracks. Inventory is fetched from `/api/shop/listing` and validated against
// the player's entitlement (purchases + earned rewards) on the server.
#pragma once
#include "CosmeticItem.h"
#include <string>
#include <vector>

namespace dionite::cosmetics {

struct ShopListing {
    std::string cosmeticId;
    int         priceCoins = 0;
    int         priceShards = 0;
    int         discountPercent = 0;
    bool        featured = false;
    long long   featuredUntilEpoch = 0;
    int         bundleSize = 1;            // single or bundle (e.g., 3 helms)
    std::vector<std::string> bundleIds;    // empty if single
};

struct SeasonPassTier {
    int tier;
    bool premium = false;        // gated behind premium pass purchase
    std::vector<std::string> cosmeticRewards;
    int xpRequired = 1000;
};

struct SeasonPass {
    int season = 1;
    std::string name = "Season of the Shattered Wilds";
    long long startEpoch = 0;
    long long endEpoch   = 0;
    int       freeTrackCoins = 500;     // soft-currency reward
    int       premiumTrackCoins = 1500;
    std::vector<SeasonPassTier> tiers;
};

class CosmeticShop {
public:
    std::vector<ShopListing> currentRotation;
    SeasonPass               activePass;
    void refreshRotation(long long nowEpoch);
};

} // namespace dionite::cosmetics
