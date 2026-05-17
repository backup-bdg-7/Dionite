#include "Logger.h"

namespace ShatteredWilds::Core::Logging {

std::shared_ptr<spdlog::logger> Logger::s_coreLogger;
std::shared_ptr<spdlog::logger> Logger::s_gameLogger;

void Logger::Initialize() {
    // Create console sink with color
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern("[%H:%M:%S %z] [%n] [%^%l%$] %v");

    // Create file sink for persistent logs
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/shattered_wilds.log", true);
    file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v");

    // Create core logger
    s_coreLogger = std::make_shared<spdlog::logger>("Core", spdlog::sinks_init_list{console_sink, file_sink});
    s_coreLogger->set_level(spdlog::level::info);
    spdlog::register_logger(s_coreLogger);

    // Create game logger
    s_gameLogger = std::make_shared<spdlog::logger>("Game", spdlog::sinks_init_list{console_sink, file_sink});
    s_gameLogger->set_level(spdlog::level::info);
    spdlog::register_logger(s_gameLogger);
}

void Logger::Shutdown() {
    spdlog::shutdown();
}

std::shared_ptr<spdlog::logger>& Logger::GetCoreLogger() {
    return s_coreLogger;
}

std::shared_ptr<spdlog::logger>& Logger::GetGameLogger() {
    return s_gameLogger;
}

} // namespace ShatteredWilds::Core::Logging
