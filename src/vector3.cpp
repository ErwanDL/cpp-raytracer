#include "vector3.hpp"
#include "math_utils.hpp"
#include <cmath>

Vector3::Vector3(){};
Vector3::Vector3(const Vector3 &v) : x(v.x), y(v.y), z(v.z){};
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z){};
Vector3::Vector3(float f) : x(f), y(f), z(f){};

Vector3 &Vector3::operator+=(const Vector3 &v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}
Vector3 &Vector3::operator-=(const Vector3 &v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}
Vector3 &Vector3::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
    return *this;
}
Vector3 &Vector3::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
    return *this;
}
Vector3 Vector3::operator-() const
{
    return Vector3(-x, -y, -z);
}

Vector3 operator+(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(v1.getX() + v2.getX(), v1.getY() + v2.getY(), v1.getZ() + v2.getZ());
}
Vector3 operator-(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(v1.getX() - v2.getX(), v1.getY() - v2.getY(), v1.getZ() - v2.getZ());
}
Vector3 operator*(const Vector3 &v, float f)
{
    return Vector3(v.getX() * f, v.getY() * f, v.getZ() * f);
}
Vector3 operator*(float f, const Vector3 &v)
{
    return v * f;
}
Vector3 operator/(const Vector3 &v, float f)
{
    return Vector3(v.getX() / f, v.getY() / f, v.getZ() / f);
}

float Vector3::lengthSquared() const
{
    return Math::sqr(x) + Math::sqr(y) + Math::sqr(z);
}

float Vector3::length() const
{
    return std::sqrt(lengthSquared());
}

float Vector3::normalize()
{
    const float l = Vector3::length();
    *this /= l;
    return l;
}

Vector3 Vector3::normalized() const
{
    Vector3 newV = Vector3(*this);
    newV.normalize();
    return newV;
}

float Vector3::dot(const Vector3 &other) const
{
    return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::cross(const Vector3 &other) const
{
    return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

float Vector3::getX() const { return x; };
float Vector3::getY() const { return y; };
float Vector3::getZ() const { return z; };

void Vector3::setX(float newX) { x = newX; };
void Vector3::setY(float newY) { y = newY; };
void Vector3::setZ(float newZ) { z = newZ; };