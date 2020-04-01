#include "shape.hpp"

#include <cmath>
#include <optional>

#include "ray.hpp"
#include "utils.hpp"

// CLASS SCENE

Scene::Scene() = default;

void Scene::addShape(Shape *pShape) { shapes.push_back(pShape); }

std::optional<Intersection> Scene::intersect(Ray &ray) const {
    bool doesIntersect{false};

    std::optional<Intersection> closestIntersection;

    for (const Shape *pShape : shapes) {
        const auto &intersection = pShape->intersect(ray);
        if (!closestIntersection ||
            (intersection &&
             (intersection->location - ray.origin).length() <
                 (closestIntersection->location - ray.origin).length())) {
            closestIntersection = intersection;
        }
    }

    return closestIntersection;
}

// CLASS SHAPE

Shape::Shape(const Material &material) : material(material) {}

const Material &Shape::getMaterial() const { return material; }

// CLASS PLANE

Plane::Plane(const Point3 &position, const Vector3 &normal,
             const Material &material)
    : Shape(material), position(position), normal(normal) {}

std::optional<Intersection> Plane::intersect(Ray &ray) const {
    float dDotN{normal.dot(ray.direction)};

    if (dDotN == 0.0f) {
        return {};
    }

    const float t{normal.dot(position - ray.origin) / dDotN};

    if (t <= Ray::MIN_RAY_DIST || t > ray.maxDist) {
        return {};
    }
    Point3 intersectionLocation{ray.origin + t * ray.direction.normalized()};
    Vector3 intersectionNormal{dDotN > 0.0f ? -normal : normal};
    return Intersection(intersectionLocation, intersectionNormal,
                        this->getMaterial());
}

// CLASS SPHERE

Sphere::Sphere(const Point3 &centre, float radius, const Material &material)
    : Shape(material), centre(centre), radius(radius) {}

std::optional<Intersection> Sphere::intersect(Ray &ray) const {
    // float a{raydirection.lengthSquared()}; always equal to 1
    float a{1.0f};
    float b{2 * ray.direction.dot(ray.origin - centre)};
    float c{(ray.origin - centre).lengthSquared() - Math::sqr(radius)};

    float discriminant{Math::sqr(b) - 4 * a * c};

    if (discriminant <= 0.0f)  // if no solution to equation
    {
        return {};
    }

    float t1{(-b - std::sqrt(discriminant)) / (2 * a)};
    float t2{(-b + std::sqrt(discriminant)) / (2 * a)};

    Point3 intersectionLocation{0.0f};
    // since t1 always <= t2, we check t1 first
    if (t1 > Ray::MIN_RAY_DIST && t1 < ray.maxDist) {
        intersectionLocation = ray.origin + t1 * ray.direction.normalized();
    } else if (t2 > Ray::MIN_RAY_DIST && t2 < ray.maxDist) {
        intersectionLocation = ray.origin + t2 * ray.direction.normalized();
    } else {
        return {};
    }
    return Intersection(intersectionLocation,
                        (intersectionLocation - centre).normalized(),
                        this->getMaterial());
}
