#ifndef VECTORS_HPP
#define VECTORS_HPP

#include "utils.hpp"
#include <cmath>
#include <iostream>

struct Vector3 {
    float x;
    float y;
    float z;

    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator*=(float f);
    Vector3& operator/=(float f);
    Vector3 operator-() const;
    bool operator==(const Vector3& other) const;

    float lengthSquared() const;
    float length() const;
    void normalize();
    Vector3 normalized() const;
    float dot(const Vector3& other) const;
    Vector3 cross(const Vector3& other) const;
    Vector3 reflected(const Vector3& normal) const;
};

Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator*(const Vector3& v, float f);
Vector3 operator*(float f, const Vector3& v);
Vector3 operator/(const Vector3& v, float f);

using Point3 = Vector3;

#endif