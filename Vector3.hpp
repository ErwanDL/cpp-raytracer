#ifndef VECTOR3_HPP
#define VECTOR3_HPP

class Vector3
{
    float x{0.0};
    float y{0.0};
    float z{0.0};

public:
    Vector3();
    Vector3(const Vector3 &v);
    Vector3(float x, float y, float z);
    Vector3(float f);

    float lengthSquared() const;
    float length() const;

    float normalize();
    Vector3 normalized() const;

    float dot(const Vector3 &other) const;
    Vector3 cross(const Vector3 &other) const;

    Vector3 &operator+=(const Vector3 &v);
    Vector3 &operator-=(const Vector3 &v);
    Vector3 &operator*=(float f);
    Vector3 &operator/=(float f);
    Vector3 operator-() const;

    float getX() const;
    float getY() const;
    float getZ() const;

    void setX(float x);
    void setY(float y);
    void setZ(float z);
};

Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
Vector3 operator*(const Vector3 &v, float f);
Vector3 operator*(float f, const Vector3 &v);
Vector3 operator/(const Vector3 &v, float f);

using Point3 = Vector3;

#endif