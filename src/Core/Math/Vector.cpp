#include "Vector.h"

namespace ShatteredWilds::Core::Math {

Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

Vector3 Vector3::operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
}

float Vector3::Dot(const Vector3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::Cross(const Vector3& other) const {
    return Vector3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

float Vector3::Length() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::Normalize() const {
    float len = Length();
    if (len > 0.0f) {
        return Vector3(x / len, y / len, z / len);
    }
    return Vector3(0.0f, 0.0f, 0.0f);
}

} // namespace ShatteredWilds::Core::Math
