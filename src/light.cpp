#include "light.hpp"
#include <cmath>
#include "camera.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include "vectors.hpp"

Light::Light() {}

Light::Light(const Point3& origin, const Color& color)
    : origin(origin), color(color) {}

Color Light::illuminate(const Ray& intersectedRay, const Scene& scene,
                        const Camera& cam) const {
    const Intersection& i{intersectedRay.getIntersection()};

    const Vector3 lightDirection{
        (intersectedRay.pointOfIntersection() - origin).normalized()};
    const float lightDotN{lightDirection.dot(i.getNormal())};

    if (lightDotN >= 0.0f) return Color(0.0f);
    return color * (i.getPShape()->getColor());
}
