#ifndef INTERSECTABLE_HPP
#define INTERSECTABLE_HPP

#include "intersection.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "sampling.hpp"
#include "vector3.hpp"
#include <memory>
#include <optional>
#include <vector>

class Intersectable {
  public:
    Material material;

    Intersectable(const Material& material) : material(material) {}

    virtual std::optional<Intersection> intersect(const Ray& ray) const = 0;
    virtual PointSamplingResult sampleForDirectLighting(const Point3& location) const = 0;
};

class Plane : public Intersectable {
  private:
    Point3 position;
    Vector3 normal;

  public:
    Plane(const Point3& position, const Vector3& normal, const Material& material)
        : Intersectable(material), position(position), normal(normal) {}

    std::optional<Intersection> intersect(const Ray& ray) const override;
    PointSamplingResult sampleForDirectLighting(const Point3& location) const override;
};

class Sphere : public Intersectable {
  private:
    Point3 center;
    float radius;

  public:
    Sphere(const Point3& center, float radius, const Material& material)
        : Intersectable(material), center(center), radius(radius) {}

    std::optional<Intersection> intersect(const Ray& ray) const override;
    PointSamplingResult sampleForDirectLighting(const Point3& location) const override;
};

#endif