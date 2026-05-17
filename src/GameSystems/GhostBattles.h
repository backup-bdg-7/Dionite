// ============================================================================
// Dionite — Game Systems: Ghost Battles (replay other players' best Spire runs)
// ============================================================================
#pragma once
#include "Core/Math/Vector.h"
#include <vector>
#include <string>

namespace dionite::game {

struct GhostFrame {
    float t;
    math::Vec3 pos;
    math::Vec3 aim;
    bool firing;
};

struct GhostRecord {
    std::string playerId;
    std::string playerName;
    int floor;
    int score;
    std::vector<GhostFrame> frames; // 60 Hz capture
};

class GhostBattleSystem {
public:
    void recordFrame(GhostFrame f) { recording_.frames.push_back(std::move(f)); }
    GhostRecord finalize(const std::string& playerId, const std::string& name, int floor, int score) {
        recording_.playerId = playerId; recording_.playerName = name;
        recording_.floor = floor; recording_.score = score;
        auto r = recording_;
        recording_ = {};
        return r;
    }

    // Sample interpolated ghost position at time t.
    bool sampleGhost(const GhostRecord& ghost, float t, math::Vec3& pos, math::Vec3& aim, bool& firing) {
        if (ghost.frames.empty()) return false;
        if (t <= ghost.frames.front().t) { pos = ghost.frames.front().pos; aim = ghost.frames.front().aim; firing = ghost.frames.front().firing; return true; }
        if (t >= ghost.frames.back().t) { pos = ghost.frames.back().pos; aim = ghost.frames.back().aim; firing = ghost.frames.back().firing; return true; }
        for (size_t i = 1; i < ghost.frames.size(); ++i) {
            if (ghost.frames[i].t >= t) {
                const auto& a = ghost.frames[i - 1];
                const auto& b = ghost.frames[i];
                float u = (t - a.t) / std::max(1e-4f, b.t - a.t);
                pos = math::Vec3::lerp(a.pos, b.pos, u);
                aim = math::Vec3::lerp(a.aim, b.aim, u);
                firing = (u < 0.5f) ? a.firing : b.firing;
                return true;
            }
        }
        return false;
    }

private:
    GhostRecord recording_;
};

} // namespace dionite::game
