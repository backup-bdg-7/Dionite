// ============================================================================
// Dionite — NPC: Dialogue system (branching, state-machine driven)
// ============================================================================
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace dionite::npc {

struct DialogueChoice {
    std::string text;
    std::string next; // next node id, or "end"
    std::function<void()> action;
};

struct DialogueNode {
    std::string id;
    std::string speaker;
    std::string text;
    std::vector<DialogueChoice> choices;
};

class DialogueGraph {
public:
    void addNode(DialogueNode n) { nodes_[n.id] = std::move(n); }

    const DialogueNode* node(const std::string& id) const {
        auto it = nodes_.find(id);
        return it == nodes_.end() ? nullptr : &it->second;
    }

private:
    std::unordered_map<std::string, DialogueNode> nodes_;
};

class DialogueRunner {
public:
    void start(const DialogueGraph& g, const std::string& startId) {
        graph_ = &g; current_ = startId;
    }
    const DialogueNode* current() const { return graph_ ? graph_->node(current_) : nullptr; }
    bool choose(int choiceIdx) {
        const auto* n = current();
        if (!n || choiceIdx < 0 || choiceIdx >= (int)n->choices.size()) return false;
        if (n->choices[choiceIdx].action) n->choices[choiceIdx].action();
        if (n->choices[choiceIdx].next == "end") { graph_ = nullptr; return false; }
        current_ = n->choices[choiceIdx].next;
        return true;
    }
private:
    const DialogueGraph* graph_ = nullptr;
    std::string current_;
};

} // namespace dionite::npc
