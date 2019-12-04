#include "shape.hpp"
#include <cmath>
#include "math_utils.hpp"

// CLASS SCENE

Scene::Scene() {}

void Scene::addShape(Shape *pShape) { shapes.push_back(pShape); }

bool Scene::intersect(Ray &ray) const {
    bool doesIntersect{false};

    for (const Shape *pShape : shapes) {
        if (pShape->intersect(ray)) doesIntersect = true;
    }

    return doesIntersect;
}

// CLASS SHAPE

Shape::Shape(const Color &color) : color(color) {}

const Color &Shape::getColor() const { return color; }

void Shape::setColor(const Color &newColor) { color = newColor; }

// CLASS PLANE

Plane::Plane(const Point3 &position, const Vector3 &normal, const Color &color)
    : Shape(color), position(position), normal(normal) {}

bool Plane::intersect(Ray &ray) const {
    float dDotN{normal.dot(ray.getDirection())};

    if (dDotN == 0.0f) return false;

    const float t{normal.dot(position - ray.getOrigin()) / dDotN};

    Intersection &i = ray.getIntersection();

    if (t <= Intersection::MIN_RAY_DIST || t >= i.getDistance()) return false;
    Vector3 intersectionNormal{dDotN > 0.0f ? -normal : normal};
    i.setNormal(intersectionNormal);
    i.setDistance(t);
    i.setPShape(this);

    return true;
}

// CLASS SPHERE

Sphere::Sphere(const Point3 &centre, float radius, const Color &color)
    : Shape(color), centre(centre), radius(radius) {}

bool Sphere::intersect(Ray &ray) const {
    // float a{ray.getDirection().lengthSquared()}; always equal to 1
    float a{1};
    float b{2 * ray.getDirection().dot(ray.getOrigin() - centre)};
    float c{(ray.getOrigin() - centre).lengthSquared() - Math::sqr(radius)};

    float discriminant{Math::sqr(b) - 4 * a * c};

    if (discriminant <= 0.0f)  // if no solution to equation
        return false;

    float t1{-b - std::sqrt(discriminant) / (2 * a)};
    float t2{-b + std::sqrt(discriminant) / (2 * a)};
    std::cout << t1 << '\t' << t2 << '\t' << a << '\n';

    Intersection &i{ray.getIntersection()};

    // we check t1 first because it is always closer than t2
    if (t1 > Intersection::MIN_RAY_DIST && t1 < i.getDistance())
        i.setDistance(t1);
    else if (t2 > Intersection::MIN_RAY_DIST && t2 < i.getDistance())
        i.setDistance(t2);
    else
        return false;
    i.setPShape(this);
    i.setNormal((ray.pointOfIntersection() - centre).normalized());
    return true;
}
