// ============================================================================
// Dionite — Math: AABB / collision primitives
// ============================================================================
#pragma once
#include "Vector.h"
#include <algorithm>

namespace dionite::math {

struct AABB {
    Vec3 min, max;
    AABB() = default;
    AABB(const Vec3& a, const Vec3& b) : min(a), max(b) {}

    bool contains(const Vec3& p) const {
        return p.x >= min.x && p.x <= max.x &&
               p.y >= min.y && p.y <= max.y &&
               p.z >= min.z && p.z <= max.z;
    }
    bool intersects(const AABB& o) const {
        return min.x <= o.max.x && max.x >= o.min.x &&
               min.y <= o.max.y && max.y >= o.min.y &&
               min.z <= o.max.z && max.z >= o.min.z;
    }
    Vec3 center() const { return (min + max) * 0.5f; }
    Vec3 extents() const { return (max - min) * 0.5f; }
    void expand(const Vec3& p) {
        min = { std::min(min.x, p.x), std::min(min.y, p.y), std::min(min.z, p.z) };
        max = { std::max(max.x, p.x), std::max(max.y, p.y), std::max(max.z, p.z) };
    }
};

inline bool circleCircle(const Vec2& a, float ar, const Vec2& b, float br) {
    float r = ar + br; return (a - b).lengthSq() <= r * r;
}

} // namespace dionite::math
