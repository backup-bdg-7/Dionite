// ============================================================================
// Dionite — UI: base screen + screen stack
// ============================================================================
#pragma once
#include <vector>
#include <memory>
#include <string>

namespace dionite::ui {

class UIScreen {
public:
    virtual ~UIScreen() = default;
    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void update(float dt) = 0;
    virtual void render() = 0;
    virtual bool blocksGameplay() const { return false; }
    virtual std::string id() const = 0;
};

class ScreenStack {
public:
    void push(std::shared_ptr<UIScreen> s) {
        if (!stack_.empty()) stack_.back()->onExit();
        s->onEnter();
        stack_.push_back(std::move(s));
    }
    void pop() {
        if (stack_.empty()) return;
        stack_.back()->onExit();
        stack_.pop_back();
        if (!stack_.empty()) stack_.back()->onEnter();
    }
    UIScreen* top() { return stack_.empty() ? nullptr : stack_.back().get(); }
    bool gameplayBlocked() const {
        for (auto it = stack_.rbegin(); it != stack_.rend(); ++it)
            if ((*it)->blocksGameplay()) return true;
        return false;
    }
    void update(float dt) { if (!stack_.empty()) stack_.back()->update(dt); }
    void render() { for (auto& s : stack_) s->render(); }
private:
    std::vector<std::shared_ptr<UIScreen>> stack_;
};

} // namespace dionite::ui
