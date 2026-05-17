// ============================================================================
// Dionite — Utils: file helpers
// ============================================================================
#pragma once
#include <string>
#include <fstream>
#include <sstream>

namespace dionite::core {

class FileUtils {
public:
    static bool readText(const std::string& path, std::string& out) {
        std::ifstream f(path);
        if (!f) return false;
        std::stringstream ss; ss << f.rdbuf();
        out = ss.str();
        return true;
    }

    static bool writeText(const std::string& path, const std::string& text) {
        std::ofstream f(path);
        if (!f) return false;
        f << text;
        return true;
    }

    static bool exists(const std::string& path) {
        std::ifstream f(path);
        return f.good();
    }
};

} // namespace dionite::core
