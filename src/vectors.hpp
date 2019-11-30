#ifndef VECTORS_HPP
#define VECTORS_HPP
#include <iostream>

class IVector {
   public:
    virtual ~IVector() {}
    virtual float lengthSquared() const = 0;

    float length() const;

    virtual float normalize() = 0;
};

class Vector3 : public IVector {
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};

   public:
    Vector3();
    Vector3(const Vector3 &v);
    Vector3(float x, float y, float z);
    Vector3(float f);
    virtual ~Vector3() override {}

    virtual float lengthSquared() const override;
    virtual float normalize() override;
    Vector3 normalized() const;
    float dot(const Vector3 &other) const;
    Vector3 cross(const Vector3 &other) const;

    Vector3 &operator+=(const Vector3 &other);
    Vector3 &operator-=(const Vector3 &other);
    Vector3 &operator*=(float f);
    Vector3 &operator/=(float f);
    Vector3 operator-() const;

    float getX() const;
    float getY() const;
    float getZ() const;

    void setX(float newX);
    void setY(float newY);
    void setZ(float newZ);
};

Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
Vector3 operator*(const Vector3 &v, float f);
Vector3 operator*(float f, const Vector3 &v);
Vector3 operator/(const Vector3 &v, float f);
std::ostream &operator<<(std::ostream &out, const Vector3 &v);

using Point3 = Vector3;

class Vector2 : public IVector {
    float u{0.0};
    float v{0.0};

   public:
    Vector2();
    Vector2(const Vector2 &other);
    Vector2(float u, float v);
    Vector2(float f);
    virtual ~Vector2() override {}
    virtual float lengthSquared() const override;
    virtual float normalize() override;
    Vector2 normalized() const;

    Vector2 &operator+=(const Vector2 &other);
    Vector2 &operator-=(const Vector2 &other);
    Vector2 &operator*=(float f);
    Vector2 &operator/=(float f);

    float getU() const;
    float getV() const;

    void setU(float newU);
    void setV(float newV);
};

using Point2 = Vector2;

#endif