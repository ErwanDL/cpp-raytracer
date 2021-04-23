#include "light.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "shape.hpp"
#include "trace.hpp"
#include "vector3.hpp"
#include <cmath>
#include <utility>
#include <vector>

Color AmbientLight::illuminate(const Intersection& intersection, const Scene&,
                               const Point3&) const {
    return intersection.material.albedo * color;
}

Color PointLight::illuminate(const Intersection& intersection, const Scene& scene,
                             const Point3& observerLocation) const {
    const Vector3 fromLight = (intersection.location - origin).normalized();
    const float lightDotN = -fromLight.dot(intersection.normal);

    if (lightDotN <= 0.0f) {
        return Color(0.0f);
    }

    // checking if a shape is between the intersection and the light,
    // in which case a shadow is cast
    Ray rayTowardsLight{intersection.location, -fromLight,
                        (origin - intersection.location).length()};
    const auto shadowIntersection = scene.findFirstIntersection(rayTowardsLight);

    if (shadowIntersection) {
        return Color(0.0f);
    }

    const Vector3 towardsObserver = (observerLocation - intersection.location).normalized();
    const float observerDotReflected =
        towardsObserver.dot(fromLight.reflected(intersection.normal));

    const Color diffuseColor = computeDiffuse(lightDotN, intersection.material);
    const Color specularColor = observerDotReflected < 0.0f
                                    ? Color(0.0f)
                                    : computeSpecular(observerDotReflected, intersection.material);

    return diffuseColor + specularColor;
}

Color PointLight::computeDiffuse(float lightDotN, const Material& material) const {
    return material.metal ? Color::BLACK : lightDotN * color * material.albedo;
}

Color PointLight::computeSpecular(float observerDotReflected, const Material& material) const {
    return std::pow(observerDotReflected, std::min(material.smoothness, 500.0f)) * color *
           (material.metal ? material.albedo : material.specularity);
}