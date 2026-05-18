#include "TrailRenderer.h"
#include <algorithm>

namespace dionite::vfx {

int TrailRenderer::addRibbon(const TrailRibbon& r) {
    ribbons.push_back(r);
    return (int)ribbons.size() - 1;
}

void TrailRenderer::pushPoint(int id, const Vec3& p) {
    if (id < 0 || id >= (int)ribbons.size()) return;
    TrailRibbon& r = ribbons[id];
    TrailPoint pt; pt.position = p; pt.born = r.elapsed; pt.color = r.startColor;
    r.points.push_back(pt);
    while ((int)r.points.size() > r.maxPoints) r.points.erase(r.points.begin());
}

void TrailRenderer::update(float dt) {
    for (auto& r : ribbons) {
        r.elapsed += dt;
        // Cull aged points.
        r.points.erase(std::remove_if(r.points.begin(), r.points.end(),
            [&](const TrailPoint& p) { return (r.elapsed - p.born) > r.pointLifetime; }),
            r.points.end());
        // Lerp color toward endColor based on age.
        for (auto& p : r.points) {
            float t = (r.elapsed - p.born) / std::max(0.0001f, r.pointLifetime);
            p.color = {
                r.startColor.x + (r.endColor.x - r.startColor.x) * t,
                r.startColor.y + (r.endColor.y - r.startColor.y) * t,
                r.startColor.z + (r.endColor.z - r.startColor.z) * t,
                r.startColor.w + (r.endColor.w - r.startColor.w) * t,
            };
        }
    }
}

} // namespace dionite::vfx
