#include "scene.hpp"
#include "ray.hpp"
#include "trace.hpp"
#include <cmath>

Color Scene::shootRay(const Ray& ray, int remainingBounces, bool isCameraRay) const {
    const auto optIntersection = findFirstIntersection(ray);

    if (!optIntersection) {
        return skyColor;
    }

    const Intersection& intersection = optIntersection.value();
    const Material& material = intersection.material.get();
    Color irradiance;
    bool clampIrradiance = (params.firefliesClamping && isCameraRay);

    if (material.type == MaterialType::Emissive) {
        Color li = material.color * material.emission;

        // If nextEventEstimation is activated and the ray comes from diffuse reflection, we
        // don't want to "double dip", i.e. count the direct diffuse lighting twice.
        irradiance = (params.nextEventEstimation && ray.isDiffuse) ? Color::BLACK : li;

        // We always want to clamp camera rays directly on lights to prevent aliasing.
        clampIrradiance = clampIrradiance || isCameraRay;
    } else {
        Color direct = (params.nextEventEstimation && material.type == MaterialType::Diffuse)
                           ? computeDirectDiffuseLighting(intersection)
                           : Color::BLACK;
        irradiance = direct;
        if (remainingBounces > 0) {
            auto [nextRay, attenuation] = reflectOrRefract(intersection, ray.origin);
            Color indirect = attenuation * shootRay(nextRay, remainingBounces - 1);
            irradiance += indirect;
        }
    }

    return clampIrradiance ? irradiance.clamped() : irradiance;
}

std::optional<Intersection> Scene::findFirstIntersection(const Ray& ray) const {
    std::optional<Intersection> closestIntersection;

    for (const auto& intersectable : intersectables) {
        auto intersection = intersectable->intersect(ray);
        if (intersection &&
            (!closestIntersection ||
             intersection->distanceToRayOrigin < closestIntersection->distanceToRayOrigin)) {
            closestIntersection = intersection;
        }
    }

    return closestIntersection;
}

Color Scene::computeDirectDiffuseLighting(const Intersection& intersection) const {
    Color intersectionColor{0.0f};
    const Material& material = intersection.material.get();

    Color brdf = material.color / Utils::PI;
    for (const auto& light : lights) {
        PointSamplingResult sample = light->sampleForDirectLighting(intersection.location);
        Vector3 toLight = sample.point - intersection.location;
        Vector3 toLightNormalized = toLight.normalized();
        float lightDotN = toLightNormalized.dot(intersection.normal);

        if (lightDotN <= 0.0f) {
            continue;
        }

        // Checking for occlusion between the intersection and light
        Ray rayTowardsLight{intersection.location, toLightNormalized};
        rayTowardsLight.maxDist = (sample.point - intersection.location).length() -
                                  Ray::MIN_RAY_DIST; // preventing auto-occlusion

        if (findFirstIntersection(rayTowardsLight)) {
            continue;
        }

        Color li = sample.normal.dot(-toLightNormalized) * light->material.emission *
                   light->material.color;

        intersectionColor += brdf * li * lightDotN / (sample.pdf * toLight.lengthSquared());
    }
    return intersectionColor;
}