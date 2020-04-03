#include "shape.hpp"

#include <cmath>
#include <optional>

#include "ray.hpp"
#include "utils.hpp"

// CLASS SCENE

Scene::Scene() = default;

void Scene::addShape(const Shape &shape) { shapes.push_back(&shape); }

std::optional<Intersection> Scene::intersect(const Ray &ray) const {
    std::optional<Intersection> closestIntersection;

    for (const Shape *pShape : shapes) {
        const auto &intersection = pShape->intersect(ray);
        if (!closestIntersection ||
            (intersection && intersection->distanceToRayOrigin <
                                 closestIntersection->distanceToRayOrigin)) {
            closestIntersection = intersection;
        }
    }

    return closestIntersection;
}

// CLASS SHAPE

Shape::Shape(const Material &material) : material(material) {}
Material Shape::getMaterial() const { return material; }

// CLASS PLANE

Plane::Plane(const Point3 &position, const Vector3 &normal,
             const Material &material)
    : Shape(material), position(position), normal(normal) {}

std::optional<Intersection> Plane::intersect(const Ray &ray) const {
    float dDotN{normal.dot(ray.direction)};

    if (dDotN == 0.0f) {
        return {};
    }

    const float t = normal.dot(position - ray.origin) / dDotN;

    if (!ray.isValidRayDistance(t)) {
        return {};
    }

    Point3 intersectionLocation = ray.origin + t * ray.direction.normalized();
    Vector3 intersectionNormal = dDotN > 0.0f ? -normal : normal;
    return Intersection(intersectionLocation, intersectionNormal, t,
                        this->getMaterial());
}

// CLASS SPHERE

Sphere::Sphere(const Point3 &centre, float radius, const Material &material)
    : Shape(material), centre(centre), radius(radius) {}

std::optional<Intersection> Sphere::intersect(const Ray &ray) const {
    // float a = raydirection.lengthSquared() is always equal to 1
    float a = 1.0f;
    float b = 2 * ray.direction.dot(ray.origin - centre);
    float c = (ray.origin - centre).lengthSquared() - Math::sqr(radius);

    auto solutions = Math::solveSecondDegreeEquation(a, b, c);

    if (!solutions) {
        return {};
    }

    float t;
    // since t1 always <= t2, we check t1 first
    if (ray.isValidRayDistance(solutions->first)) {
        t = solutions->first;
    } else if (ray.isValidRayDistance(solutions->second)) {
        t = solutions->second;
    } else {
        return {};
    }

    Point3 intersectionLocation = ray.origin + t * ray.direction.normalized();
    return Intersection(intersectionLocation,
                        (intersectionLocation - centre).normalized(), t,
                        this->getMaterial());
}
