// DecalSystem.h — Projected textures for blood splatters, scorch marks, ice patches,
// footprints. Cuboid OBB projector → fragment shader samples decal texture in the
// projector's local UV space. Limited to 64 decals on-screen at once for mobile perf.
#pragma once
#include "../Core/Math/Vec3.h"
#include "../Core/Math/Vec4.h"
#include "../Core/Math/Mat4.h"
#include <string>
#include <vector>

namespace dionite::vfx {

enum class DecalLayer { Blood, Scorch, Ice, Footprint, Magic, Stain };

struct Decal {
    DecalLayer layer = DecalLayer::Blood;
    std::string texture;
    Mat4   projector;     // OBB transform, scale = decal half-size
    Vec4   tint{1,1,1,1};
    float  age       = 0.f;
    float  lifetime  = 30.f;   // seconds (blood lasts 30s, scorch 60s, etc.)
    float  fadeIn    = 0.05f;
    float  fadeOut   = 1.5f;
    bool   sticky    = false;  // glued to a moving entity (e.g., enemy with bleed)
    int    parentEntity = -1;
};

class DecalSystem {
public:
    static constexpr int kMaxDecals = 64;
    std::vector<Decal> decals;

    int  spawn(const Decal& d);
    void update(float dt);
    float currentAlpha(const Decal& d) const;
};

} // namespace dionite::vfx
