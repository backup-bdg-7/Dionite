// ============================================================================
// Dionite — World: chunk streaming for open-world (load/unload by camera radius)
// ============================================================================
#pragma once
#include "Core/Math/Vector.h"
#include <unordered_map>
#include <string>
#include <functional>
#include <vector>

namespace dionite::world {

struct ChunkCoord {
    int cx, cy;
    bool operator==(const ChunkCoord& o) const { return cx == o.cx && cy == o.cy; }
};

struct ChunkCoordHash {
    size_t operator()(const ChunkCoord& c) const noexcept {
        return std::hash<int>()(c.cx) ^ (std::hash<int>()(c.cy) << 1);
    }
};

struct Chunk {
    ChunkCoord coord;
    bool loaded = false;
    // (real impl: tile grid, props, spawned entities)
};

class StreamingManager {
public:
    using Loader = std::function<void(Chunk&)>;
    using Unloader = std::function<void(Chunk&)>;

    StreamingManager(float chunkSize, int radius)
        : chunkSize_(chunkSize), radius_(radius) {}

    void setLoader(Loader l) { loader_ = std::move(l); }
    void setUnloader(Unloader u) { unloader_ = std::move(u); }

    void update(const math::Vec3& camPos) {
        int ccx = (int)std::floor(camPos.x / chunkSize_);
        int ccy = (int)std::floor(camPos.z / chunkSize_);
        std::unordered_map<ChunkCoord, Chunk, ChunkCoordHash> next;
        for (int dy = -radius_; dy <= radius_; ++dy) {
            for (int dx = -radius_; dx <= radius_; ++dx) {
                ChunkCoord c{ccx + dx, ccy + dy};
                auto it = chunks_.find(c);
                if (it != chunks_.end()) {
                    next[c] = std::move(it->second);
                } else {
                    Chunk ch{c, false};
                    if (loader_) loader_(ch);
                    ch.loaded = true;
                    next[c] = std::move(ch);
                }
            }
        }
        for (auto& [coord, ch] : chunks_) {
            if (next.find(coord) == next.end() && unloader_) unloader_(ch);
        }
        chunks_ = std::move(next);
    }

    size_t loadedCount() const { return chunks_.size(); }

private:
    float chunkSize_;
    int radius_;
    std::unordered_map<ChunkCoord, Chunk, ChunkCoordHash> chunks_;
    Loader loader_;
    Unloader unloader_;
};

} // namespace dionite::world
