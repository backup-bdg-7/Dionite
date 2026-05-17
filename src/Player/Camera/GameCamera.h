// ============================================================================
// Dionite — Player: third-person follow camera with smoothing + shake
// ============================================================================
#pragma once
#include "Core/Math/Vector.h"
#include "Core/Math/Matrix.h"

namespace dionite::player {

class GameCamera {
public:
    void follow(const math::Vec3& target, float dt) {
        math::Vec3 desired = target + offset_;
        position_ = math::Vec3::lerp(position_, desired, 1.f - std::exp(-smoothing_ * dt));
        lookAt_   = math::Vec3::lerp(lookAt_, target, 1.f - std::exp(-smoothing_ * dt));
        // tick shake
        if (shakeTime_ > 0.f) {
            shakeTime_ -= dt;
            float k = std::max(0.f, shakeTime_) * shakeIntensity_;
            shakeOffset_ = math::Vec3((std::rand()%200-100)*0.01f*k,
                                       (std::rand()%200-100)*0.01f*k,
                                       (std::rand()%200-100)*0.01f*k);
        } else {
            shakeOffset_ = {};
        }
    }

    math::Mat4 viewMatrix() const {
        return math::Mat4::lookAt(position_ + shakeOffset_, lookAt_ + shakeOffset_, {0,1,0});
    }

    void shake(float seconds, float intensity = 4.f) {
        shakeTime_ = std::max(shakeTime_, seconds);
        shakeIntensity_ = std::max(shakeIntensity_, intensity);
    }

    math::Vec3 position() const { return position_ + shakeOffset_; }

private:
    math::Vec3 position_{0, 12, -12};
    math::Vec3 lookAt_{0, 0, 0};
    math::Vec3 offset_{0, 14, -10};
    math::Vec3 shakeOffset_{};
    float smoothing_ = 8.f;
    float shakeTime_ = 0.f;
    float shakeIntensity_ = 0.f;
};

} // namespace dionite::player
