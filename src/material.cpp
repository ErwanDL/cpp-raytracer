#include "material.hpp"
#include "color.hpp"
#include "intersection.hpp"
#include "ray.hpp"
#include "sampling.hpp"
#include "utils.hpp"
#include "vector3.hpp"
#include <cmath>

float schlickReflectance(float IOR, float cosTheta) {
    float R0 = Utils::sqr((IOR - 1.0f) / (IOR + 1.0f));
    return R0 + (1.0f - R0) * std::pow(1.0f - cosTheta, 5);
}

std::pair<Ray, Color> reflectOrRefract(const Intersection& intersection, const Point3& rayOrigin) {
    const Material& material = intersection.material;
    Vector3 fromObsDir = (intersection.location - rayOrigin).normalized();
    Vector3 normal =
        intersection.normal.dot(fromObsDir) > 0.0f ? -intersection.normal : intersection.normal;
    float cosTheta = -normal.dot(fromObsDir);
    float inIOROverOutIOR = intersection.backFace ? material.IOR : 1.0f / material.IOR;

    bool reflect = (material.type == MaterialType::Refractive) &&
                   (std::sqrt(1 - Utils::sqr(cosTheta)) * inIOROverOutIOR > 1.0f) &&
                   schlickReflectance(material.IOR, cosTheta) > Utils::random();

    // Specular reflection
    if (material.type == MaterialType::Metal || reflect) {
        Vector3 perfectReflectionDirection = fromObsDir.reflected(intersection.normal);

        Vector3 sampledDirection =
            sampleHemisphereGlossy(perfectReflectionDirection, 1.0f / material.smoothness);
        // Reflected rays that would shoot beneath the surface are reflected about the
        // perfect reflection direction, back above the surface
        if (sampledDirection.dot(intersection.normal) < 0.0f) {
            sampledDirection = (-sampledDirection).reflected(perfectReflectionDirection);
        }
        Ray reflectedRay{intersection.location, sampledDirection};
        Color attenuation = material.type == MaterialType::Metal ? material.color : Color::WHITE;

        return {reflectedRay, attenuation};

        // Refraction
    } else if (material.type == MaterialType::Refractive) {
        Vector3 refractedTangent = inIOROverOutIOR * (fromObsDir + cosTheta * normal);
        Vector3 refractedNormal = -std::sqrt(1.0f - refractedTangent.lengthSquared()) * normal;
        Vector3 refractedDir = refractedNormal + refractedTangent;
        Ray refractedRay{intersection.location, refractedDir};

        return {refractedRay, Color::WHITE};

        // Diffuse reflection
    } else {
        Color brdf = material.color / Utils::PI;
        auto sample = sampleHemisphereCosineWeighted(intersection.normal);
        Ray reflectedRay{intersection.location, sample.direction, true};
        Color attenuation = brdf * sample.direction.dot(intersection.normal) / sample.pdf;

        return {reflectedRay, attenuation};
    }
}