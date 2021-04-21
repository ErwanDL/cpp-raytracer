#ifndef VECTORS_HPP
#define VECTORS_HPP
#include <iostream>

struct Vector3 {
    float x;
    float y;
    float z;

    Vector3(float x, float y, float z);
    explicit Vector3(float f);

    float lengthSquared() const;
    float length() const;
    void normalize();
    Vector3 normalized() const;
    float dot(const Vector3& other) const;
    Vector3 cross(const Vector3& other) const;
    Vector3 reflected(const Vector3& normal) const;

    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator*=(float f);
    Vector3& operator/=(float f);
    Vector3 operator-() const;
    bool operator==(const Vector3& other) const;
};

Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator*(const Vector3& v, float f);
Vector3 operator*(float f, const Vector3& v);
Vector3 operator/(const Vector3& v, float f);
std::ostream& operator<<(std::ostream& out, const Vector3& v);

using Point3 = Vector3;

struct Vector2 {
    float u;
    float v;

    Vector2(float u, float v);
    explicit Vector2(float f);
    float lengthSquared() const;
    float length() const;
    void normalize();
    Vector2 normalized() const;

    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(float f);
    Vector2& operator/=(float f);
    bool operator==(const Vector2& other) const;
};

using Point2 = Vector2;

#endif