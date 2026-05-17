# Dionite — Architecture Overview

This document describes the runtime architecture of **Dionite / Shattered Wilds**.

## High-Level Diagram

```
+------------------------------------------------------------+
|                    Platform Host                            |
|   iOS (Swift+Metal)   |   Desktop (GLFW+Vulkan)             |
+-----------+----------+-+--------------------+---------------+
            | C ABI bridge (dionite_* funcs)                  |
+-----------v-------------------------------------------------+
|                     DioniteGame (C++ static lib)            |
| Combat | World | Loot | Progression | NPCs | UI | Audio …   |
+-----------+-------------------------------------------------+
            | uses
+-----------v-------------------------------------------------+
|                     DioniteCore (C++ static lib)            |
|  ECS | Math | Logger | Config | Serialization | FileUtils   |
+-------------------------------------------------------------+
```

## Modules

### Core (`src/Core/`)
- **ECS** — Entity/Component/System pattern with type-indexed component lookup.
- **Math** — `Vec2/3/4`, `Mat4`, `AABB`, deterministic `Random` (SplitMix64).
- **Logging** — tagged, level-filtered logger with timestamps.
- **Config** — JSON-backed config with `find_package(nlohmann_json)`.
- **Serialization** — read/write JSON blobs for save game.
- **Utils** — `FileUtils`, `Time` / `FrameTimer`.

### World (`src/World/`)
- **Biomes** — 5 biome definitions (Verdant, Ashen, Frozen, Sunken, Sky).
- **DungeonGenerator** — BSP-style procedural rooms + L-corridors with sigil affixes.
- **StreamingManager** — chunk load/unload by camera distance.
- **Events** — dynamic events (Blood Moon, Treasure Goblin, Invasion).

### Player (`src/Player/`)
- **PlayerController** — input → motion/aim/dash; mana regen; lifesteal apply.
- **GameCamera** — smoothed follow camera with shake offset.
- **Abilities** — soul abilities (Dash, Grasp, Blast, Veil, Avatar).
- **Customization** — body/head meshes + tint + sigil.

### Combat (`src/Combat/`)
- **Weapons** — modular base + slots (barrel/scope/mag/stock/muzzle/grip), enchantments (vampiric, echo, chain).
- **Spells** — projectile / AoE / channel / buff / teleport.
- **Projectiles** — pooled, lifetime-managed, supports pierce.
- **AI** — `EnemyAI` state machine (Idle/Patrol/Chase/Attack/Flee/Dead) and `BossAI` (2-phase, telegraphed patterns).
- **Feedback** — `FeedbackSystem` (hitstop, screen shake, floating damage numbers).
- **Effects** — burn/slow/shock/poison/bleed/soul-mark/stun.

### Progression
- **SkillTree** — 3 branches (Warrior, Arcanist, Shadow) × ~17 nodes = 50+ nodes total, with keystones.
- **LevelSystem** — XP formula `50 + lvl² × 18`.
- **SoulFragments** — boss-drop fragments fueling biome unlocks.

### Loot
- **ItemBase** — `Item` with affixes + ordered sockets.
- **RaritySystem** — 6 tiers + colors.
- **Gems** — 6 base gems, tier-up via 3-stone fusion (tier ≤ 5).
- **Runes** — 10 base runes; `RuneWordLibrary::detect()` matches exact ordered combos.
- **Socketing** — `SocketManager::aggregate()` rolls up affix + socket + rune-word stats.
- **Chests** — wooden/iron/gilded/vault + mimic spawn.
- **Crafting** — Chimera (reroll), Masterwork (rarity up), Transmute (3→1 upgrade).

### NPCs
- **Blacksmith** — repair + masterwork.
- **Jeweler** — socket / unsocket.
- **Traveling Mage** — chimera + roams biomes.
- **General Merchant** — buy/sell with weighted offer restock.
- **Dialogue** — graph/runner.

### UI
- **HUD** — orb health/mana, hotbar, minimap, buffs.
- **LoginScreen / InventoryScreen** — declarative state only; rendered by platform.
- **ItemTooltip / UIAnimator**.

### Networking
- **NetworkClient** — REST HTTP interface (target platform supplies libcurl/cpr).
- **Auth** — register/login/apple sign-in glue.
- **CloudSave** — pull/push JSON player blob.

### Game Systems
- **QuestSystem**, **Economy**, **InfinitySpire**, **GhostBattles**, **EnchantmentMatrix** (5×5 grid w/ row+column synergies).

## Backend
Node.js + Express + PostgreSQL.
- `/api/auth` — register / login / refresh / me / apple
- `/api/save` — get / put / reset
- `/api/spire/run` — submit; `/api/spire/leaderboard` — top 50
- `/api/match/ghost` — record; `/api/match/ghost/random/:floor` — fetch
- `/api/admin/*` — stats, players, grant_gold, delete
- `ws://.../ws/ghost?floor=N&token=...` — live ghost streaming

## Platform Notes
- iOS: build `DioniteIOS` static lib, link from Xcode project at `platforms/ios/Dionite.xcodeproj` (generated separately). Metal shader at `Shaders.metal`.
- Desktop: implement GLFW/Vulkan window in `src/platforms/desktop/main.cpp` (the provided file is a headless harness).
- Android: NativeActivity + JNI bridge in `platforms/android/`.
