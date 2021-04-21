#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <memory>
#include <optional>
#include <vector>

#include "material.hpp"
#include "vectors.hpp"

struct Ray;
class Shape;

struct Intersection {
    Point3 location;
    Vector3 normal;
    float distanceToRayOrigin;
    Material material;

    Intersection(Point3 location, const Vector3& normal, float distanceToRayOrigin,
                 const Material& material)
        : location(location), normal(normal), distanceToRayOrigin(distanceToRayOrigin),
          material(material) {}
};

class Intersectable {
  public:
    virtual std::optional<Intersection> intersect(const Ray& ray) const = 0;
};

class Scene : public Intersectable {
  private:
    std::vector<std::shared_ptr<Shape>> shapes{};

  public:
    Scene();

    void addShape(std::shared_ptr<Shape> shape);
    std::optional<Intersection> intersect(const Ray& ray) const override;
};

class Shape : public Intersectable {
  public:
    Material material;

    explicit Shape(const Material& material);
};

class Plane : public Shape {
  private:
    Point3 position;
    Vector3 normal;

  public:
    Plane(const Point3& position, const Vector3& normal, const Material& material);

    std::optional<Intersection> intersect(const Ray& ray) const override;
};

class Sphere : public Shape {
  private:
    Point3 centre;
    float radius;

  public:
    Sphere(const Point3& centre, float radius, const Material& material);

    std::optional<Intersection> intersect(const Ray& ray) const override;
};

#endif