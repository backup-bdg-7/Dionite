// ============================================================================
// Dionite — Logging: lightweight tagged logger (spdlog-style API)
// ============================================================================
#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <mutex>
#include <chrono>
#include <iomanip>

namespace dionite::core {

enum class LogLevel { Trace, Debug, Info, Warn, Error, Critical };

class Logger {
public:
    static Logger& instance() { static Logger l; return l; }

    void setLevel(LogLevel l) { level_ = l; }

    template <typename... Args>
    void log(LogLevel level, const std::string& tag, Args&&... args) {
        if ((int)level < (int)level_) return;
        std::ostringstream ss;
        (ss << ... << args);
        std::lock_guard<std::mutex> lk(mu_);
        auto t  = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm tm; localtime_r(&t, &tm);
        std::cout << "[" << std::put_time(&tm, "%H:%M:%S") << "] "
                  << "[" << levelStr(level) << "] "
                  << "[" << tag << "] "
                  << ss.str() << std::endl;
    }

private:
    LogLevel level_ = LogLevel::Info;
    std::mutex mu_;
    static const char* levelStr(LogLevel l) {
        switch (l) {
            case LogLevel::Trace: return "TRACE";
            case LogLevel::Debug: return "DEBUG";
            case LogLevel::Info: return "INFO ";
            case LogLevel::Warn: return "WARN ";
            case LogLevel::Error: return "ERROR";
            case LogLevel::Critical: return "CRIT ";
        }
        return "?";
    }
};

#define DLOG_INFO(tag, ...)  ::dionite::core::Logger::instance().log(::dionite::core::LogLevel::Info,  tag, __VA_ARGS__)
#define DLOG_WARN(tag, ...)  ::dionite::core::Logger::instance().log(::dionite::core::LogLevel::Warn,  tag, __VA_ARGS__)
#define DLOG_ERR(tag,  ...)  ::dionite::core::Logger::instance().log(::dionite::core::LogLevel::Error, tag, __VA_ARGS__)
#define DLOG_DBG(tag,  ...)  ::dionite::core::Logger::instance().log(::dionite::core::LogLevel::Debug, tag, __VA_ARGS__)

} // namespace dionite::core
