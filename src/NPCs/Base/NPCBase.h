// ============================================================================
// Dionite — NPCs: Base NPC interface
// ============================================================================
#pragma once
#include "Core/Math/Vector.h"
#include <string>
#include <vector>

namespace dionite::npc {

enum class NPCKind { Blacksmith, Jeweler, TravelingMage, GeneralMerchant, QuestGiver };

struct DialogueLine {
    std::string speaker;
    std::string text;
};

class NPCBase {
public:
    virtual ~NPCBase() = default;
    NPCBase(std::string id, std::string name, NPCKind kind) : id_(std::move(id)), name_(std::move(name)), kind_(kind) {}

    const std::string& id() const { return id_; }
    const std::string& name() const { return name_; }
    NPCKind kind() const { return kind_; }

    math::Vec3 position;
    std::vector<DialogueLine> greetingLines;

    virtual std::string greet() {
        if (greetingLines.empty()) return name_ + ": Welcome, wanderer.";
        const auto& l = greetingLines[std::rand() % greetingLines.size()];
        return l.speaker + ": " + l.text;
    }

protected:
    std::string id_;
    std::string name_;
    NPCKind kind_;
};

} // namespace dionite::npc
