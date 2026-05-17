// ============================================================================
// Dionite — Loot: base item definitions and rarity rolling
// ============================================================================
#pragma once
#include "Core/Math/Random.h"
#include <string>
#include <vector>

namespace dionite::loot {

enum class Rarity { Common, Magic, Rare, Epic, Legendary, Mythic };

inline const char* rarityName(Rarity r) {
    switch (r) {
        case Rarity::Common: return "Common";
        case Rarity::Magic: return "Magic";
        case Rarity::Rare: return "Rare";
        case Rarity::Epic: return "Epic";
        case Rarity::Legendary: return "Legendary";
        case Rarity::Mythic: return "Mythic";
    }
    return "?";
}

inline int socketsFor(Rarity r) {
    static const int arr[] = {0, 1, 2, 3, 4, 6};
    return arr[(int)r];
}

inline int affixesFor(Rarity r) {
    static const int arr[] = {0, 1, 2, 3, 4, 5};
    return arr[(int)r];
}

struct Affix {
    std::string id;
    std::string name;
    std::string stat;
    float amount;
};

struct Item {
    std::string uid;
    std::string baseId;
    std::string name;
    std::string kind; // "weapon", "armor", "trinket"
    Rarity rarity;
    float damage = 0.f;
    int ilvl = 1;
    std::vector<Affix> affixes;
    std::vector<std::string> sockets; // ids of socketed gems/runes, in order; empty string for empty
    int socketCount = 0;
};

class LootRoller {
public:
    LootRoller(uint64_t seed) : rng_(seed) {}

    Rarity rollRarity(float luck = 0.f) {
        float weights[6] = {60.f, 25.f, 10.f, 4.f, 0.9f + luck * 1.5f, 0.1f + luck * 0.5f};
        float sum = 0; for (float w : weights) sum += w;
        float r = rng_.rangeF(0, sum);
        for (int i = 0; i < 6; ++i) { if ((r -= weights[i]) <= 0) return (Rarity)i; }
        return Rarity::Common;
    }

    Item rollWeapon(int level, float luck = 0.f);

private:
    math::Random rng_;
};

} // namespace dionite::loot
