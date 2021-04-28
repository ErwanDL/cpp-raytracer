#include "scene.hpp"
#include "ray.hpp"
#include "sampling.hpp"
#include "trace.hpp"
#include <cmath>

Color Scene::shootRay(const Ray& ray, int remainingBounces, bool isCameraRay) const {
    const auto optIntersection = findFirstIntersection(ray);

    if (!optIntersection) {
        return skyColor;
    }

    const Intersection& intersection = optIntersection.value();
    const Material& material = intersection.material.get();

    if (material.emission > 0.0f) { // If hit object is a light
        // If camera ray : clamp value (to prevent aliasing) and terminate path.
        if (isCameraRay) {
            return (material.color * material.emission).clamped();
        }
        // With NEE activated, we want to avoid counting direct diffuse lighting twice.
        return (ray.isSpecular || !params.nextEventEstimation) * material.color * material.emission;
    }

    Color direct =
        params.nextEventEstimation ? computeDirectDiffuseLighting(intersection) : Color::BLACK;

    if (remainingBounces == 0) {
        return direct;
    }

    return direct + computeIndirectLighting(intersection, ray.origin, remainingBounces - 1);
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

    // Metals have no diffuse component.
    if (material.metal) {
        return intersectionColor;
    }

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

Color Scene::computeIndirectLighting(const Intersection& intersection,
                                     const Point3& observerLocation, int remainingBounces) const {
    const Material& material = intersection.material;

    if (material.metal || Utils::random() <= material.specularity) {
        // reflect the ray : specular
        Vector3 perfectReflectionDirection =
            (intersection.location - observerLocation).normalized().reflected(intersection.normal);

        Vector3 sampledDirection =
            sampleHemisphereGlossy(perfectReflectionDirection, 1.0f / material.smoothness);
        // Reflected rays that would shoot beneath the surface are reflected about the
        // perfect reflection direction, back above the surface
        if (sampledDirection.dot(intersection.normal) < 0.0f) {
            sampledDirection = (-sampledDirection).reflected(perfectReflectionDirection);
        }
        Ray reflectedRay{intersection.location, sampledDirection, true};
        Color incomingLight = shootRay(reflectedRay, remainingBounces);

        return incomingLight * (material.metal ? material.color : 1.0f);
    } else {
        // refract the ray : diffuse
        Color brdf = material.color / Utils::PI;
        auto sample = sampleHemisphereCosineWeighted(intersection.normal);
        Ray refractedRay{intersection.location, sample.direction};
        float refractedDotN = sample.direction.dot(intersection.normal);
        Color incomingLight = shootRay(refractedRay, remainingBounces);

        return incomingLight * brdf * refractedDotN / sample.pdf;
    }
}
