#include "Entity.h"

namespace ShatteredWilds::Core::ECS {

Entity::Entity(entt::entity handle, entt::registry& registry)
    : m_handle(handle), m_registry(&registry) {}

bool Entity::IsValid() const {
    return m_registry && m_registry->valid(m_handle);
}

entt::entity Entity::GetHandle() const {
    return m_handle;
}

} // namespace ShatteredWilds::Core::ECS
