#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <vector>
#include "image.hpp"
#include "ray.hpp"
#include "vectors.hpp"

class Shape {
   public:
    virtual bool intersect(Intersection &i) = 0;
    virtual bool doesIntersect(const Ray &ray) const = 0;
    virtual ~Shape() {}
};

class ShapeSet : public Shape {
   protected:
    std::vector<Shape *> shapes{};

   public:
    ShapeSet();
    virtual ~ShapeSet() {}

    void addShape(Shape *shape);
    virtual bool intersect(Intersection &i);
    virtual bool doesIntersect(const Ray &ray) const;
};

class Plane : public Shape {
   protected:
    Point3 position;
    Vector3 normal;
    Color color;

   public:
    Plane(const Point3 &position, const Vector3 &normal,
          const Color &color = Color{1.0f});
    virtual ~Plane() {}

    virtual bool intersect(Intersection &i);
    virtual bool doesIntersect(const Ray &ray) const;
};

class Sphere : public Shape {
   protected:
    Point3 centre;
    float radius;
    Color color;

   public:
    Sphere(const Point3 &centre, float radius,
           const Color &color = Color{1.0f});
    virtual ~Sphere() {}

    virtual bool intersect(Intersection &i);
    virtual bool doesIntersect(const Ray &ray) const;
};

#endif