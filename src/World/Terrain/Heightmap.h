// ============================================================================
// Dionite — World: Heightmap generator (open-world outdoor zones).
//
// Layered FBM + ridge noise + erosion smoothing produces a heightmap that
// captures Diablo IV-style rolling terrain with mountains, valleys, and
// natural-looking flow. Output is a width × height grid of meters.
// ============================================================================
#pragma once
#include "Noise.h"
#include <vector>

namespace dionite::world {

struct TerrainConfig {
    int   resolution = 512;        // grid samples per side
    float worldSize  = 1024.f;     // world meters per side
    float maxHeight  = 80.f;
    float seaLevel   = 4.f;
    float mountainHeight = 60.f;
    float erosionPasses  = 3;
    float continentalScale = 1.f / 800.f;
    float detailScale      = 1.f / 60.f;
    float ridgeScale       = 1.f / 220.f;
    uint64_t seed = 0xBEEFBABEULL;
};

struct Heightmap {
    int   resolution;
    float worldSize;
    float maxHeight;
    std::vector<float> data; // length = res*res, in meters

    float at(int x, int y) const {
        x = std::max(0, std::min(resolution - 1, x));
        y = std::max(0, std::min(resolution - 1, y));
        return data[y * resolution + x];
    }
    float& mut(int x, int y) { return data[y * resolution + x]; }

    // Convert grid index (x,y) to world (X, Z) — Y is height
    void worldXY(int gx, int gy, float& outX, float& outZ) const {
        outX = (gx / (float)(resolution - 1)) * worldSize - worldSize * 0.5f;
        outZ = (gy / (float)(resolution - 1)) * worldSize - worldSize * 0.5f;
    }
};

class HeightmapGenerator {
public:
    explicit HeightmapGenerator(const TerrainConfig& c) : cfg_(c), noise_(c.seed) {}

    Heightmap generate() const {
        Heightmap h;
        h.resolution = cfg_.resolution;
        h.worldSize  = cfg_.worldSize;
        h.maxHeight  = cfg_.maxHeight;
        h.data.assign((size_t)cfg_.resolution * cfg_.resolution, 0.f);

        const int N = cfg_.resolution;
        for (int y = 0; y < N; ++y) {
            for (int x = 0; x < N; ++x) {
                float wx, wz; h.worldXY(x, y, wx, wz);
                // Continental shape — low-frequency warped FBM
                float cont = noise_.warped(wx * cfg_.continentalScale, wz * cfg_.continentalScale, 5);
                cont = (cont + 1.f) * 0.5f;            // [0,1]
                // Mountain ridges
                float ridge = noise_.ridge(wx * cfg_.ridgeScale, wz * cfg_.ridgeScale, 5);
                // Fine detail
                float det = noise_.fbm(wx * cfg_.detailScale, wz * cfg_.detailScale, 4) * 0.5f;
                // Blend: ridges only appear above 0.55 continental
                float ridgeMask = std::max(0.f, (cont - 0.55f) * 2.f);
                float height = cont * cfg_.maxHeight * 0.55f
                              + ridge * ridgeMask * cfg_.mountainHeight
                              + det * 6.f;
                // Sea floor floor — keep below sea level smooth
                if (height < cfg_.seaLevel - 3.f) height = (cfg_.seaLevel - 3.f) + height * 0.05f;
                h.mut(x, y) = height;
            }
        }
        // Thermal erosion smoothing
        for (int i = 0; i < (int)cfg_.erosionPasses; ++i) erodeOnce(h);
        return h;
    }

private:
    void erodeOnce(Heightmap& h) const {
        const int N = h.resolution;
        std::vector<float> out = h.data;
        for (int y = 1; y < N - 1; ++y) {
            for (int x = 1; x < N - 1; ++x) {
                float c = h.at(x, y);
                float avg = (h.at(x-1,y) + h.at(x+1,y) + h.at(x,y-1) + h.at(x,y+1)) * 0.25f;
                out[y * N + x] = c * 0.85f + avg * 0.15f;
            }
        }
        h.data = std::move(out);
    }
    TerrainConfig cfg_;
    Noise noise_;
};

} // namespace dionite::world
