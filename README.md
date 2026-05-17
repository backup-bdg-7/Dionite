# Shattered Wilds (Dionite)

An open-world ARPG blending Elden Ring exploration, Diablo loot depth, and Fortnite gun modularity.

## Vision
Start weak and afraid → become an unstoppable god with broken builds, insane loot, and endless endgame.

## Platforms
- iOS (primary, Metal)
- Cross-platform shared C++ core
- Dedicated backend with PostgreSQL, secure auth, cloud saves, and React-based admin web dashboard

## Folder Structure
```
ShatteredWilds/
├── CMakeLists.txt                  # Root build
├── README.md
├── LICENSE
├── .gitignore
├── docs/                           # Architecture, API, balance docs
│   ├── architecture.md
│   ├── api-specs.md
│   ├── balance-sheet.xlsx
│   └── diagrams/                   # Draw.io / PlantUML files
│
├── assets/                         # All game assets (organized)
│   ├── models/                     # 3D models (fbx/gltf)
│   ├── textures/                   # PBR textures, atlases
│   ├── animations/                 # Skeleton animations
│   ├── audio/                      # SFX, music, voice
│   ├── vfx/                        # Particle systems, shaders
│   ├── ui/                         # HUD, menus, icons
│   ├── fonts/
│   ├── logos/                      # Game logo, app icons (all sizes)
│   └── data/                       # JSON/CSV for loot tables, runes, etc.
│
├── src/                            # Shared C++ Core (90%+ code)
│   ├── Core/
│   │   ├── ECS/                    # Entity Component System
│   │   ├── Math/
│   │   ├── Logging/
│   │   ├── Config/
│   │   ├── Serialization/          # Save/Load
│   │   └── Utils/
│   │
│   ├── World/
│   │   ├── StreamingManager/       # Open world loading
│   │   ├── Biomes/                 # 5 biome definitions
│   │   ├── DungeonGenerator/       # Procedural + affixes
│   │   ├── Environment/            # Puzzles, emotes, secrets
│   │   └── Events/                 # Dynamic events
│   │
│   ├── Player/
│   │   ├── Controller/
│   │   ├── Camera/
│   │   ├── Abilities/              # Unlocked soul powers
│   │   └── Customization/
│   │
│   ├── Combat/
│   │   ├── Weapons/
│   │   │   ├── Base/
│   │   │   ├── Ammo/
│   │   │   │   ├── Types/
│   │   │   │   └── Effects/
│   │   │   ├── Mods/               # Barrels, scopes, etc.
│   │   │   └── Enchantments/       # Vampiric, Echo-Cast, etc.
│   │   ├── Spells/
│   │   ├── Projectiles/
│   │   ├── AI/                     # Boss + enemy AI
│   │   ├── Feedback/               # Screen shake, hitstop, damage numbers
│   │   └── Effects/                # Status, VFX binding
│   │
│   ├── Progression/
│   │   ├── SkillTrees/             # 3 trees, 50+ nodes
│   │   ├── Levels/
│   │   └── SoulFragments/          # Campaign rewards
│   │
│   ├── Loot/
│   │   ├── Items/                  # Base item definitions
│   │   ├── RaritySystem/
│   │   ├── Gems/
│   │   │   ├── Types/
│   │   │   ├── Upgrades/
│   │   │   └── Effects/
│   │   ├── Runes/
│   │   │   ├── Types/
│   │   │   ├── Words/              # Rune Word logic
│   │   │   └── Synergies/
│   │   ├── Socketing/              # Socket manager
│   │   ├── Chests/                 # Mimics, Vaults
│   │   └── Crafting/               # Chimera, Masterwork
│   │
│   ├── NPCs/
│   │   ├── Base/
│   │   ├── Vendors/
│   │   │   ├── Blacksmith/
│   │   │   ├── Jeweler/
│   │   │   ├── TravelingMage/
│   │   │   └── GeneralMerchant/
│   │   └── Dialogue/
│   │
│   ├── UI/
│   │   ├── Screens/                # Login, menus, HUD
│   │   ├── Inventory/
│   │   ├── Tooltips/
│   │   └── Animations/
│   │
│   ├── Networking/
│   │   ├── Client/                 # API calls, WebSocket
│   │   ├── Auth/
│   │   └── Sync/                   # Cloud save logic
│   │
│   ├── Audio/
│   ├── Rendering/                  # Metal/Vulkan abstraction
│   └── GameSystems/                # Economy, Quests, etc.
│
├── server/                         # Backend Services
│   ├── src/
│   │   ├── main/                   # Entry point
│   │   ├── api/                    # REST + GraphQL
│   │   ├── auth/                   # JWT, Apple ID, etc.
│   │   ├── database/               # Models, migrations (PostgreSQL)
│   │   ├── game/                   # Player data, inventory, builds
│   │   ├── admin/                  # Admin logic
│   │   ├── matchmaking/            # Ghost battles, etc.
│   │   └── services/               # Economy, analytics
│   ├── web/                        # Admin Dashboard (React/Next.js)
│   │   ├── public/
│   │   ├── src/
│   │   │   ├── components/
│   │   │   ├── pages/              # Players, Analytics, etc.
│   │   │   └── utils/
│   │   └── package.json
│   ├── Dockerfile
│   ├── docker-compose.yml
│   └── migrations/                 # DB schema
│
├── platforms/                      # Platform-specific
│   ├── ios/                        # Xcode project, SwiftUI glue, Metal
│   ├── android/
│   ├── desktop/                    # Win/mac entry
│   └── shared/                     # Common platform code
│
├── tests/                          # Unit + integration + playtests
├── tools/                          # Asset pipeline, build scripts
├── build/                          # CMake output (gitignored)
└── releases/                       # Signed builds, TestFlight
```

## Development Plan

### Phase 0: Setup
* CMake + cross-platform build
* Core ECS, Logging, Math, Rendering abstraction
* Backend (Docker + PostgreSQL) + basic web admin

### Phase 1: Foundation
* Player controller, camera, basic movement + soul abilities
* Inventory + basic loot
* Login / cloud save system
* UI framework + login screen with animations

### Phase 2: World & Combat
* Open world streaming + 5 biomes
* Procedural dungeon generator + sigil affixes
* Full combat (guns, spells, feedback systems)
* Boss AI (2-phase)

### Phase 3: Depth Systems
* Skill trees (50+ nodes)
* Enchantment Matrix
* Gem & Rune Socket System + Rune Words
* Crafting (Chimera, upgrades)

### Phase 4: Economy & NPCs
* Blacksmith, Jeweler, Traveling Mage (roaming)
* Full vendor inventories, dialogue, animations

### Phase 5: Endgame & Polish
* Infinity Spire + ghost battles
* All VFX, audio, screen shake, damage numbers
* Secrets, emotes, hidden areas
* Performance optimization (60 FPS mobile)

### Phase 6: Cross-Platform & Live
* Platform layers
* Server scaling, anti-cheat basics
* App icons, logos, splash screens
* TestFlight / Store deployment

## Building

### Desktop (Linux/macOS/Windows)
```bash
mkdir -p build && cd build
cmake ..
make -j$(nproc)
```

### iOS
Use Xcode project in `platforms/ios/`

## Backend
See `server/` directory for Docker setup and API documentation.

## License
See LICENSE file.

## Acknowledgements
Built with passion by the Dionite team.
