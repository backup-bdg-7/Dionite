// ============================================================================
// Dionite / Shattered Wilds — Core ECS: Component base class
// ============================================================================
#pragma once

namespace dionite::core {

class Entity;

class Component {
public:
    virtual ~Component() = default;
    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void update(float /*dt*/) {}

    Entity* owner() const { return owner_; }
    void setOwner(Entity* o) { owner_ = o; }

protected:
    Entity* owner_ = nullptr;
};

} // namespace dionite::core
