// ============================================================================
// Dionite — UI: Login screen (email/password + Apple ID)
// ============================================================================
#pragma once
#include "UIScreen.h"
#include <string>
#include <functional>

namespace dionite::ui {

class LoginScreen : public UIScreen {
public:
    std::string email;
    std::string password;
    bool blocksGameplay() const override { return true; }
    std::string id() const override { return "login"; }
    void update(float) override {}
    void render() override {}

    std::function<void(const std::string&, const std::string&)> onLogin;
    std::function<void(const std::string&, const std::string&)> onRegister;
    std::function<void()> onAppleSignIn;
};

} // namespace dionite::ui
