cmake_minimum_required(VERSION 3.16)
project(ShatteredWilds LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Dependencies
find_package(glfw3 3.3 REQUIRED)
find_package(vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(stb_image REQUIRED)
find_package(assimp REQUIRED)
find_package(nlohmann_json 3.2 REQUIRED)
find_package(entt REQUIRED)
find_package(spdlog REQUIRED)
find_package(utf8proc REQUIRED)

# Core library
add_library(Core STATIC
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

target_include_directories(Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/ECS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Math
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Logging
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Config
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Serialization
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/Utils
    ${GLFW3_INCLUDE_DIRS}
    ${Vulkan_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${STB_IMAGE_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${NLOHMANN_JSON_INCLUDE_DIR}
    ${ENTT_INCLUDE_DIRS}
    ${SPDLOG_INCLUDE_DIRS}
    ${UTF8PROC_INCLUDE_DIR}
)

target_link_libraries(Core PUBLIC
    glfw
    ${Vulkan_LIBRARIES}
    glm
    stb_image
    assimp
    nlohmann_json::nlohmann_json
    entt::entt
    spdlog::spdlog
    utf8proc::utf8proc
)

# Game library (will link with Core)
add_library(Game STATIC
    src/World/StreamingManager/StreamingManager.cpp
    src/World/Biomes/Biome.cpp
    src/World/DungeonGenerator/DungeonGenerator.cpp
    src/Player/Controller/PlayerController.cpp
    src/Player/Camera/GameCamera.cpp
    src/Combat/Weapons/WeaponBase.cpp
    src/Combat/Spells/SpellBase.cpp
    src/Progression/SkillTrees/SkillTree.cpp
    src/Loot/Items/ItemBase.cpp
    src/Loot/Gems/GemBase.cpp
    src/Loot/Runes/RuneBase.cpp
    src/NPCs/Base/NPCBase.cpp
    src/UI/Screens/UIScreen.cpp
    src/Networking/Client/NetworkClient.cpp
    src/Audio/AudioManager.cpp
    src/Rendering/Renderer.cpp
    src/GameSystems/QuestSystem.cpp
    src/GameSystems/EconomySystem.cpp
)

target_include_directories(Game PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Core
    ${CORE_INCLUDE_DIRS}
)

target_link_libraries(Game PUBLIC Core)

# Executable for desktop (placeholder)
add_executable(ShatteredWildsDesktop src/platforms/desktop/main.cpp)
target_link_libraries(ShatteredWildsDesktop PRIVATE Game)

# iOS executable (placeholder - will be configured in Xcode)
add_executable(ShatteredWildsiOSEmpty src/platforms/ios/main.cpp)

# Copy assets to build directory
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/assets)
add_custom_command(TARGET ShatteredWildsDesktop POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:ShatteredWildsDesktop>
)

# Install rules
install(TARGETS ShatteredWildsDesktop DESTINATION bin)
install(DIRECTORY assets/ DESTINATION .)
