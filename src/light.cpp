#include "light.hpp"

#include <cmath>

#include "camera.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include "vectors.hpp"

// CLASS LIGHTRACK

LightRack::LightRack(const Color& ambient) : ambient(ambient) {}

void LightRack::addLight(Light* pLight) { lights.push_back(pLight); }

Color LightRack::illuminate(const Intersection& intersection,
                            const Scene& scene,
                            const Point3& observerLocation) const {
    const Material& material{intersection.material};
    Color resultingColor{0.0f};

    resultingColor += this->ambient * material.color;

    for (const Light* pLight : lights) {
        resultingColor +=
            pLight->illuminate(intersection, scene, observerLocation);
    }
    return resultingColor;
}

// CLASS POINTLIGHT

PointLight::PointLight(const Point3& origin, const Color& color)
    : origin(origin), color(color) {}

Color PointLight::illuminate(const Intersection& intersection,
                             const Scene& scene,
                             const Point3& observerLocation) const {
    const Vector3 towardsLight = (origin - intersection.location).normalized();
    const float lightDotN = towardsLight.dot(intersection.normal);

    if (lightDotN <= 0.0f) {
        return Color(0.0f);
    }

    // checking if a shape is between the intersection and the light,
    // in which case a shadow is cast
    Ray rayTowardsLight{intersection.location, towardsLight,
                        (origin - intersection.location).length()};
    if (scene.intersect(rayTowardsLight)) {
        return Color(0.0f);
    }

    const Vector3 towardsCam =
        (observerLocation - intersection.location).normalized();
    const float observerDotReflected =
        towardsCam.dot(towardsLight.reflected(intersection.normal));

    const Color diffuseColor = computeDiffuse(lightDotN, intersection.material);
    const Color specularColor =
        computeSpecular(observerDotReflected, intersection.material);
    return diffuseColor + specularColor;
}

Color PointLight::computeDiffuse(float lightDotN,
                                 const Material& material) const {
    return lightDotN * this->color * material.color * material.diffuse;
}

Color PointLight::computeSpecular(float observerDotReflected,
                                  const Material& material) const {
    return std::pow(observerDotReflected, material.shininess) *
           material.specular * this->color;
}