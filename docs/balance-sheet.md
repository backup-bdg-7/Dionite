# Dionite — Balance Sheet

> Master tuning document. All numbers are starting points — playtest and tweak.

## Rarity Weights

| Rarity     | Drop Weight | Sockets | Affixes | Color    |
| ---------- | ----------: | ------: | ------: | -------- |
| Common     |        60.0 |       0 |       0 | `#B0B0B0`|
| Magic      |        25.0 |       1 |       1 | `#3B82F6`|
| Rare       |        10.0 |       2 |       2 | `#FBBF24`|
| Epic       |         4.0 |       3 |       3 | `#A855F7`|
| Legendary  |         0.9 |       4 |       4 | `#F97316`|
| Mythic     |         0.1 |       6 |       5 | `#E11D48`|

Luck (player stat) adds to Legendary/Mythic weights:
`legendary += luck × 1.5`, `mythic += luck × 0.5`.

## XP Curve

`xpForLevel(L) = 50 + L² × 18`

| Level | XP req | Total to reach |
|------:|-------:|---------------:|
|     1 |     68 |             68 |
|     5 |    500 |          1,210 |
|    10 |  1,850 |          6,470 |
|    25 | 11,300 |         92,475 |
|    50 | 45,050 |        765,500 |
|   100 |180,050 |      6,066,050 |

## Boss HP / DMG Scaling

Boss HP at floor F = `baseHP × (1 + F × 0.18)`
Boss DMG at floor F = `baseDMG × (1 + F × 0.12)`

| Boss            | Base HP | Base DMG |
|-----------------|--------:|---------:|
| Verdant Warden  |     800 |       28 |
| Tyrant of Ashes |   1,400 |       40 |
| Frostlord       |   2,200 |       50 |
| Drowned King    |   3,200 |       64 |
| Sky Sovereign   |   5,000 |       80 |

## Spire Difficulty

- Enemy HP multiplier (floor F): `1 + F × 0.18`
- Enemy DMG multiplier (floor F): `1 + F × 0.12`
- Gold drop multiplier (floor F): `1 + F/4`
- Affix count: `1 + F/5`

## Gold Costs

- Blacksmith repair/upgrade (+10% dmg): `50 + dmg × 4 × (1 + rarityIdx)`
- Blacksmith masterwork (rarity up): `500 × (1 + rarityIdx × 2)`
- Jeweler socket: 100g, unsocket: 250g
- Mage chimera reroll: `350 × (1 + rarityIdx)`
- Spire entry fee: `floor × 50`

## Skill Tree

- 3 branches × ~17 nodes = 51+ allocatable nodes
- Keystones cost 5 skill points and require full prior path
- 1 skill point per level
- Max level: 100 → 100 skill points total
