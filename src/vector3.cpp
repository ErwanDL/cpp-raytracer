#include "vector3.hpp"
#include "utils.hpp"
#include <cmath>
#include <iostream>

Vector3& Vector3::operator+=(const Vector3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}
Vector3& Vector3::operator-=(const Vector3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}
Vector3& Vector3::operator*=(float f) {
    x *= f;
    y *= f;
    z *= f;
    return *this;
}
Vector3& Vector3::operator/=(float f) {
    x /= f;
    y /= f;
    z /= f;
    return *this;
}

Vector3 Vector3::operator-() const { return Vector3(-x, -y, -z); }

bool Vector3::operator==(const Vector3& other) const {
    return Utils::floatingPointEquality(x, other.x) && Utils::floatingPointEquality(y, other.y) &&
           Utils::floatingPointEquality(z, other.z);
}

Vector3 operator+(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
Vector3 operator-(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
Vector3 operator*(const Vector3& v, float f) { return Vector3(v.x * f, v.y * f, v.z * f); }
Vector3 operator*(float f, const Vector3& v) { return Vector3(v.x * f, v.y * f, v.z * f); }
Vector3 operator/(const Vector3& v, float f) { return Vector3(v.x / f, v.y / f, v.z / f); }

float Vector3::lengthSquared() const { return Utils::sqr(x) + Utils::sqr(y) + Utils::sqr(z); }

float Vector3::length() const { return std::sqrt(lengthSquared()); }

void Vector3::normalize() {
    const float l = length();
    *this /= l;
}

Vector3 Vector3::normalized() const {
    Vector3 v{*this};
    v.normalize();
    return v;
}

float Vector3::dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }

Vector3 Vector3::cross(const Vector3& other) const {
    return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

/* Produces a reflection like this :
https://docs.unity3d.com/StaticFiles/ScriptRefImages/Vec3ReflectDiagram.png */
Vector3 Vector3::reflected(const Vector3& normal) const {
    auto v = normal.normalized();
    return *this - 2 * dot(v) * v;
}
