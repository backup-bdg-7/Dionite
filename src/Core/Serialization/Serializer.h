// ============================================================================
// Dionite — Serialization: save/load player profile via JSON blobs
// ============================================================================
#pragma once
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

namespace dionite::core {

class Serializer {
public:
    static bool writeJson(const std::string& path, const nlohmann::json& j) {
        std::ofstream f(path);
        if (!f) return false;
        f << j.dump(2);
        return true;
    }

    static bool readJson(const std::string& path, nlohmann::json& out) {
        std::ifstream f(path);
        if (!f) return false;
        try { f >> out; } catch (...) { return false; }
        return true;
    }
};

} // namespace dionite::core
