// ============================================================================
// Dionite — World: Procedural noise primitives.
//   Hashed value noise + fractal Brownian motion (FBM).
//   Deterministic per seed; safe for cross-platform reproducibility
//   (no <random> floats, no platform float divergence).
// ============================================================================
#pragma once
#include <cstdint>
#include <cmath>

namespace dionite::world {

class Noise {
public:
    explicit Noise(uint64_t seed = 1337) : seed_(seed) {}

    // Smooth value noise in [-1, 1].
    float value(float x, float y) const {
        int x0 = (int)std::floor(x), y0 = (int)std::floor(y);
        float fx = x - x0, fy = y - y0;
        float v00 = hash(x0,     y0    );
        float v10 = hash(x0 + 1, y0    );
        float v01 = hash(x0,     y0 + 1);
        float v11 = hash(x0 + 1, y0 + 1);
        float ux = smoothstep(fx);
        float uy = smoothstep(fy);
        float a = lerp(v00, v10, ux);
        float b = lerp(v01, v11, ux);
        return lerp(a, b, uy);
    }

    // FBM: layered noise; returns ~[-1, 1].
    float fbm(float x, float y, int octaves = 6, float lacunarity = 2.f, float gain = 0.5f) const {
        float amp = 1.f, freq = 1.f, sum = 0.f, norm = 0.f;
        for (int i = 0; i < octaves; ++i) {
            sum  += value(x * freq, y * freq) * amp;
            norm += amp;
            amp  *= gain;
            freq *= lacunarity;
        }
        return sum / std::max(0.0001f, norm);
    }

    // Ridge noise (mountain ridges). Returns [0, 1].
    float ridge(float x, float y, int octaves = 5) const {
        float n = 1.f - std::abs(fbm(x, y, octaves));
        return n * n;
    }

    // Domain-warped FBM (for natural terrain).
    float warped(float x, float y, int octaves = 5, float warpAmp = 4.f) const {
        float qx = fbm(x + 1.7f, y + 9.2f, 3);
        float qy = fbm(x + 8.3f, y + 2.8f, 3);
        return fbm(x + warpAmp * qx, y + warpAmp * qy, octaves);
    }

private:
    static float smoothstep(float t) { return t * t * (3.f - 2.f * t); }
    static float lerp(float a, float b, float t) { return a + (b - a) * t; }

    float hash(int x, int y) const {
        uint64_t h = (uint64_t)(x * 374761393) ^ (uint64_t)(y * 668265263) ^ seed_;
        h = (h ^ (h >> 13)) * 1274126177u;
        h = h ^ (h >> 16);
        return ((h & 0xFFFFFF) / float(0x800000)) - 1.f;
    }

    uint64_t seed_;
};

} // namespace dionite::world
