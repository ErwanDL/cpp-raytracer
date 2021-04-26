#ifndef INTERSECTABLE_HPP
#define INTERSECTABLE_HPP

#include "intersection.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vector3.hpp"
#include <memory>
#include <optional>
#include <vector>

class Intersectable {
  protected:
    Material material;

    Intersectable(const Material& material) : material(material) {}

  public:
    virtual std::optional<Intersection> intersect(const Ray& ray) const = 0;
    virtual Color computeDirectDiffuse(const Point3& location,
                                       const Point3& sampledPoint) const = 0;
    virtual Point3 sampleSolidAngle(const Point3& location) const = 0;
};

class Plane : public Intersectable {
  private:
    Point3 position;
    Vector3 normal;

  public:
    Plane(const Point3& position, const Vector3& normal, const Material& material)
        : Intersectable(material), position(position), normal(normal) {}

    std::optional<Intersection> intersect(const Ray& ray) const override;
    Color computeDirectDiffuse(const Point3& observer, const Point3& sampledPoint) const override;
    Point3 sampleSolidAngle(const Point3& observer) const override;
};

class Sphere : public Intersectable {
  private:
    Point3 center;
    float radius;

  public:
    Sphere(const Point3& center, float radius, const Material& material)
        : Intersectable(material), center(center), radius(radius) {}

    std::optional<Intersection> intersect(const Ray& ray) const override;
    Color computeDirectDiffuse(const Point3& observer, const Point3& sampledPoint) const override;
    Point3 sampleSolidAngle(const Point3& observer) const override;
};

#endif