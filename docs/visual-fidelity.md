# Dionite — Visual Fidelity Pipeline (Diablo IV Bar)

This document is the **visual-quality contract**. It explains every rendering layer
you need to wire into the iOS Metal backend (or Vulkan desktop) to make the game
*look* like Diablo IV — not a flat tile demo.

## The Pipeline (top-down)

```
1.  Shadow pass — 4 cascade depth maps for directional sun
2.  Terrain pass — 4-layer splat (grass / rock / dirt / snow), triplanar for cliffs,
                   per-vertex normals, normal-map blending, distance fog
3.  Opaque scene — PBR (GGX + Smith + Schlick), point lights, normal maps,
                   emissive, rim light
4.  Foliage — instanced draws, alpha-test for leaves, wind sway in vertex shader
5.  Decals — blood pools, scorch marks, banners
6.  Transparent — water (Fresnel + dual-scrolling normal maps), particles, VFX
7.  Post-FX — bright-pass → bloom blur → composite → color grade → CA → vignette → dither
```

Source files:
- `platforms/ios/Dionite/Shaders.metal` — PBR main pass.
- `platforms/ios/Dionite/TerrainShaders.metal` — splat-map terrain + water.
- `platforms/ios/Dionite/PostFXShaders.metal` — bloom, grading, vignette, CA, dither.

## Per-biome lighting + post-FX presets

| Biome           | Sun Color       | Ambient    | Fog Color  | Fog Density | Bloom | Saturation | Look                       |
| --------------- | --------------- | ---------- | ---------- | ----------- | ----- | ---------- | -------------------------- |
| Verdant Wilds   | `#fef3c7` warm  | `#1f2937`  | `#0a1a10`  | 0.010       | 1.0   | 1.05       | misty forest mid-day       |
| Ashen Wastes    | `#fb923c` ember | `#7f1d1d`  | `#1a0d05`  | 0.022       | 1.4   | 1.10       | sulphurous, infernal       |
| Frozen Spire    | `#bae6fd` cool  | `#1e293b`  | `#08121f`  | 0.018       | 1.2   | 0.90       | bleak ice with rim cyan    |
| Sunken Crypts   | `#5eead4` teal  | `#022c22`  | `#061d22`  | 0.030       | 1.3   | 1.00       | bioluminescent abyss       |
| Sky Citadel     | `#fde047` gold  | `#fef3c7`  | `#1a1208`  | 0.014       | 1.5   | 1.15       | celestial gold + clouds    |

## World Events (push global overrides)

`WorldEventSystem` events ship their own `fogColorHex`, `ambientHex`, `sunColorHex`,
`postBloomMult`, `postSaturation`, and `postFogDensity` so a Helltide turns
EVERY biome into a deep crimson, regardless of base palette.

## Dungeon Atmosphere

`DungeonDecorator` is layered on top of `DungeonMeshBuilder` output and adds:
- biome-themed **banners**, **hanging chains** (boss rooms), **wall torches**,
- **debris** scatter across floor tiles,
- **blood pools** in boss & treasure rooms,
- **fog volumes** centered on boss rooms (heavier density for drama),
- biome-specific **ambient SFX loop**.

## Boss Encounters

`BossEncounterLibrary` defines:
- **Arena props** with per-instance point lights (soul braziers, gold braziers).
- **Two-phase mechanics** with telegraphs (`telegraphVfx`, `telegraphSeconds`),
  cooldown timers, damage, radius.
- **Cinematic intro** — letterbox seconds, camera path, voice line stack, music id.
- **Nameplate color** matching boss element.

`BossEncounterDriver::tick(dt)` returns the next mechanic to play, respecting
phase + cooldowns. Render layer just consumes telegraph VFX id, draws the warning
shape (red ring / orange cone / yellow line / etc.) for `telegraphSeconds`, then
plays the impact VFX.

## Required Texture Assets per Biome

```
assets/textures/<biome>/
  ├─ grass_albedo.png        2048x2048  sRGB
  ├─ grass_normal.png        2048x2048  linear
  ├─ grass_mra.png           2048x2048  linear (R=metallic G=rough B=AO)
  ├─ rock_albedo.png         2048x2048  sRGB
  ├─ rock_normal.png         2048x2048  linear
  ├─ rock_mra.png            2048x2048  linear
  ├─ dirt_albedo.png         1024x1024  sRGB
  ├─ dirt_normal.png         1024x1024  linear
  ├─ dirt_mra.png            1024x1024  linear
  ├─ snow_albedo.png         1024x1024  sRGB
  ├─ snow_normal.png         1024x1024  linear
  └─ snow_mra.png            1024x1024  linear

assets/textures/water/
  ├─ water_normal_a.png      1024x1024  linear (animated UV A)
  └─ water_normal_b.png      1024x1024  linear (animated UV B)

assets/textures/lut/
  ├─ verdant.cube      → bake to 32^3 LUT texture
  ├─ ashen.cube
  ├─ frozen.cube
  ├─ sunken.cube
  └─ sky.cube
```

## VFX Library (telegraph + impact shapes)

The boss/dungeon system references these by name. Build each as a particle
prefab + ground decal in your asset pipeline:

| VFX ID            | Shape                | Color        | Use case             |
| ----------------- | -------------------- | ------------ | -------------------- |
| `ring_red`        | expanding ground ring| crimson      | Meteor warning       |
| `ring_yellow`     | ground ring          | gold         | AoE smash            |
| `ring_white`      | ground ring          | white        | Unmaking channel     |
| `cone_orange`     | 60° cone             | fire orange  | Tyrant's Roar        |
| `line_blue`       | 3m wide line         | ice          | Drowned geyser       |
| `line_yellow`     | 3m wide line         | shock        | Chain Lightning      |
| `line_orange`     | charge corridor      | ember        | Burning Charge       |
| `circle_gold`     | overhead circle      | gold pillar  | Divine Judgement     |
| `circle_teal`     | overhead circle      | abyssal teal | Drowned Grasp        |
| `wave_white`      | radial wave          | dust         | Gust push            |
| `spiral_blue`     | swirling spiral      | maelstrom    | Drowned King pull    |
| `aura_red`        | self-aura            | fire         | Tyrant phase 2       |
| `aura_gold`       | self-aura            | divine       | Sovereign phase 2    |
| `aura_green`      | self-aura            | nature       | Doppelgänger spawn   |

## Performance Targets

- iPhone 13+ : **60 FPS @ 1080p** with full post-FX stack.
- iPhone 12 mini : **60 FPS @ 1080p** with bloom mip count reduced from 5 → 3.
- iPad Pro M-series : **120 FPS @ 1440p** with HDR.
- Memory: terrain mesh (512^2 single mesh) ≈ 64 MB; with LOD0..2 streaming ≈ 24 MB.

## Cheat Sheet — "Why does Diablo IV look like Diablo IV?"

1. **Heavy atmospheric fog** that gets thicker with distance (we have it).
2. **Bloom on emissives** — torches, gem glow, weapon trails (PostFX composite).
3. **Color grading per biome** — LUT-driven (compositor reads 32³ LUT).
4. **Rim light** on characters against dark backgrounds (PBR fragment).
5. **High-contrast tonemap** with crushed blacks (Reinhard + 1.2 contrast).
6. **Vignette** dims edges, focuses on player (PostFX composite).
7. **Splat-mapped terrain** so ground never feels tiled-flat (TerrainShaders.metal).
8. **Decals & blood pools** soak the floor in storytelling (DungeonDecorator).
9. **Telegraphed boss mechanics** — ground-painted warnings (BossEncounter).
10. **Cinematic letterbox** on boss intros (BossEncounter.cinematic).

Hit all 10 and the game will feel like Diablo IV.
