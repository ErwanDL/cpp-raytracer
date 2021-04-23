#include "scene.hpp"
#include "sampling.hpp"
#include "trace.hpp"

Color Scene::shootRay(const Ray& ray, int nBounces) const {
    const auto intersection = findFirstIntersection(ray);
    if (!intersection) {
        return skyColor;
    }
    Color direct = computeDirectLighting(intersection.value(), ray.origin);

    if (nBounces == 0) {
        return direct;
    }

    Color indirect = computeIndirectLighting(intersection.value(), ray.origin, nBounces - 1);

    return direct + indirect;
}

std::optional<Intersection> Scene::findFirstIntersection(const Ray& ray) const {
    std::optional<Intersection> closestIntersection;

    for (const auto& shape : shapes) {
        auto intersection = shape->intersect(ray);
        if (intersection &&
            (!closestIntersection ||
             intersection->distanceToRayOrigin < closestIntersection->distanceToRayOrigin)) {
            closestIntersection = intersection;
        }
    }

    return closestIntersection;
}

Color Scene::computeDirectLighting(const Intersection& intersection,
                                   const Point3& observerLocation) const {
    Color intersectionColor{0.0f};
    for (const auto& light : lights) {
        intersectionColor += light->illuminate(intersection, *this, observerLocation);
    }
    return intersectionColor;
}

Color Scene::computeIndirectLighting(const Intersection& intersection,
                                     const Point3& observerLocation, int nBounces) const {
    const Material& material = intersection.material;

    if (material.metal || Utils::random() <= material.specularity) {
        // reflect the ray : specular
        Vector3 perfectReflectionDirection =
            (intersection.location - observerLocation).normalized().reflected(intersection.normal);

        Vector3 sampledDirection =
            sampleHemisphere(perfectReflectionDirection, 1.0f / material.smoothness);
        // Reflected rays that would shoot beneath the surface are reflected about the
        // perfect reflection direction, back above the surface
        if (sampledDirection.dot(intersection.normal) < 0.0f) {
            sampledDirection = (-sampledDirection).reflected(perfectReflectionDirection);
        }
        Ray reflectedRay{intersection.location, sampledDirection};
        Color incomingLight = shootRay(reflectedRay, nBounces);

        return incomingLight * (material.metal ? material.albedo : 1.0f);
    } else {
        // refract the ray : diffuse
        Vector3 sampledDirection = sampleHemisphere(intersection.normal, 1.0f);
        Ray refractedRay{intersection.location, sampledDirection};
        Color incomingLight = shootRay(refractedRay, nBounces);

        return incomingLight * material.albedo;
    }
}
