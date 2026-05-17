// ============================================================================
// Dionite / Shattered Wilds — Core ECS: Entity
// ============================================================================
#pragma once
#include <cstdint>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>

namespace dionite::core {

using EntityID = uint64_t;
constexpr EntityID INVALID_ENTITY = 0;

class Component;

class Entity {
public:
    explicit Entity(EntityID id) : id_(id), alive_(true) {}
    ~Entity() = default;

    EntityID id() const { return id_; }
    bool alive() const { return alive_; }
    void kill() { alive_ = false; }

    template <typename T, typename... Args>
    T* addComponent(Args&&... args) {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
        auto comp = std::make_shared<T>(std::forward<Args>(args)...);
        comp->setOwner(this);
        T* raw = comp.get();
        components_[std::type_index(typeid(T))] = std::move(comp);
        return raw;
    }

    template <typename T>
    T* getComponent() const {
        auto it = components_.find(std::type_index(typeid(T)));
        if (it == components_.end()) return nullptr;
        return static_cast<T*>(it->second.get());
    }

    template <typename T>
    bool hasComponent() const {
        return components_.find(std::type_index(typeid(T))) != components_.end();
    }

    template <typename T>
    void removeComponent() {
        components_.erase(std::type_index(typeid(T)));
    }

    const std::unordered_map<std::type_index, std::shared_ptr<Component>>& components() const {
        return components_;
    }

private:
    EntityID id_;
    bool alive_;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components_;
};

} // namespace dionite::core
