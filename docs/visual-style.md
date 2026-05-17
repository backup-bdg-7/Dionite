# Dionite — Visual & Camera Style Guide (Diablo III / IV-inspired 3D)

> The kit is built around an **isometric/over-the-shoulder 3D** camera, not 2D.
> Every system uses `Vec3` world positions, `Mat4` view/projection matrices,
> and a Renderer interface that supports PBR materials + cascaded shadows +
> dynamic point lights, mirroring Diablo IV's lighting model.

## Camera

- **Type:** Fixed isometric/over-the-shoulder with smoothed follow.
- **Defaults:** pitch 55°, yaw 35°, distance 18 m, 60° vertical FOV.
- **User control:** **two-finger horizontal drag** (touch) or **Alt + Mouse drag**
  (desktop) yaws the camera ±25° around the player — matches Diablo IV's "camera
  rotation" preference. No free orbit.
- **Smoothing:** exponential lerp w/ time-constant 9 s⁻¹.
- **Code:** `src/Player/Camera/GameCamera.h`.

## Player Input

Players can use **either or both** at any time:
1. **Click-to-move** — click/tap the ground; the world position is path-found
   via `NavGrid` (A* on the dungeon tile graph, 8-connected, no corner-cutting).
2. **WASD / Left stick / Left thumb** — direct movement, rotated by the camera
   yaw so "up" always means "away from the camera". Cancels the click target.

Aim direction priority: **mouse world cursor > right stick > movement vector**.

## Rendering

- **PBR pipeline** with metallic + roughness + AO packed in MRA texture.
- **Normal mapping** via tangent-space TBN.
- **Cascaded shadow maps** — 4 cascades (splits at 8 / 24 / 60 / 200 m), 3×3 PCF.
- **Dynamic point lights** — one per dungeon room (alternating warm/cool tints),
  attenuation `(1 - d/r)²`, max 16 per scene tile.
- **Tonemap:** Reinhard + 2.2 gamma.
- **Emissive bloom feed** — material's emissive channel modulated by
  `albedoTint.a`.
- **Rim light** — subtle warm highlight (`0.18 × pow(1-NdV, 3)`) for that
  Diablo "characters pop off dark backgrounds" feel.

Source: `src/Rendering/Renderer.h` and `platforms/ios/Dionite/Shaders.metal`.

## Biome Lighting Recipes

| Biome           | Sun Direction       | Sun Color (linear) | Ambient | Point-Light Hue |
| --------------- | ------------------- | ------------------ | ------- | --------------- |
| Verdant Wilds   | (-0.4, -1, -0.3)    | warm white         | mossy   | green/amber     |
| Ashen Wastes    | (-0.3, -1, -0.5)    | orange-tinted      | smoky   | ember           |
| Frozen Spire    | (-0.5, -1, -0.2)    | cool blue-white    | icy     | cyan/silver     |
| Sunken Crypts   | almost vertical     | dim teal           | abyssal | bioluminescent  |
| Sky Citadel     | strong directional  | gold-white         | warm    | gold/sun        |

## Dungeon Mesh

The BSP tile grid is fed into `DungeonMeshBuilder` which emits:
- One **floor quad** per `Floor/Spawn/Chest/BossPad` tile.
- One **wall segment** (height 3.2 m) per `Wall` tile.
- **Prop anchors**: `chest`, `boss_pad`, `spawn`, `torch` (two torches per room).
- **Point lights** at room centers.

The renderer instances pre-uploaded floor/wall meshes per biome material —
this is how Diablo IV stays performant on phones while showing thousands of
floor tiles.

## Asset Conventions

| Folder                 | Contents                                      |
| ---------------------- | --------------------------------------------- |
| `assets/models/`       | `.fbx` / `.gltf` characters, weapons, props   |
| `assets/textures/<biome>/` | `<material>_albedo.png`, `_normal.png`, `_mra.png`, `_emissive.png` |
| `assets/animations/`   | skeleton animations (`.fbx`) by character     |
| `assets/audio/{music,sfx,voice}/` | `.ogg` preferred                   |
| `assets/data/*.json`   | game-data (runes, gems, biomes, skill tree)   |
