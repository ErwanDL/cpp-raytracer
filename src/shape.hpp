#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <optional>
#include <vector>

#include "material.hpp"
#include "ray.hpp"
#include "vectors.hpp"

struct Intersection;
class Shape;

class Intersectable {
   public:
    virtual std::optional<Intersection> intersect(const Ray &ray) const = 0;
};

class Scene : public Intersectable {
   private:
    std::vector<const Shape *> shapes{};

   public:
    Scene();

    void addShape(const Shape &shape);
    std::optional<Intersection> intersect(const Ray &ray) const override;
};

class Shape : public Intersectable {
   private:
    Material material;

   public:
    explicit Shape(const Material &material);
    Material getMaterial() const;
};

class Plane : public Shape {
   private:
    Point3 position;
    Vector3 normal;

   public:
    Plane(const Point3 &position, const Vector3 &normal,
          const Material &material = Material());

    std::optional<Intersection> intersect(const Ray &ray) const override;
};

class Sphere : public Shape {
   private:
    Point3 centre;
    float radius;

   public:
    Sphere(const Point3 &centre, float radius,
           const Material &material = Material());

    std::optional<Intersection> intersect(const Ray &ray) const override;
};

#endif