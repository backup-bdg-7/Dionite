// ============================================================================
// Dionite — Math: Vec2 / Vec3 / Vec4 (lightweight, no GLM dependency)
// ============================================================================
#pragma once
#include <cmath>
#include <ostream>

namespace dionite::math {

struct Vec2 {
    float x, y;
    Vec2(float x_=0.f, float y_=0.f) : x(x_), y(y_) {}
    Vec2 operator+(const Vec2& o) const { return {x+o.x, y+o.y}; }
    Vec2 operator-(const Vec2& o) const { return {x-o.x, y-o.y}; }
    Vec2 operator*(float s) const { return {x*s, y*s}; }
    Vec2 operator/(float s) const { return {x/s, y/s}; }
    Vec2& operator+=(const Vec2& o) { x+=o.x; y+=o.y; return *this; }
    Vec2& operator-=(const Vec2& o) { x-=o.x; y-=o.y; return *this; }
    float length() const { return std::sqrt(x*x + y*y); }
    float lengthSq() const { return x*x + y*y; }
    Vec2 normalized() const { float l = length(); return l > 1e-6f ? *this / l : Vec2(0,0); }
    float dot(const Vec2& o) const { return x*o.x + y*o.y; }
    static Vec2 lerp(const Vec2& a, const Vec2& b, float t) { return a + (b - a) * t; }
};

struct Vec3 {
    float x, y, z;
    Vec3(float x_=0.f, float y_=0.f, float z_=0.f) : x(x_), y(y_), z(z_) {}
    Vec3 operator+(const Vec3& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vec3 operator-(const Vec3& o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vec3 operator*(float s) const { return {x*s, y*s, z*s}; }
    Vec3 operator/(float s) const { return {x/s, y/s, z/s}; }
    Vec3& operator+=(const Vec3& o) { x+=o.x; y+=o.y; z+=o.z; return *this; }
    float length() const { return std::sqrt(x*x + y*y + z*z); }
    float lengthSq() const { return x*x + y*y + z*z; }
    Vec3 normalized() const { float l = length(); return l > 1e-6f ? *this / l : Vec3(0,0,0); }
    float dot(const Vec3& o) const { return x*o.x + y*o.y + z*o.z; }
    Vec3 cross(const Vec3& o) const {
        return { y*o.z - z*o.y, z*o.x - x*o.z, x*o.y - y*o.x };
    }
    static Vec3 lerp(const Vec3& a, const Vec3& b, float t) { return a + (b - a) * t; }
};

struct Vec4 {
    float x, y, z, w;
    Vec4(float x_=0.f, float y_=0.f, float z_=0.f, float w_=0.f) : x(x_), y(y_), z(z_), w(w_) {}
};

inline std::ostream& operator<<(std::ostream& os, const Vec2& v) { return os << "(" << v.x << "," << v.y << ")"; }
inline std::ostream& operator<<(std::ostream& os, const Vec3& v) { return os << "(" << v.x << "," << v.y << "," << v.z << ")"; }

} // namespace dionite::math
