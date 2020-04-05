#include "light.hpp"

#include <cmath>
#include <utility>
#include <vector>

#include "ray.hpp"
#include "shape.hpp"
#include "utils.hpp"
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
AmbientLight::AmbientLight(const Color& color, bool useAmbientOcclusion)
    : color(color), useAmbientOcclusion(useAmbientOcclusion) {}

Color AmbientLight::illuminate(const Intersection& intersection,
                               const Intersectable& scene,
                               const Point3&) const {
    float occlusionCoeff = 1.0f;
    if (useAmbientOcclusion) {
        const float distanceToClosestShape = scene.distanceTo(intersection);
        if (distanceToClosestShape != Math::INF) {
            occlusionCoeff = 0.6f + 0.4f * (distanceToClosestShape /
                                            (1 + distanceToClosestShape));
        }
    }

    return intersection.material.color * color * occlusionCoeff;
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
    if (scene.intersect(rayTowardsLight)) {
        return Color(0.0f);
    }

    const Vector3 towardsCam =
        (observerLocation - intersection.location).normalized();
    const float observerDotReflected =
        towardsCam.dot(fromLight.reflected(intersection.normal));

    const Color diffuseColor = computeDiffuse(lightDotN, intersection.material);
    const Color specularColor =
        computeSpecular(observerDotReflected, intersection.material);
    return diffuseColor + specularColor;
}

Color PointLight::computeDiffuse(float lightDotN,
                                 const Material& material) const {
    return lightDotN * this->color * material.color;
}

Color PointLight::computeSpecular(float observerDotReflected,
                                  const Material& material) const {
    return std::pow(observerDotReflected, material.shininess) *
           material.specular * this->color;
}