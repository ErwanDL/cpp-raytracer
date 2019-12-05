#include "light.hpp"
#include <cmath>
#include "camera.hpp"
#include "ray.hpp"
#include "shape.hpp"
#include "vectors.hpp"

// CLASS LIGHTRACK

LightRack::LightRack(const Color& ambient) : ambient(ambient) {}

void LightRack::addLight(ILight* pLight) { lights.push_back(pLight); }

Color LightRack::illuminate(const Ray& intersectedRay, const Scene& scene,
                            const Camera& cam) const {
    const Material& material{
        intersectedRay.getIntersection().getPShape()->getMaterial()};
    Color resultingColor{0.0f};
    resultingColor += this->ambient * material.ambient * material.color;
    for (const ILight* pLight : lights) {
        resultingColor += pLight->illuminate(intersectedRay, scene, cam);
    }
    return resultingColor;
}

// CLASS POINTLIGHT

PointLight::PointLight() {}

PointLight::PointLight(const Point3& origin, const Color& color)
    : origin(origin), color(color) {}

Color PointLight::illuminate(const Ray& intersectedRay, const Scene& scene,
                             const Camera& cam) const {
    const Point3 interPoint{intersectedRay.pointOfIntersection()};
    const Intersection& i{intersectedRay.getIntersection()};

    const Vector3 towardsLight{(origin - interPoint).normalized()};
    const float lightDotN{towardsLight.dot(i.getNormal())};

    if (lightDotN <= 0.0f) return Color(0.0f);

    // checking if a shape is between the intersection and the light,
    // in which case a shadow is cast
    Ray rayTowardsLight{interPoint, towardsLight,
                        (origin - interPoint).length()};
    if (scene.intersect(rayTowardsLight)) {
        return Color(0.0f);
    }

    const Material& material{i.getPShape()->getMaterial()};

    return lightDotN * this->color * material.color * material.diffuse;
}
