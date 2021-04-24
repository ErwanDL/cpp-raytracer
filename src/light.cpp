#include "light.hpp"
#include "ray.hpp"
#include "sampling.hpp"
#include "scene.hpp"
#include "shape.hpp"
#include "trace.hpp"
#include "vector3.hpp"
#include <cmath>
#include <utility>
#include <vector>

Color AmbientLight::illuminate(const Intersection& intersection, const Scene&,
                               const Point3&) const {
    return intersection.material.get().albedo * color;
}

Color SphereLight::illuminate(const Intersection& intersection, const Scene& scene,
                              const Point3& observerLocation) const {
    // Sampling the whole sphere volume for now.
    // May want to switch to sampling only the visible sphere surface later.
    Point3 sampledPoint = sampleSpherePoint(origin, radius);
    const Vector3 fromLight = (intersection.location - sampledPoint).normalized();
    const float lightDotN = -fromLight.dot(intersection.normal);

    if (lightDotN <= 0.0f) {
        return Color(0.0f);
    }

    // checking if a shape is between the intersection and the light,
    // in which case a shadow is cast
    Ray rayTowardsLight{intersection.location, -fromLight,
                        (sampledPoint - intersection.location).length()};
    const auto shadowIntersection = scene.findFirstIntersection(rayTowardsLight);

    if (shadowIntersection) {
        return Color(0.0f);
    }

    const Vector3 towardsObserver = (observerLocation - intersection.location).normalized();
    const float observerDotReflected =
        towardsObserver.dot(fromLight.reflected(intersection.normal));

    const Color diffuseColor = computeDiffuse(lightDotN, intersection.material.get());
    const Color specularColor =
        observerDotReflected < 0.0f
            ? Color(0.0f)
            : computeSpecular(observerDotReflected, intersection.material.get());

    return diffuseColor + specularColor;
}

Color SphereLight::computeDiffuse(float lightDotN, const Material& material) const {
    return material.metal ? Color::BLACK : lightDotN * color * material.albedo;
}

Color SphereLight::computeSpecular(float observerDotReflected, const Material& material) const {
    float clampedSmoothness = std::min(material.smoothness, 500.0f);
    return std::pow(observerDotReflected, clampedSmoothness) * color *
           (material.metal ? material.albedo : material.specularity);
}