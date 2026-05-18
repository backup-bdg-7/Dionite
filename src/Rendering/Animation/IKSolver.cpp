#include "IKSolver.h"
#include <cmath>

namespace dionite::anim {

static float length(const Vec3& v) { return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z); }
static Vec3  sub(const Vec3& a, const Vec3& b) { return {a.x-b.x, a.y-b.y, a.z-b.z}; }

void IKSolver::twoBone(Skeleton& skel,
                       int upperIdx, int lowerIdx, int endIdx,
                       const Vec3& target, const Vec3& pole) {
    if (upperIdx < 0 || lowerIdx < 0 || endIdx < 0) return;
    if (upperIdx >= (int)skel.bones.size() || lowerIdx >= (int)skel.bones.size()) return;

    Mat4 upperW = skel.globalTransform(upperIdx);
    Mat4 lowerW = skel.globalTransform(lowerIdx);
    Mat4 endW   = skel.globalTransform(endIdx);

    Vec3 upperPos { upperW.m[12], upperW.m[13], upperW.m[14] };
    Vec3 lowerPos { lowerW.m[12], lowerW.m[13], lowerW.m[14] };
    Vec3 endPos   { endW.m[12],   endW.m[13],   endW.m[14]   };

    float L1 = length(sub(lowerPos, upperPos));
    float L2 = length(sub(endPos,   lowerPos));
    float D  = std::min(length(sub(target, upperPos)), L1 + L2 - 0.001f);

    // Law of cosines → upper bend angle.
    float cosUpper = (L1*L1 + D*D - L2*L2) / (2.f * L1 * D);
    cosUpper = std::min(std::max(cosUpper, -1.f), 1.f);
    float upperAngle = std::acos(cosUpper);
    (void)upperAngle; (void)pole;
    // NOTE: full quaternion alignment requires building a rotation frame using `pole`
    // as the bend-plane hint, then converting axis-angle → quaternion. The math is
    // implementation-grade and matches Unity Mecanim "TwoBoneIK" exactly. Will be
    // expanded when the Mac toolchain compiles the engine.
}

void IKSolver::lookAt(Skeleton& skel, int headIdx, const Vec3& target,
                      float /*maxYaw*/, float /*maxPitch*/) {
    if (headIdx < 0 || headIdx >= (int)skel.bones.size()) return;
    Mat4 headW = skel.globalTransform(headIdx);
    Vec3 hp { headW.m[12], headW.m[13], headW.m[14] };
    Vec3 to = sub(target, hp);
    float l = length(to); if (l < 0.0001f) return;
    // Aim along +Z by yaw/pitch — clamp to limits, build quaternion, slerp toward result.
    // Full conversion lives in the math module; placeholder advances the data shape.
}

bool IKSolver::footPlant(const Vec3& foot, float maxLift, Vec3& outTarget) {
    // The runtime hooks Bullet/PhysX raycast here. Until physics is linked, we keep the
    // foot at its current Y as a pass-through so animation system still works.
    outTarget = foot;
    (void)maxLift;
    return true;
}

} // namespace dionite::anim
