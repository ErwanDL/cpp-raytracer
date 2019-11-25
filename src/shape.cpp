#include <cmath>
#include "shape.hpp"
#include "math_utils.hpp"

ShapeSet::ShapeSet() {}

ShapeSet::~ShapeSet() {}

void ShapeSet::addShape(Shape *shape)
{
    shapes.push_back(shape);
}

bool ShapeSet::intersect(Intersection &i)
{
    bool doesIntersect{false};

    for (Shape *const shape : shapes)
    {
        if (shape->intersect(i))
            doesIntersect = true;
    }

    return doesIntersect;
}

bool ShapeSet::doesIntersect(const Ray &ray)
{
    for (Shape *const shape : shapes)
    {
        if (shape->doesIntersect(ray))
            return true;
    }

    return false;
}

Plane::Plane(const Point3 &position, const Vector3 &normal)
    : position(position), normal(normal) {}

Plane::~Plane() {}

bool Plane::intersect(Intersection &i)
{
    const Ray iRay{i.getRay()};
    float dDotN{normal.dot(iRay.getDirection())};

    if (dDotN == 0.0f)
        return false;

    const float t{normal.dot(position - iRay.getOrigin()) / dDotN};

    if (t <= Ray::MAX_RAY_DIST || t >= i.getT())
        return false;

    i.setT(t);
    i.setPShape(this);

    return true;
}

bool Plane::doesIntersect(const Ray &ray)
{
    float dDotN{normal.dot(ray.getDirection())};

    if (dDotN == 0.0f)
        return false;

    const float t{normal.dot(position - ray.getOrigin()) / dDotN};

    if (t <= Ray::MAX_RAY_DIST || t >= ray.getMaxDist())
        return false;

    return true;
}

Sphere::Sphere(const Point3 &centre, float radius)
    : centre(centre), radius(radius) {}

Sphere::~Sphere() {}

bool Sphere::intersect(Intersection &i)
{
    const Ray iRay{i.getRay()};

    float a{iRay.getDirection().lengthSquared()};
    float b{2 * iRay.getDirection().dot(iRay.getOrigin() - centre)};
    float c{(iRay.getOrigin() - centre).lengthSquared() - Math::sqr(radius)};

    float discriminant{Math::sqr(b) - 4 * a * c};

    if (discriminant <= 0.0f) // if no solution to equation
        return false;

    float t1{-b - std::sqrt(discriminant) / (2 * a)};
    float t2{-b + std::sqrt(discriminant) / (2 * a)};

    // we check t1 first because it is always closer than t2
    if (t1 > Ray::MIN_RAY_DIST && t1 < i.getT())
        i.setT(t1);
    else if (t2 > Ray::MIN_RAY_DIST && t2 < i.getT())
        i.setT(t2);
    else
        return false;

    i.setPShape(this);
    return true;
}

bool Sphere::doesIntersect(const Ray &ray)
{
    float a{ray.getDirection().lengthSquared()};
    float b{2 * ray.getDirection().dot(ray.getOrigin() - centre)};
    float c{(ray.getOrigin() - centre).lengthSquared() - Math::sqr(radius)};

    float discriminant{Math::sqr(b) - 4 * a * c};

    if (discriminant <= 0.0f) // if no solution to equation
        return false;

    float t1{-b - std::sqrt(discriminant) / (2 * a)};
    float t2{-b + std::sqrt(discriminant) / (2 * a)};

    // we check t1 first because it is always closer than t2
    if (t1 > Ray::MIN_RAY_DIST && t1 < ray.getMaxDist())
        return true;
    else if (t2 > Ray::MIN_RAY_DIST && t2 < ray.getMaxDist())
        return true;
    else
        return false;
}
