// ============================================================================
// Dionite — Game Systems: Quest system (campaign + side quests)
// ============================================================================
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace dionite::game {

enum class QuestState { Locked, Available, Active, Completed, Failed };

struct QuestObjective {
    std::string id;
    std::string description;
    int progress = 0;
    int target = 1;
    bool isComplete() const { return progress >= target; }
};

struct Quest {
    std::string id;
    std::string title;
    std::string lore;
    std::string biome;
    QuestState state = QuestState::Available;
    std::vector<QuestObjective> objectives;
    int rewardGold = 0;
    int rewardXP = 0;
    std::vector<std::string> rewardItems;
    bool allComplete() const {
        for (auto& o : objectives) if (!o.isComplete()) return false;
        return true;
    }
};

class QuestSystem {
public:
    void add(Quest q) { quests_[q.id] = std::move(q); }
    Quest* get(const std::string& id) {
        auto it = quests_.find(id);
        return it == quests_.end() ? nullptr : &it->second;
    }
    void progressObjective(const std::string& questId, const std::string& objId, int delta = 1) {
        auto* q = get(questId); if (!q) return;
        for (auto& o : q->objectives) if (o.id == objId) o.progress += delta;
        if (q->allComplete()) q->state = QuestState::Completed;
    }
    std::vector<Quest*> active() {
        std::vector<Quest*> v;
        for (auto& [id, q] : quests_) if (q.state == QuestState::Active) v.push_back(&q);
        return v;
    }
private:
    std::unordered_map<std::string, Quest> quests_;
};

} // namespace dionite::game
