// ============================================================================
// Dionite — Config: JSON-backed configuration manager
// ============================================================================
#pragma once
#include <string>
#include <unordered_map>
#include <variant>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

namespace dionite::core {

class ConfigManager {
public:
    static ConfigManager& instance() { static ConfigManager c; return c; }

    bool load(const std::string& path) {
        std::ifstream f(path);
        if (!f) return false;
        try { f >> data_; } catch (...) { return false; }
        path_ = path;
        return true;
    }

    bool save() const {
        if (path_.empty()) return false;
        std::ofstream f(path_);
        if (!f) return false;
        f << data_.dump(2);
        return true;
    }

    template <typename T>
    T get(const std::string& key, T def) const {
        try { return data_.value(key, def); } catch (...) { return def; }
    }

    nlohmann::json& json() { return data_; }
    const nlohmann::json& json() const { return data_; }

private:
    nlohmann::json data_;
    std::string path_;
};

} // namespace dionite::core
