# Dionite — Testing Plan

## Unit Tests (C++)
Recommended framework: `Catch2` or `doctest`. Place tests under `tests/`.

### Core
- ECS: `World::create()` returns unique ids; component add/get/remove cycle.
- Math: `Vec2::normalized()` on zero returns `(0,0)`; `Mat4::perspective` produces valid clip space.
- Random: deterministic with seed.
- Logging: level filtering.

### Combat
- WeaponSystem cooldown gating.
- WeaponSystem `fromTemplate("shotgun")` produces 7 pellets.
- ProjectilePool reuses slots after lifetime.
- EnemyAI transitions Chase → Attack at attackRange.

### Loot
- LootRoller rarity distribution converges to weights over 10k samples (chi² ≤ 0.05).
- SocketManager detects DOOM only on exact order `sol/amn/ral`.
- Crafter::transmute requires same-rarity, bumps tier.

### Progression
- SkillTree node lookup; prerequisites enforced.
- LevelSystem awards exactly one skill point per ding.

## Integration Tests (Server)
- POST register → login → /me round-trip.
- PUT save then GET save returns identical blob.
- Admin endpoints reject non-admin tokens (403).

## Playtests
- 30-min "first run" — does a new player reach Verdant Warden? Is it satisfying?
- 2-hour endgame — Spire floor 20+ with 4-socket weapon and rune word.
- Touch controls feel — left stick deadzone < 0.2, no double-tap drift.
- Controller — Xbox + DualSense detected; right trigger fires; sticks invert.

## Performance Targets
- 60 FPS @ 1080p on iPhone 13+ (Metal 2-tier 4).
- 120 FPS on Pro Motion devices.
- ≤ 1.2 GB RSS on iPhone 12 mini (4 GB total).
- Loading time < 3 s cold start.
