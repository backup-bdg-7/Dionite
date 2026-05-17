# Dionite — Shattered Wilds (Studio Starter Kit)

## Original Problem Statement (verbatim, summarized)
Build a mobile app titled **Dionite / Shattered Wilds** — an open-world ARPG blending Elden Ring exploration, Diablo loot depth, and Fortnite gun modularity. iOS primary (Metal), shared C++ core, dedicated backend with PostgreSQL and React admin dashboard. AAA-indie quality. Full implementation, no stubs.

## Realistic Scope (mutually agreed with user)
- **Option B** chosen: deliver the complete source-code scaffolding for the native game (C++ core + Swift/Metal iOS + Android JNI + Node.js backend + React admin) as a Studio Starter Kit. User compiles and debugs on their own Mac + Xcode and reports issues for fixes.
- **Visual direction: Diablo III / IV-style isometric 3D** (revised mid-build from initial 2D plan). Fixed over-the-shoulder camera, click-to-move + WASD hybrid input, full PBR rendering pipeline with cascaded shadow maps and dynamic point lights.
- The Emergent preview hosts a **Studio Manifest landing page** (FastAPI + React) that browses the generated codebase so the user can verify what was produced without leaving the browser.

## Tech Decisions
- Game: C++17 shared core, Swift + Metal iOS host, Android NativeActivity (JNI).
- Backend: Node.js 20 + Express + PostgreSQL 16 + WebSocket + JWT (bcryptjs + jsonwebtoken).
- Admin: React 18 + Vite + axios + recharts.
- Preview only: FastAPI + MongoDB stub at `/app/backend/` + React landing page at `/app/frontend/`.

## What's Implemented (date: 2026-05-17)
- **Core (C++)**: ECS, Vec2/3/4, Mat4, AABB, deterministic Random, Logger, Config, Serializer, FileUtils, Time.
- **World**: 5 biome definitions, BSP procedural dungeon generator with sigil affixes, chunk streaming manager, dynamic events scheduler.
- **Player**: input-driven controller w/ dash, smoothed follow camera with shake, Soul Abilities registry (5 abilities), customization fields.
- **Combat**: WeaponBase + WeaponSystem (cooldown / reload / spread / pellets / explode); Mods registry (barrel/scope/mag/stock/muzzle/grip); Ammo types; Enchantment library (vampiric, echo-cast, chain-lightning, …); SpellBase + SpellSystem; pooled Projectile; EnemyAI state machine; BossAI with 2-phase pattern lists; FeedbackSystem (hitstop, shake, damage numbers); StatusManager (burn/slow/shock/poison/bleed/stun/soul-mark).
- **Progression**: classless SkillTree with 51+ nodes across 3 trees (Warrior, Arcanist, Shadow) and 3 keystones; LevelSystem (XP = 50 + L² × 18); SoulFragmentLedger.
- **Loot**: Item + Affix model; LootRoller with weighted rarity + luck; 6 Gems with fusion; 10 Runes; 5 RuneWords with detect(); SocketManager.aggregate(); ChestSystem (Wooden/Iron/Gilded/Vault/Mimic); Crafter (Chimera, Masterwork, Transmute).
- **NPCs**: NPCBase; Blacksmith (repair+masterwork); Jeweler (socket+unsocket); TravelingMage (chimera+roam); GeneralMerchant (restock+buy+sell); Dialogue graph+runner.
- **UI**: UIScreen + ScreenStack; HUD; LoginScreen; InventoryScreen (equip/unequip); ItemTooltip text builder; UIAnimator.
- **Networking**: NetworkClient (HTTP interface); Auth (register/login/apple); CloudSave (pull/push).
- **Audio**: AudioManager interface + Stub impl.
- **Rendering**: abstract Renderer + NullRenderer for headless validation.
- **Game Systems**: QuestSystem, Economy, InfinitySpire (escalating floors), GhostBattleSystem (record+sample), EnchantmentMatrix (5×5 with row+column synergies).
- **Platform — iOS (Swift)**: AppDelegate, GameViewController (MTKView + Metal pipeline), DioniteBridge (Swift→C++ via @_silgen_name), GamepadBridge (Xbox/PS via GameController), VirtualJoystick (touch), DioniteBridgeImpl.cpp (extern "C" exports), Shaders.metal (vertex + fragment + rim light), Info.plist.
- **Platform — Android**: MainActivity.java (NativeActivity + JNI bridge methods), build.gradle.
- **Platform — Desktop**: validation harness in `src/platforms/desktop/main.cpp`.
- **Backend**: Express server, auth routes (register/login/refresh/me/apple-sign-in), game routes (save/load/spire), admin routes (stats/players/grant_gold/delete), matchmaking routes (ghost POST/GET random), WebSocket gateway for live ghost streaming, economy + analytics services, Docker + docker-compose, SQL migration.
- **Admin dashboard**: React + Vite + axios; Login, Dashboard, Players, Leaderboard, Items pages.
- **Data**: `loot_tables.json`, `runes.json`, `gems.json`, `biomes.json`, `skill_tree.json`.
- **Docs**: `architecture.md`, `api-specs.md`, `build-instructions.md`, `balance-sheet.md`, `testing-plan.md`.
- **Preview**: FastAPI manifest API + React landing page that browses every file (filter + tabs + content preview).

**Generated totals:** 133 files, 5,811 lines (92 C++/.h+.cpp, 8 Swift/Metal/plist, 2 Android, 20 server JS/JSX/SQL/yml, 5 JSON data, 6 docs).

## 3D Conversion (revised from initial 2D plan)
- **Camera** (`src/Player/Camera/GameCamera.h`) rewritten as a fixed isometric/over-the-shoulder follow camera: pitch 55°, yaw 35°, distance 18 m, 60° FOV. Supports player-driven ±25° yaw pan (two-finger drag on touch, Alt-drag on desktop) — matches Diablo IV's preference.
- **PlayerController** (`src/Player/Controller/PlayerController.h`) now supports hybrid input: **click-to-move** with A\* navigation target + **WASD/stick** rotated by camera yaw so "up" is always away-from-camera.
- **NavGrid** (`src/World/DungeonGenerator/NavGrid.h`) — A\* over the dungeon tile graph, 8-connected, no corner-cutting, returns world-space waypoints.
- **DungeonMeshBuilder** (`src/World/DungeonGenerator/DungeonMeshBuilder.h`) — converts BSP tile grid into 3D scene data: floor quads, wall segments (3.2 m), prop anchors (chest / boss_pad / spawn / torch), point lights per room (alternating warm/cool).
- **Renderer** (`src/Rendering/Renderer.h`) extended with `PBRMaterial`, `PointLight`, `DirectionalLight`, `CameraUniforms` with 4-cascade shadow VP matrices.
- **Shaders.metal** rewritten as full PBR pipeline: GGX + Smith G + Schlick fresnel, normal mapping via TBN, cascaded shadow map sampling with 3×3 PCF, point-light loop, Reinhard tonemap + gamma 2.2, emissive bloom feed, rim light.
- **iOS GameViewController** picks up tap-to-move (projects NDC tap into world via `dionite_click_to_move`) and two-finger pan for `dionite_camera_pan`.
- **Desktop harness** now exercises full 3D loop: dungeon → mesh → NavGrid path → camera/player update with rendered draw submissions.
- **New doc:** `docs/visual-style.md` — camera config, biome lighting recipes, dungeon mesh conventions.

## What Won't Compile In The Preview (by design)
- The C++ game core needs `nlohmann_json` (and ideally GLFW + Vulkan for desktop, Assimp for meshes).
- The iOS layer needs **Xcode 15+ on a Mac** with Apple Developer signing.
- The backend wants **Postgres 16** running (use `cd server && docker compose up`).
- All of these are documented in `docs/build-instructions.md`.

## Next Action Items
- User builds locally (Mac + Xcode for iOS) per `docs/build-instructions.md`.
- User reports compile errors / runtime bugs — main agent patches the offending files.
- P1 enhancements queued: integrate `cpr` for real HTTP, wire `assimp` mesh loading in DioniteGame, add Vulkan renderer for desktop, build Xcode `.xcodeproj`, build proper Android `.gradle` modules.
- P2 enhancements: shader library (multi-pass for VFX), entity culling for streaming, save-blob schema versioning.

## Future / Backlog
- Live ops dashboard with revenue tracking (IAP cosmetics).
- Cross-platform PvP arena.
- Seasonal Spire affixes that rotate weekly.
- Modding API to register custom runes/gems/affixes from JSON.
