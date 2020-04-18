#include "light.hpp"

#include <cmath>
#include <utility>
#include <vector>

#include "ray.hpp"
#include "shape.hpp"
#include "vectors.hpp"

// CLASS LIGHTRACK

void LightRack::addLight(const Light& light) { lights.push_back(&light); }

Color LightRack::illuminate(const Intersection& intersection,
                            const Intersectable& scene,
                            const Point3& observerLocation) const {
    Color resultingColor{0.0f};

    for (const Light* pLight : lights) {
        resultingColor +=
            pLight->illuminate(intersection, scene, observerLocation);
    }
    return resultingColor;
}

// CLASS AMBIENTLIGHT
AmbientLight::AmbientLight(const Color& color) : color(color) {}

Color AmbientLight::illuminate(const Intersection& intersection,
                               const Intersectable&, const Point3&) const {
    return intersection.material.diffuseColor * color;
}

// CLASS POINTLIGHT

PointLight::PointLight(const Point3& origin, const Color& color)
    : origin(origin), color(color) {}

Color PointLight::illuminate(const Intersection& intersection,
                             const Intersectable& scene,
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
    const auto shadowIntersection = scene.intersect(rayTowardsLight);

    if (shadowIntersection) {
        return Color(0.0f);
    }

    const Vector3 towardsObserver =
        (observerLocation - intersection.location).normalized();
    const float observerDotReflected =
        towardsObserver.dot(fromLight.reflected(intersection.normal));

    const Color diffuseColor = computeDiffuse(lightDotN, intersection.material);
    const Color specularColor =
        observerDotReflected < 0.0f
            ? Color(0.0f)
            : computeSpecular(observerDotReflected, intersection.material);

    return diffuseColor + specularColor;
}

Color PointLight::computeDiffuse(float lightDotN,
                                 const Material& material) const {
    return lightDotN * this->color * material.diffuseColor;
}

Color PointLight::computeSpecular(float observerDotReflected,
                                  const Material& material) const {
    return std::pow(observerDotReflected, material.smoothness) *
           material.specularColor * this->color *
           std::pow((material.smoothness - 1) / material.smoothness, 0.8f);
}