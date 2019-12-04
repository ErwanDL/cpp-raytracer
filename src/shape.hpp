#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <vector>
#include "image.hpp"
#include "ray.hpp"
#include "vectors.hpp"

class IIntersectable {
   public:
    virtual bool intersect(Ray &ray) const = 0;
    virtual ~IIntersectable() {}
};

class Scene : public IIntersectable {
   protected:
    std::vector<Shape *> shapes{};

   public:
    Scene();
    virtual ~Scene() {}

    void addShape(Shape *shape);
    virtual bool intersect(Ray &ray) const override;
};

class Shape : public IIntersectable {
   protected:
    Color color;

   public:
    Shape(const Color &color);
    virtual ~Shape() {}

    const Color &getColor() const;

    void setColor(const Color &newColor);
};

class Plane : public Shape {
   protected:
    Point3 position;
    Vector3 normal;

   public:
    Plane(const Point3 &position, const Vector3 &normal,
          const Color &color = Color{1.0f});
    virtual ~Plane() {}

    virtual bool intersect(Ray &ray) const override;
};

class Sphere : public Shape {
   protected:
    Point3 centre;
    float radius;

   public:
    Sphere(const Point3 &centre, float radius,
           const Color &color = Color{1.0f});
    virtual ~Sphere() {}

    virtual bool intersect(Ray &ray) const override;
};

#endif