#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <optional>
#include <vector>

#include "material.hpp"
#include "ray.hpp"
#include "vectors.hpp"

class Intersection;

class IIntersectable {
   public:
    virtual std::optional<Intersection> intersect(Ray &ray) const = 0;
    virtual ~IIntersectable() = default;
};

class Scene : public IIntersectable {
   private:
    std::vector<Shape *> shapes{};

   public:
    Scene();
    ~Scene() override = default;

    void addShape(Shape *shape);
    std::optional<Intersection> intersect(Ray &ray) const override;
};

class Shape : public IIntersectable {
   protected:
    Material material;

   public:
    Shape(const Material &material);
    virtual ~Shape() {}

    const Material &getMaterial() const;

    void setMaterial(const Material &newMaterial);
};

class Plane : public Shape {
   private:
    Point3 position;
    Vector3 normal;

   public:
    Plane(const Point3 &position, const Vector3 &normal,
          const Material &material = Material());

    std::optional<Intersection> intersect(Ray &ray) const override;
};

class Sphere : public Shape {
   private:
    Point3 centre;
    float radius;

   public:
    Sphere(const Point3 &centre, float radius,
           const Material &material = Material());

    std::optional<Intersection> intersect(Ray &ray) const override;
};

#endif