#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace ShatteredWilds::Core::Logging {

class Logger {
public:
    static void Initialize();
    static void Shutdown();

    static std::shared_ptr<spdlog::logger>& GetCoreLogger();
    static std::shared_ptr<spdlog::logger>& GetGameLogger();

private:
    static std::shared_ptr<spdlog::logger> s_coreLogger;
    static std::shared_ptr<spdlog::logger> s_gameLogger;
};

} // namespace ShatteredWilds::Core::Logging
