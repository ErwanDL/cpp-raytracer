#include "light.hpp"
#include "intersectable.hpp"
#include "ray.hpp"
#include "sampling.hpp"
#include "scene.hpp"
#include "trace.hpp"
#include "vector3.hpp"
#include <cmath>
#include <utility>
#include <vector>

Color SphereLight::illuminate(const Intersection& intersection, const Scene& scene,
                              const Point3& observerLocation) const {
    // Sampling the whole sphere volume for now.
    // May want to switch to sampling only the visible sphere surface later.
    Point3 sampledPoint = sampleSpherePoint(origin, radius);
    Vector3 fromLight = (intersection.location - sampledPoint).normalized();
    float lightDotN = -fromLight.dot(intersection.normal);

    if (lightDotN <= 0.0f) {
        return Color(0.0f);
    }

    // checking if a shape is between the intersection and the light,
    // in which case a shadow is cast
    Ray rayTowardsLight{intersection.location, -fromLight,
                        (sampledPoint - intersection.location).length()};
    auto shadowIntersection = scene.findFirstIntersection(rayTowardsLight);

    if (shadowIntersection) {
        return Color(0.0f);
    }

    Vector3 towardsObserver = (observerLocation - intersection.location).normalized();
    float observerDotReflected = towardsObserver.dot(fromLight.reflected(intersection.normal));

    const Material& material = intersection.material.get();
    return (computeDiffuse(lightDotN, material) + computeSpecular(observerDotReflected, material)) /
           (intersection.location - sampledPoint).lengthSquared();
}

// Still using a normalized Phong model for both diffuse and specular
Color SphereLight::computeDiffuse(float lightDotN, const Material& material) const {
    return material.metal ? Color::BLACK
                          : lightDotN * intensity * color * material.color / Utils::PI;
}

Color SphereLight::computeSpecular(float observerDotReflected, const Material& material) const {
    if (observerDotReflected <= 0.0f) {
        return Color(0.0f);
    }
    float normalization = (material.smoothness + 1.0f) / (2.0f * Utils::PI);
    return std::pow(observerDotReflected, material.smoothness) * intensity * color *
           (material.metal ? material.color : material.specularity) * normalization;
}