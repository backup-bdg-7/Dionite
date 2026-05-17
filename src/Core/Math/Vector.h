#pragma once

#include <cmath>

namespace ShatteredWilds::Core::Math {

struct Vector3 {
    float x, y, z;

    Vector3();
    Vector3(float x, float y, float z);

    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(float scalar) const;

    float Dot(const Vector3& other) const;
    Vector3 Cross(const Vector3& other) const;

    float Length() const;
    Vector3 Normalize() const;
};

} // namespace ShatteredWilds::Core::Math
