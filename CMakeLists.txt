cmake_minimum_required(VERSION 3.20)
project(Dionite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# -----------------------------------------------------------------------------
# Dependencies (expect they were installed via vcpkg or system package manager).
# Set DIONITE_USE_BUNDLED=ON to skip find_package for headless CI builds.
# -----------------------------------------------------------------------------
option(DIONITE_USE_BUNDLED "Skip find_package for nlohmann_json (use bundled header)" OFF)
if(NOT DIONITE_USE_BUNDLED)
  find_package(nlohmann_json CONFIG QUIET)
endif()

# -----------------------------------------------------------------------------
# Core library (header-mostly, but includes .cpp stubs for build symbols).
# -----------------------------------------------------------------------------
add_library(DioniteCore STATIC
    src/Core/ECS/Entity.cpp
    src/Core/ECS/Component.cpp
    src/Core/ECS/System.cpp
    src/Core/Math/Vector.cpp
    src/Core/Math/Matrix.cpp
    src/Core/Logging/Logger.cpp
    src/Core/Config/ConfigManager.cpp
    src/Core/Serialization/Serializer.cpp
    src/Core/Utils/FileUtils.cpp
)
target_include_directories(DioniteCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
if(TARGET nlohmann_json::nlohmann_json)
  target_link_libraries(DioniteCore PUBLIC nlohmann_json::nlohmann_json)
endif()

# -----------------------------------------------------------------------------
# Game library (combat, world, loot, NPCs, UI, networking, rendering)
# -----------------------------------------------------------------------------
add_library(DioniteGame STATIC
    src/World/Biomes/Biome.cpp
    src/World/DungeonGenerator/DungeonGenerator.cpp
    src/World/StreamingManager/StreamingManager.cpp
    # 3D extensions:
    # DungeonMeshBuilder.h and NavGrid.h are header-only and consumed by Renderer
    # callers; no .cpp needed.
    src/Player/Controller/PlayerController.cpp
    src/Player/Camera/GameCamera.cpp
    src/Combat/Weapons/WeaponBase.cpp
    src/Combat/Spells/SpellBase.cpp
    src/Progression/SkillTrees/SkillTree.cpp
    src/Loot/Items/ItemBase.cpp
    src/NPCs/Base/NPCBase.cpp
    src/UI/Screens/UIScreen.cpp
    src/Networking/Client/NetworkClient.cpp
    src/Audio/AudioManager.cpp
    src/Rendering/Renderer.cpp
    src/GameSystems/QuestSystem.cpp
    src/GameSystems/EconomySystem.cpp
)
target_link_libraries(DioniteGame PUBLIC DioniteCore)

# -----------------------------------------------------------------------------
# Desktop executable (headless validation harness; replace main.cpp with a real
# GLFW/Vulkan main on your platform of choice).
# -----------------------------------------------------------------------------
add_executable(DioniteDesktop src/platforms/desktop/main.cpp)
target_link_libraries(DioniteDesktop PRIVATE DioniteGame)

# -----------------------------------------------------------------------------
# iOS bridge library (shared with Swift host app via .a).
# Build with: cmake -GXcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_ARCHITECTURES=arm64
# -----------------------------------------------------------------------------
if(CMAKE_SYSTEM_NAME STREQUAL "iOS" OR APPLE)
  add_library(DioniteIOS STATIC
      platforms/ios/Dionite/DioniteBridgeImpl.cpp
  )
  target_link_libraries(DioniteIOS PUBLIC DioniteGame)
  set_target_properties(DioniteIOS PROPERTIES
      XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 15.0
      XCODE_ATTRIBUTE_ENABLE_BITCODE NO)
endif()

# -----------------------------------------------------------------------------
# Install
# -----------------------------------------------------------------------------
install(TARGETS DioniteDesktop RUNTIME DESTINATION bin)
install(DIRECTORY assets/ DESTINATION share/dionite/assets)
