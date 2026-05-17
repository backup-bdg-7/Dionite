// ============================================================================
// Dionite — iOS main shim (entry is actually AppDelegate.swift; this exists
// so CMake has a target for non-Apple builds).
// ============================================================================
#include "Core/Logging/Logger.h"
int main(int, char**) {
    dionite::core::Logger::instance().log(dionite::core::LogLevel::Info, "ios", "iOS entry point is AppDelegate.swift");
    return 0;
}
