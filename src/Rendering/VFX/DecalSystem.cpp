#include "DecalSystem.h"
#include <algorithm>

namespace dionite::vfx {

int DecalSystem::spawn(const Decal& d) {
    if ((int)decals.size() >= kMaxDecals) {
        // Evict the oldest non-sticky decal.
        auto it = std::min_element(decals.begin(), decals.end(),
            [](const Decal& a, const Decal& b) {
                if (a.sticky != b.sticky) return !a.sticky;
                return a.age > b.age;
            });
        if (it != decals.end()) decals.erase(it);
    }
    decals.push_back(d);
    return (int)decals.size() - 1;
}

void DecalSystem::update(float dt) {
    for (size_t i = 0; i < decals.size(); ) {
        decals[i].age += dt;
        if (decals[i].age >= decals[i].lifetime) {
            decals[i] = decals.back();
            decals.pop_back();
        } else ++i;
    }
}

float DecalSystem::currentAlpha(const Decal& d) const {
    if (d.age < d.fadeIn)  return d.age / d.fadeIn;
    float remain = d.lifetime - d.age;
    if (remain < d.fadeOut) return remain / d.fadeOut;
    return 1.f;
}

} // namespace dionite::vfx
