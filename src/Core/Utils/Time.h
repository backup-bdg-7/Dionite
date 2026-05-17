// ============================================================================
// Dionite — Utils: high-resolution timer
// ============================================================================
#pragma once
#include <chrono>

namespace dionite::core {

class Time {
public:
    static double now() {
        using clock = std::chrono::high_resolution_clock;
        auto t = clock::now().time_since_epoch();
        return std::chrono::duration<double>(t).count();
    }
};

class FrameTimer {
public:
    FrameTimer() : last_(Time::now()) {}
    float tick() {
        double n = Time::now();
        float dt = static_cast<float>(n - last_);
        last_ = n;
        return dt;
    }
private:
    double last_;
};

} // namespace dionite::core
