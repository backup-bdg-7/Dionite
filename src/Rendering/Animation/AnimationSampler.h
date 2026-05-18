// AnimationSampler.h — evaluates an AnimationClip onto a Skeleton at a given time.
// Uses linear interpolation for T/S and SLERP for rotation. Looping is wrapped at duration.
#pragma once
#include "AnimationClip.h"
#include "Skeleton.h"

namespace dionite::anim {

class AnimationSampler {
public:
    // Sample `clip` at time `tSeconds` and write local TRS into `skel`.
    static void sample(const AnimationClip& clip, float tSeconds, Skeleton& skel);

    // Blend two clips (e.g., run-fwd + run-strafe) into the skeleton.
    // `alpha` 0 = a only, 1 = b only.
    static void blend(const AnimationClip& a, float ta,
                      const AnimationClip& b, float tb,
                      float alpha, Skeleton& skel);

    // Additive layer — useful for upper-body firing while lower-body runs.
    // Applies `additive` over the existing skeleton pose with weight in [0,1].
    static void applyAdditive(const AnimationClip& additive, float t, float weight, Skeleton& skel);
};

} // namespace dionite::anim
