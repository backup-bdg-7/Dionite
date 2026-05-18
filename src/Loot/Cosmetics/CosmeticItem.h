// CosmeticItem.h — Visual-only equipment skins (transmog, dyes, weapon wraps, mounts,
// emotes, pet companions, portrait frames). They never affect stats — only appearance.
// Pricing tier mirrors Diablo IV cosmetic shop (Basic / Premium / Elite).
#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace dionite::cosmetics {

enum class CosmeticSlot {
    Helm, Chest, Gloves, Pants, Boots, Cloak,
    WeaponMain, WeaponOff, Shield,
    Mount, Pet, Emote, PortraitFrame, Banner, TrailFX
};

enum class Rarity { Common, Uncommon, Rare, Epic, Legendary, Mythic };
enum class PriceTier { Free, Bronze, Silver, Gold, Platinum, Mythic };

struct CosmeticItem {
    std::string id;                     // e.g., "helm_ravenwarden_01"
    std::string name;                   // "Ravenwarden's Visor"
    std::string flavor;                 // bardic flavor line
    CosmeticSlot slot;
    Rarity      rarity   = Rarity::Common;
    PriceTier   tier     = PriceTier::Bronze;
    int         priceShards = 0;        // soft currency
    int         priceCoins  = 0;        // premium currency (real money)
    std::string meshAsset;              // "meshes/cosmetics/helm_ravenwarden.gltf"
    std::string albedo, normal, roughness, metallic, emissive;
    std::string vfxAttachment;          // optional particle attachment id
    std::vector<std::string> dyeChannels;
    bool        animated = false;       // e.g., flowing cloth, glowing runes
    int         seasonNumber = 0;       // 0 == evergreen
    std::string classRestriction;       // "" or "Sorcerer" / "Crusader" etc.
    int         requiredLevel = 1;
    bool        eventOnly = false;      // limited-time event reward
};

class CosmeticCatalog {
public:
    void add(const CosmeticItem& c);
    const CosmeticItem* find(const std::string& id) const;
    std::vector<const CosmeticItem*> bySlot(CosmeticSlot s) const;
    std::vector<const CosmeticItem*> bySeason(int n) const;
    size_t size() const { return items.size(); }
private:
    std::unordered_map<std::string, CosmeticItem> items;
};

} // namespace dionite::cosmetics
