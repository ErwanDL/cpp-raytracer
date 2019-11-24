#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "vector3.hpp"
#include "ray.hpp"

class Shape
{
    virtual ~Shape() {}

    virtual bool intersect(Intersection &i) = 0;
    virtual bool doesIntersect(const Ray &ray) = 0;
};

#endif