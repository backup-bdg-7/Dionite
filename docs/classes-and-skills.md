# Dionite — Classes, Skills & Paragon

This document is the canonical guide to how character classes, skills, and
the endgame paragon system are wired together.

## The 5 Classes

| Class       | Resource    | HP base/lvl | Theme biome    | Damage Affinity       |
| ----------- | ----------- | ----------: | -------------- | --------------------- |
| Crusader    | Wrath       | 130 / +8    | Ashen Wastes   | Physical, **Holy**    |
| Necromancer | Essence     | 110 / +6    | Sunken Crypts  | **Shadow**, Poison    |
| Sorcerer    | Mana        |  90 / +5    | Sky Citadel    | **Fire, Cold, Shock** |
| Ranger      | Discipline  | 105 / +6    | Verdant Wilds  | Physical, Poison      |
| Monk        | Spirit      | 115 / +7    | Frozen Spire   | Physical, Shock, Holy |

Code: `src/Progression/Classes/ClassRegistry.h`
Data: `assets/data/classes.json`

## Skill Categories (Diablo-style rules)

| Slot | Category   | Cost?       | Cooldown?  | Notes                                    |
| ---: | ---------- | ----------- | ---------- | ---------------------------------------- |
|   0  | Basic      | **None**    | **None**   | Generators. Always usable.               |
|   1  | Core       | Resource    | 0–2 s      | Primary spenders.                        |
|   2  | Defensive  | 0 or low    | 15–35 s    | Damage reduction, shields, invuln.       |
|   3  | Mobility   | low         | 6–30 s     | Dashes, teleports.                       |
|   4  | Utility    | varies      | 10–30 s    | Buffs, debuffs, summons, CC.             |
|   5  | Ultimate   | 0 or full   | 60–120 s   | Long-cooldown signature.                 |
|  Pas | Passive    | None        | None       | Up to 4 equipped, always on.             |

These rules are enforced in `LoadoutManager::tryCast` —
- `Basic` skills never check cooldown or cost; they generate resource.
- All other categories consume resource (×rank cost mult) and respect cooldowns
  (×rank cooldown mult, which decreases as you rank up).

Code: `src/Progression/Skills/SkillLibrary.h`,
`src/Progression/Skills/LoadoutManager.h`

## Skill Catalogue (~125 skills, 25/class)

Per class, the `SkillLibrary` loads:
- 3 Basics
- 3–4 Cores
- 3 Defensives
- 2 Mobilities
- 3–4 Utilities
- 2–3 Ultimates
- 7–8 Passives

Each skill has up to **5 ranks** with damage / cooldown / cost scaling:
- +25% damage per rank
- −10% cooldown per rank (floor 50%)
- −8% resource cost per rank (floor 50%)

Skill data lives in `src/Progression/Skills/SkillLibrary.<Class>.cpp`.

## Loadout

A `PlayerLoadout` carries one `ActiveSlot` per category (6 total) plus 4
passive slots. Players reassign anytime out of combat. Skill rank persists
per skill globally; loadout is just which to bring.

## Resource Manager

`ResourceState { current, maximum, regenPerSec }` lives on the player.
`LoadoutManager::tickResource(state, dt)` regens passively when applicable
(Mana, Essence, Spirit). Wrath and Discipline regenerate only via combat
(Basic generators + class passives).

## Paragon (endgame)

Unlocks at level 100. From there, **every 1.5 M XP grants 1 paragon level**
and 1 paragon point. Paragon points slot into nodes on a 21×21 board.
Boards include node types:

- **Normal** — small stat per point, up to 4 ranks.
- **Magic** — +stat with conditions.
- **Rare** — bigger bonus + secondary condition.
- **Legendary** — unique passive (single point).
- **Glyph Socket** — fits one glyph; glyph empowers same-stat nodes within
  its tile radius. Glyphs level up by clearing **Nightmare Dungeons**.

Code: `src/Progression/Paragon/ParagonSystem.h`

## Loading & Character UI Flow

1. App boot → `LoadingScreen` (60 % bar, biome tips).
2. Splash → `CharacterSelectScreen` (server lists saves via `/api/characters`).
3. New → `CharacterCreationScreen` (class pick + name + hardcore toggle).
4. Confirm → POST `/api/characters` → backend creates save blob → load game.

Code: `src/UI/Screens/{LoadingScreen, CharacterSelectScreen, CharacterCreationScreen}.h`
