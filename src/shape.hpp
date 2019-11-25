#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "vector3.hpp"
#include "ray.hpp"
#include <vector>

class Shape
{
public:
    virtual ~Shape() {}

    virtual bool intersect(Intersection &i) = 0;
    virtual bool doesIntersect(const Ray &ray) = 0;
};

class ShapeSet : public Shape
{
protected:
    std::vector<Shape *> shapes{};

public:
    ShapeSet();

    virtual ~ShapeSet();

    void addShape(Shape *shape);

    virtual bool intersect(Intersection &i);
    virtual bool doesIntersect(const Ray &ray);
};

class Plane : public Shape
{
protected:
    Point3 position;
    Vector3 normal;

public:
    Plane(const Point3 &position, const Vector3 &normal);

    virtual ~Plane();

    virtual bool intersect(Intersection &i);
    virtual bool doesIntersect(const Ray &ray);
};

class Sphere : public Shape
{
protected:
    Point3 centre;
    float radius;

public:
    Sphere(const Point3 &centre, float radius);

    virtual ~Sphere();

    virtual bool intersect(Intersection &i);
    virtual bool doesIntersect(const Ray &ray);
};

#endif