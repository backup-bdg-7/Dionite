# 🗡️ Dionite — Shattered Wilds

> An open-world ARPG with **Diablo III/IV-style isometric 3D combat**, Elden Ring exploration scope, Diablo loot depth,
> and Fortnite gun modularity. Native iOS (Metal) primary platform with shared C++ core and PostgreSQL backend.
>
> **Visual style:** fixed over-the-shoulder isometric camera (pitch 55°, yaw 35°), click-to-move + WASD hybrid input,
> full PBR rendering with cascaded shadow maps, dynamic point lights per dungeon room, normal-mapped meshes,
> Reinhard tonemap + 2.2 gamma. See [`docs/visual-style.md`](docs/visual-style.md).

## ⚠ Important Note on this Repository

This repo is a **Studio Starter Kit** — a fully scaffolded, well-architected source tree designed to be taken to a **Mac + Xcode** (for iOS) or a **desktop dev box** for compilation, integration, and continued development. It will **not compile or run** inside the Emergent Linux container (no Metal, no Xcode, no signing). The preview URL shows a **Studio Manifest** landing page documenting what is in this tree.

## Project Vision

Start weak and afraid → become an unstoppable god with broken builds, insane loot, and endless endgame.

## Key Systems (all implemented as code, not stubs)

| System                  | Location                                        |
| ----------------------- | ----------------------------------------------- |
| ECS + Math + Logging    | `src/Core/`                                     |
| 5 Biomes                | `src/World/Biomes/`, `assets/data/biomes.json`  |
| Procedural Dungeons     | `src/World/DungeonGenerator/` (BSP + sigil affixes) |
| Open-world Streaming    | `src/World/StreamingManager/`                   |
| Player Controller       | `src/Player/Controller/`                        |
| **Diablo-style** Isometric Camera + Click-to-move ground pick | `src/Player/Camera/`     |
| **A\* NavGrid** pathfinding | `src/World/DungeonGenerator/NavGrid.h`       |
| **3D Dungeon Mesh Builder** (floor/wall/prop/light) | `src/World/DungeonGenerator/DungeonMeshBuilder.h` |
| Soul Abilities          | `src/Player/Abilities/`                         |
| Modular Weapons + Mods  | `src/Combat/Weapons/{Base,Ammo,Mods,Enchantments}` |
| Spells                  | `src/Combat/Spells/`                            |
| Pooled Projectiles      | `src/Combat/Projectiles/`                       |
| Enemy + 2-Phase Boss AI | `src/Combat/AI/`                                |
| Hitstop + Shake + Numbers | `src/Combat/Feedback/`                        |
| Status Effects          | `src/Combat/Effects/`                           |
| Skill Tree (51+ nodes)  | `src/Progression/SkillTrees/`, `assets/data/skill_tree.json` |
| Levels / XP             | `src/Progression/Levels/`                       |
| Soul Fragments          | `src/Progression/SoulFragments/`                |
| Items + Affixes + Roller | `src/Loot/Items/`                              |
| Gems (6) + Fusion       | `src/Loot/Gems/`, `assets/data/gems.json`       |
| Runes (10) + Rune Words | `src/Loot/Runes/{Base,Words}`, `assets/data/runes.json` |
| Socket Manager          | `src/Loot/Socketing/`                           |
| Chests + Mimics + Vaults| `src/Loot/Chests/`                              |
| Crafting (Chimera/Masterwork/Transmute) | `src/Loot/Crafting/`            |
| Blacksmith              | `src/NPCs/Vendors/Blacksmith/`                  |
| Jeweler                 | `src/NPCs/Vendors/Jeweler/`                     |
| Traveling Mage          | `src/NPCs/Vendors/TravelingMage/`               |
| General Merchant        | `src/NPCs/Vendors/GeneralMerchant/`             |
| Dialogue Graph + Runner | `src/NPCs/Dialogue/`                            |
| HUD + Screens + Tooltips | `src/UI/`                                      |
| HTTP Client + Auth + Cloud Save | `src/Networking/`                       |
| Audio Manager (abstract) | `src/Audio/`                                   |
| Renderer (abstract + NullRenderer) | `src/Rendering/`                     |
| Quest System            | `src/GameSystems/QuestSystem.h`                 |
| Economy                 | `src/GameSystems/EconomySystem.h`               |
| Infinity Spire endgame  | `src/GameSystems/InfinitySpire.h`               |
| Ghost Battles           | `src/GameSystems/GhostBattles.h`                |
| Enchantment Matrix (5×5)| `src/GameSystems/EnchantmentMatrix.h`           |
| iOS Swift glue + Metal shader | `platforms/ios/Dionite/`                  |
| Virtual joysticks       | `platforms/ios/Dionite/VirtualJoystick.swift`   |
| Xbox / PlayStation gamepad bridge | `platforms/ios/Dionite/GamepadBridge.swift` |
| Android JNI scaffold    | `platforms/android/`                            |
| Desktop CMake harness   | `src/platforms/desktop/main.cpp`                |
| Backend (Node + PG)     | `server/`                                       |
| Admin Dashboard (React) | `server/web/`                                   |

## Folder Structure
See [`docs/architecture.md`](docs/architecture.md) for a full module map.

## Build
See [`docs/build-instructions.md`](docs/build-instructions.md).

```bash
# C++ validation harness (Linux/Mac)
cmake -S . -B build -DDIONITE_USE_BUNDLED=ON && cmake --build build -j

# Backend + admin (Docker)
cd server && docker compose up --build
```

## Controls

| Input         | Action                                           |
| ------------- | ------------------------------------------------ |
| WASD / Left stick / Left thumb | Move                            |
| Mouse / Right stick / Right thumb | Aim                          |
| LMB / RT / A button / Fire button | Shoot                        |
| Space / B / B button / Dash button | Dash                        |
| Q / LB / X button / Ability 1 | Soul Ability 1                   |
| E / RB / Y button / Ability 2 | Soul Ability 2                   |
| F / A button (near NPC) | Interact                              |
| Tab / Select  | Inventory                                        |
| Esc / Start   | Pause                                            |

Gamepad support uses Apple's `GameController` framework (`GamepadBridge.swift`) — Xbox, PlayStation, and any MFi controller. Touch controls use two virtual joysticks plus a fire button.

## License
MIT — see [LICENSE](LICENSE).
