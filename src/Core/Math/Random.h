// ============================================================================
// Dionite — Math: Seeded random utilities (deterministic for dungeons)
// ============================================================================
#pragma once
#include <cstdint>

namespace dionite::math {

class Random {
public:
    explicit Random(uint64_t seed = 0xC0FFEE12345ULL) : state_(seed ? seed : 1) {}
    void seed(uint64_t s) { state_ = s ? s : 1; }

    uint64_t next() {
        // SplitMix64
        uint64_t z = (state_ += 0x9E3779B97F4A7C15ULL);
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31);
    }

    int range(int min, int maxIncl) {
        uint64_t span = (uint64_t)(maxIncl - min + 1);
        return min + (int)(next() % span);
    }

    float unit() { return (next() >> 11) * (1.0f / 9007199254740992.0f); }
    float range01() { return unit(); }
    float rangeF(float a, float b) { return a + (b - a) * unit(); }
    bool chance(float p) { return unit() < p; }

private:
    uint64_t state_;
};

} // namespace dionite::math
