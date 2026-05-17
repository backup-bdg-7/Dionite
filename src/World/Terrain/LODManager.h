// ============================================================================
// Dionite — World: LOD manager (distance-based mesh swap for terrain tiles
// and foliage instances). Each tile has 3 LODs (full / half / quarter density).
// ============================================================================
#pragma once
#include "TerrainMeshBuilder.h"
#include "FoliageScatter.h"
#include "Core/Math/Vector.h"
#include <vector>
#include <unordered_map>

namespace dionite::world {

enum class LODLevel : uint8_t { Lod0 = 0, Lod1 = 1, Lod2 = 2, Culled = 3 };

struct LODConfig {
    float lod0Distance = 80.f;
    float lod1Distance = 200.f;
    float lod2Distance = 400.f;
    float cullDistance = 800.f;
};

struct TerrainTileLOD {
    int chunkX, chunkZ;
    render::Mesh lod0;
    render::Mesh lod1;
    render::Mesh lod2;
};

class LODManager {
public:
    explicit LODManager(const LODConfig& cfg = {}) : cfg_(cfg) {}

    LODLevel select(const math::Vec3& tileCenter, const math::Vec3& cam) const {
        float d = (tileCenter - cam).length();
        if (d > cfg_.cullDistance) return LODLevel::Culled;
        if (d > cfg_.lod2Distance) return LODLevel::Lod2;
        if (d > cfg_.lod1Distance) return LODLevel::Lod1;
        return LODLevel::Lod0;
    }

    // Build 3 LODs of a tile by skipping every Nth vertex.
    static TerrainTileLOD buildLODs(const TerrainTile& t) {
        TerrainTileLOD out{ t.chunkX, t.chunkZ, t.mesh, decimate(t.mesh, 2), decimate(t.mesh, 4) };
        return out;
    }

    // Foliage culling — keep only instances within cull distance and apply density falloff.
    static std::vector<FoliageInstance>
    filterFoliage(const std::vector<FoliageInstance>& all, const math::Vec3& cam, const LODConfig& cfg) {
        std::vector<FoliageInstance> keep;
        keep.reserve(all.size());
        for (auto& f : all) {
            float d = (f.position - cam).length();
            if (d > cfg.cullDistance) continue;
            if (d > cfg.lod1Distance) {
                // Half density beyond lod1
                if (((size_t)(f.position.x * 13 + f.position.z * 7) & 1) != 0) continue;
            }
            if (d > cfg.lod2Distance) {
                // Quarter density beyond lod2
                if (((size_t)(f.position.x * 11 + f.position.z * 5) & 3) != 0) continue;
            }
            keep.push_back(f);
        }
        return keep;
    }

private:
    // Naive vertex-stride decimation. Real impl would use mesh simplification (QEM).
    static render::Mesh decimate(const render::Mesh& src, int stride) {
        render::Mesh out;
        int N = 0;
        // Find original grid width by looking for first index jump
        for (size_t i = 0; i < src.indices.size(); i += 3) {
            uint32_t a = src.indices[i], b = src.indices[i + 1];
            if ((int)(b - a) > 1) { N = (int)(b - a); break; }
        }
        if (N <= 0) return src;
        int H = (int)(src.vertices.size() / N);
        for (int y = 0; y < H; y += stride)
            for (int x = 0; x < N; x += stride)
                out.vertices.push_back(src.vertices[y * N + x]);
        int W2 = (N + stride - 1) / stride;
        int H2 = (H + stride - 1) / stride;
        for (int y = 0; y < H2 - 1; ++y) {
            for (int x = 0; x < W2 - 1; ++x) {
                uint32_t a = y * W2 + x;
                uint32_t b = a + 1;
                uint32_t c = a + W2;
                uint32_t d = c + 1;
                out.indices.push_back(a); out.indices.push_back(c); out.indices.push_back(b);
                out.indices.push_back(b); out.indices.push_back(c); out.indices.push_back(d);
            }
        }
        return out;
    }

    LODConfig cfg_;
};

} // namespace dionite::world
