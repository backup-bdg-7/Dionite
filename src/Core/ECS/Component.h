#pragma once

#include <entt/entt.hpp>

namespace ShatteredWilds::Core::ECS {

class Component {
public:
    virtual ~Component() = default;
    // Components are data-only, no logic here
};

} // namespace ShatteredWilds::Core::ECS
