#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "intersection.hpp"
#include "material.hpp"
#include "vector3.hpp"
#include <memory>
#include <optional>
#include <vector>

class Intersectable {
  public:
    virtual std::optional<Intersection> intersect(const Ray& ray) const = 0;
};

class Shape : public Intersectable {
  protected:
    Material material;

    Shape(const Material& material) : material(material) {}
};

class Plane : public Shape {
  private:
    Point3 position;
    Vector3 normal;

  public:
    Plane(const Point3& position, const Vector3& normal, const Material& material)
        : Shape(material), position(position), normal(normal) {}

    std::optional<Intersection> intersect(const Ray& ray) const override;
};

class Sphere : public Shape {
  private:
    Point3 centre;
    float radius;

  public:
    Sphere(const Point3& centre, float radius, const Material& material)
        : Shape(material), centre(centre), radius(radius) {}

    std::optional<Intersection> intersect(const Ray& ray) const override;
};

#endif