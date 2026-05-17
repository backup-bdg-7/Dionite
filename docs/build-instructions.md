# Dionite — Build Instructions

> ⚠ **This source tree is a studio starter kit.** It will NOT compile on the
> Emergent preview container (Linux + Python/Node). All build steps below
> must be run on your own developer machine (Mac for iOS, any OS for desktop).

## Prerequisites

| Tool                | Version | Notes                                          |
| ------------------- | ------- | ---------------------------------------------- |
| CMake               | ≥ 3.20  | `brew install cmake` / `apt install cmake`     |
| Clang / Apple LLVM  | C++17   | Xcode 15+ or Clang 14+                         |
| nlohmann/json       | latest  | `brew install nlohmann-json` or vcpkg          |
| (Optional) GLFW     | 3.3+    | only for desktop window                        |
| (Optional) Vulkan   | 1.3+    | only for desktop renderer                      |
| Node.js             | 20+     | for the backend / admin dashboard              |
| Docker              | 24+     | optional, for backend + Postgres stack         |
| Xcode               | 15+     | iOS build (Mac required)                       |

## 1. C++ Core (Desktop validation harness)
```bash
cmake -S . -B build -DDIONITE_USE_BUNDLED=ON
cmake --build build -j
./build/DioniteDesktop      # runs a headless validation loop
```

If you don't have `nlohmann/json` installed system-wide, drop `nlohmann/json.hpp`
into `src/external/nlohmann/` and toggle `-DDIONITE_USE_BUNDLED=ON`.

## 2. iOS Build (Mac + Xcode required)
```bash
# Generate Xcode project for the C++ bridge static lib
cmake -G Xcode -S . -B build_ios \
      -DCMAKE_SYSTEM_NAME=iOS \
      -DCMAKE_OSX_ARCHITECTURES=arm64 \
      -DCMAKE_OSX_DEPLOYMENT_TARGET=15.0
cmake --build build_ios --config Release
# Then open platforms/ios/Dionite.xcodeproj (you must create this via Xcode
# referencing the Swift sources under platforms/ios/Dionite/ and linking
# libDioniteIOS.a from build_ios/Release-iphoneos/).
```
Sign with your Apple Developer team, set `PRODUCT_BUNDLE_IDENTIFIER` to
`com.dionite.shatteredwilds`, archive, and submit to TestFlight.

## 3. Android Build (Studio + NDK)
Open `platforms/android` in Android Studio (or run `./gradlew assembleRelease`).
The `MainActivity.java` JNI methods call into the C++ library compiled by
CMake at the repo root.

## 4. Backend & Admin Dashboard
```bash
cd server
docker compose up --build       # starts Postgres + server (:4000) + admin (:5173)
```
Or run server only:
```bash
cd server
yarn install
DATABASE_URL=postgres://dionite:dionite@localhost:5432/dionite \
JWT_SECRET=change-me \
yarn dev
```
Admin only:
```bash
cd server/web
yarn install
VITE_API_URL=http://localhost:4000 yarn dev
```
Default admin (seeded only if you POST `/api/auth/register` with
`email=admin@dionite.game` then bump role manually via SQL or create your own).

## 5. Asset Pipeline
Drop `.fbx` / `.gltf` into `assets/models/`. The runtime supports
`assimp`-loaded meshes (link `find_package(assimp)` in `CMakeLists.txt`
under `DioniteGame` once you wire up the import path).

Audio: `.wav` / `.ogg` go in `assets/audio/{music,sfx,voice}/`.

Data: extend `assets/data/*.json` for new runes, gems, biomes, bosses.
