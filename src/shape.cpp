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
             (intersection->location - ray.getOrigin()).length() <
                 (closestIntersection->location - ray.getOrigin()).length())) {
            closestIntersection = intersection;
        }
    }

    return closestIntersection;
}

// CLASS SHAPE

Shape::Shape(const Material &material) : material(material) {}

const Material &Shape::getMaterial() const { return material; }

void Shape::setMaterial(const Material &newMaterial) { material = newMaterial; }

// CLASS PLANE

Plane::Plane(const Point3 &position, const Vector3 &normal,
             const Material &material)
    : Shape(material), position(position), normal(normal) {}

std::optional<Intersection> Plane::intersect(Ray &ray) const {
    float dDotN{normal.dot(ray.getDirection())};

    if (dDotN == 0.0f) {
        return {};
    }

    const float t{normal.dot(position - ray.getOrigin()) / dDotN};

    if (t <= Ray::MIN_RAY_DIST || t > ray.maxDist) {
        return {};
    }
    Point3 intersectionLocation{ray.getOrigin() +
                                t * ray.getDirection().normalized()};
    Vector3 intersectionNormal{dDotN > 0.0f ? -normal : normal};
    return Intersection(intersectionLocation, intersectionNormal, *this, ray);
}

// CLASS SPHERE

Sphere::Sphere(const Point3 &centre, float radius, const Material &material)
    : Shape(material), centre(centre), radius(radius) {}

std::optional<Intersection> Sphere::intersect(Ray &ray) const {
    // float a{ray.getDirection().lengthSquared()}; always equal to 1
    float a{1.0f};
    float b{2 * ray.getDirection().dot(ray.getOrigin() - centre)};
    float c{(ray.getOrigin() - centre).lengthSquared() - Math::sqr(radius)};

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
        intersectionLocation =
            ray.getOrigin() + t1 * ray.getDirection().normalized();
    } else if (t2 > Ray::MIN_RAY_DIST && t2 < ray.maxDist) {
        intersectionLocation =
            ray.getOrigin() + t2 * ray.getDirection().normalized();
    } else {
        return {};
    }
    return Intersection(intersectionLocation,
                        (intersectionLocation - centre).normalized(), *this,
                        ray);
}
