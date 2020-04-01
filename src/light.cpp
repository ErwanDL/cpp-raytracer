#include "light.hpp"

#include <cmath>

#include "camera.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include "vectors.hpp"

// CLASS LIGHTRACK

LightRack::LightRack(const Color& ambient) : ambient(ambient) {}

void LightRack::addLight(ILight* pLight) { lights.push_back(pLight); }

Color LightRack::illuminate(const Intersection& intersection,
                            const Scene& scene, const Camera& cam) const {
    const Material& material{intersection.getShape().getMaterial()};
    Color resultingColor{0.0f};
    resultingColor += this->ambient * material.ambient * material.color;
    for (const ILight* pLight : lights) {
        resultingColor += pLight->illuminate(intersection, scene, cam);
    }
    return resultingColor;
}

// CLASS POINTLIGHT

PointLight::PointLight(const Point3& origin, const Color& color)
    : origin(origin), color(color) {}

Color PointLight::illuminate(const Intersection& intersection,
                             const Scene& scene, const Camera& cam) const {
    const Vector3 towardsLight{(origin - intersection.location).normalized()};
    const float lightDotN{towardsLight.dot(intersection.normal)};

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

    const Material& material{intersection.getShape().getMaterial()};
    const Color diffuseColor{lightDotN * this->color * material.color *
                             material.diffuse};
    const Vector3 towardsCam{
        (cam.getOrigin() - intersection.location).normalized()};
    const Color specularColor{
        std::pow(towardsCam.dot(towardsLight.reflected(intersection.normal)),
                 material.shininess) *
        material.specular * this->color};
    return diffuseColor + specularColor;
}
