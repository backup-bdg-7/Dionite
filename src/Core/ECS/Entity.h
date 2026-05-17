#pragma once

#include <entt/entt.hpp>

namespace ShatteredWilds::Core::ECS {

class Entity {
public:
    Entity(entt::entity handle, entt::registry& registry);
    ~Entity() = default;

    bool IsValid() const;
    entt::entity GetHandle() const;

    // Template methods for component manipulation would go here in a full implementation
    // For brevity in this skeleton, we're showing the core concept

private:
    entt::entity m_handle{entt::null};
    entt::registry* m_registry{nullptr};
};

} // namespace ShatteredWilds::Core::ECS
