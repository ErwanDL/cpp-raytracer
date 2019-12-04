#include "vectors.hpp"
#include <cmath>
#include <iostream>
#include "math_utils.hpp"

float IVector::length() const { return std::sqrt(lengthSquared()); }

// CLASS VECTOR3

Vector3::Vector3() {}
Vector3::Vector3(const Vector3 &v) : x(v.x), y(v.y), z(v.z) {}
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
Vector3::Vector3(float f) : x(f), y(f), z(f) {}

Vector3 &Vector3::operator+=(const Vector3 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}
Vector3 &Vector3::operator-=(const Vector3 &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}
Vector3 &Vector3::operator*=(float f) {
    x *= f;
    y *= f;
    z *= f;
    return *this;
}
Vector3 &Vector3::operator/=(float f) {
    x /= f;
    y /= f;
    z /= f;
    return *this;
}
Vector3 Vector3::operator-() const { return Vector3(-x, -y, -z); }

Vector3 operator+(const Vector3 &v1, const Vector3 &v2) {
    return Vector3(v1.getX() + v2.getX(), v1.getY() + v2.getY(),
                   v1.getZ() + v2.getZ());
}
Vector3 operator-(const Vector3 &v1, const Vector3 &v2) {
    return Vector3(v1.getX() - v2.getX(), v1.getY() - v2.getY(),
                   v1.getZ() - v2.getZ());
}
Vector3 operator*(const Vector3 &v, float f) {
    return Vector3(v.getX() * f, v.getY() * f, v.getZ() * f);
}
Vector3 operator*(float f, const Vector3 &v) {
    return Vector3(v.getX() * f, v.getY() * f, v.getZ() * f);
}
Vector3 operator/(const Vector3 &v, float f) {
    return Vector3(v.getX() / f, v.getY() / f, v.getZ() / f);
}

std::ostream &operator<<(std::ostream &out, const Vector3 &v) {
    out << "Vector3(" << v.getX() << ", " << v.getY() << ", " << v.getZ()
        << ')';
    return out;
}

float Vector3::lengthSquared() const {
    return Math::sqr(x) + Math::sqr(y) + Math::sqr(z);
}

float Vector3::normalize() {
    const float l = length();
    *this /= l;
    return l;
}

Vector3 Vector3::normalized() const {
    Vector3 v{*this};
    v.normalize();
    return v;
}

float Vector3::dot(const Vector3 &other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::cross(const Vector3 &other) const {
    return Vector3(y * other.z - z * other.y, z * other.x - x * other.z,
                   x * other.y - y * other.x);
}

float Vector3::getX() const { return x; }
float Vector3::getY() const { return y; }
float Vector3::getZ() const { return z; }

void Vector3::setX(float newX) { x = newX; }
void Vector3::setY(float newY) { y = newY; }
void Vector3::setZ(float newZ) { z = newZ; }

// CLASS VECTOR2

Vector2::Vector2() {}
Vector2::Vector2(const Vector2 &other) : u(other.u), v(other.v) {}
Vector2::Vector2(float u, float v) : u(u), v(v) {}
Vector2::Vector2(float f) : u(f), v(f) {}

float Vector2::lengthSquared() const { return Math::sqr(u) + Math::sqr(v); }

float Vector2::normalize() {
    const float l = length();
    *this /= l;
    return l;
}

Vector2 Vector2::normalized() const {
    Vector2 v{*this};
    v.normalize();
    return v;
}

Vector2 &Vector2::operator+=(const Vector2 &other) {
    u += other.u;
    v += other.v;
    return *this;
}
Vector2 &Vector2::operator-=(const Vector2 &other) {
    u -= other.u;
    v -= other.v;
    return *this;
}
Vector2 &Vector2::operator*=(float f) {
    u *= f;
    v *= f;
    return *this;
}
Vector2 &Vector2::operator/=(float f) {
    u /= f;
    v /= f;
    return *this;
}

float Vector2::getU() const { return u; }
float Vector2::getV() const { return v; }

void Vector2::setU(float newU) { u = newU; }
void Vector2::setV(float newV) { v = newV; }