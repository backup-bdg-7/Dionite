#include "AnimationSampler.h"
#include <algorithm>
#include <cmath>

namespace dionite::anim {

static float wrap(float t, float dur, LoopMode mode) {
    if (dur <= 0.f) return 0.f;
    switch (mode) {
        case LoopMode::Loop:     return std::fmod((std::fmod(t, dur) + dur), dur);
        case LoopMode::PingPong: {
            float k = std::fmod(t, dur * 2.f);
            if (k < 0) k += dur * 2.f;
            return k <= dur ? k : (dur * 2.f - k);
        }
        case LoopMode::Once:
        default: return std::min(std::max(t, 0.f), dur);
    }
}

template <typename T, typename Interp>
static T sampleTrack(const std::vector<Keyframe<T>>& keys, float t, T fallback, Interp interp) {
    if (keys.empty()) return fallback;
    if (t <= keys.front().t) return keys.front().value;
    if (t >= keys.back().t)  return keys.back().value;
    for (size_t i = 1; i < keys.size(); ++i) {
        if (t < keys[i].t) {
            float dt = keys[i].t - keys[i-1].t;
            float a  = dt > 0 ? (t - keys[i-1].t) / dt : 0.f;
            return interp(keys[i-1].value, keys[i].value, a);
        }
    }
    return keys.back().value;
}

static Vec3 lerp3(const Vec3& a, const Vec3& b, float t) {
    return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t };
}

static Vec4 slerp(Vec4 a, Vec4 b, float t) {
    float d = a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
    if (d < 0) { b.x=-b.x; b.y=-b.y; b.z=-b.z; b.w=-b.w; d=-d; }
    if (d > 0.9995f) {
        Vec4 r{ a.x+(b.x-a.x)*t, a.y+(b.y-a.y)*t, a.z+(b.z-a.z)*t, a.w+(b.w-a.w)*t };
        float n = std::sqrt(r.x*r.x+r.y*r.y+r.z*r.z+r.w*r.w);
        return n > 0 ? Vec4{r.x/n,r.y/n,r.z/n,r.w/n} : Vec4{0,0,0,1};
    }
    float th0 = std::acos(d), th = th0 * t;
    float s0  = std::sin(th0 - th) / std::sin(th0);
    float s1  = std::sin(th) / std::sin(th0);
    return { a.x*s0 + b.x*s1, a.y*s0 + b.y*s1, a.z*s0 + b.z*s1, a.w*s0 + b.w*s1 };
}

void AnimationSampler::sample(const AnimationClip& clip, float tSeconds, Skeleton& skel) {
    float t = wrap(tSeconds, clip.duration, clip.loop);
    for (const auto& tr : clip.tracks) {
        if (tr.boneIndex < 0 || tr.boneIndex >= (int)skel.bones.size()) continue;
        auto& bone = skel.bones[tr.boneIndex];
        bone.localTranslation = sampleTrack(tr.translation, t, bone.localTranslation, lerp3);
        bone.localRotation    = sampleTrack(tr.rotation,    t, bone.localRotation,    slerp);
        bone.localScale       = sampleTrack(tr.scale,       t, bone.localScale,       lerp3);
    }
}

void AnimationSampler::blend(const AnimationClip& a, float ta,
                             const AnimationClip& b, float tb,
                             float alpha, Skeleton& skel) {
    // Sample a, then blend b's pose in. We mutate skeleton bone-by-bone.
    sample(a, ta, skel);
    if (alpha <= 0.f) return;
    float tBw = wrap(tb, b.duration, b.loop);
    for (const auto& tr : b.tracks) {
        if (tr.boneIndex < 0 || tr.boneIndex >= (int)skel.bones.size()) continue;
        auto& bone = skel.bones[tr.boneIndex];
        bone.localTranslation = lerp3(bone.localTranslation,
            sampleTrack(tr.translation, tBw, bone.localTranslation, lerp3), alpha);
        bone.localRotation    = slerp(bone.localRotation,
            sampleTrack(tr.rotation,    tBw, bone.localRotation,    slerp), alpha);
        bone.localScale       = lerp3(bone.localScale,
            sampleTrack(tr.scale,       tBw, bone.localScale,       lerp3), alpha);
    }
}

void AnimationSampler::applyAdditive(const AnimationClip& add, float t, float w, Skeleton& skel) {
    float tw = wrap(t, add.duration, add.loop);
    for (const auto& tr : add.tracks) {
        if (tr.boneIndex < 0 || tr.boneIndex >= (int)skel.bones.size()) continue;
        auto& bone = skel.bones[tr.boneIndex];
        Vec3 dt = sampleTrack(tr.translation, tw, Vec3{0,0,0}, lerp3);
        Vec4 dr = sampleTrack(tr.rotation,    tw, Vec4{0,0,0,1}, slerp);
        bone.localTranslation.x += dt.x * w;
        bone.localTranslation.y += dt.y * w;
        bone.localTranslation.z += dt.z * w;
        bone.localRotation = slerp(bone.localRotation, dr, w);
    }
}

} // namespace dionite::anim
