#include <catch2/catch.hpp>
#include <cmath>

#include "../src/light.hpp"
#include "../src/material.hpp"
#include "../src/ray.hpp"
#include "../src/shape.hpp"
#include "../src/vectors.hpp"

TEST_CASE("PointLight") {
    const PointLight light{Point3(0.0f, 5.0f, 0.0f), Color(0.5f)};
    const Point3 observerLocation(Point3(0.0f));
    SECTION("lights as black if normal is opposed") {
        const Intersection intersection{Point3(0.0f, 1.0f, 0.0f),
                                        Vector3(1.0f, -2.0f, 0.0f), 1.0f,
                                        Material(Color(0.5f))};

        const Color resultingColor{
            light.illuminate(intersection, Scene(), observerLocation)};

        REQUIRE(Color(0.0f) == resultingColor);
    }

    SECTION("lights as black if normal is orthogonal") {
        const Intersection intersection{Point3(0.0f, 1.0f, 0.0f),
                                        Vector3(1.0f, -1.0f, 0.0f), 1.0f,
                                        Material(Color(0.5f))};

        const Color resultingColor{
            light.illuminate(intersection, Scene(), observerLocation)};

        REQUIRE(Color(0.0f) == resultingColor);
    }

    SECTION("lights as black if a plane is between") {
        const Intersection intersection{Point3(0.0f, 1.0f, 0.0f),
                                        Vector3(0.0f, 1.0f, 0.0f), 1.0f,
                                        Material(Color(0.5f))};

        Scene scene;
        Plane plane{Point3(0.0f, 3.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};
        scene.addShape(&plane);

        const Color resultingColor{
            light.illuminate(intersection, scene, observerLocation)};

        REQUIRE(Color(0.0f) == resultingColor);
    }

    SECTION(
        "lights as diffuse color times light color if normal faces light and "
        "no specular") {
        const Intersection intersection{
            Point3(0.0f, 5.0f, 3.0f), Vector3(0.0f, 0.0f, -1.0f), 1.0f,
            Material(Color(1.0f, 0.5f, 0.0f), 0.0f)};

        const Color resultingColor{
            light.illuminate(intersection, Scene(), observerLocation)};

        REQUIRE(Color(0.5f, 0.25f, 0.0f) == resultingColor);
    }

    SECTION(
        "lights as full light color + (cosine * diffuse) if cam at "
        "exact reflection angle and fully specular and reflective") {
        // the Z value is chosen so that the cosine of the angle between the
        // normal and the light is 0.5
        const Intersection intersection{
            Point3(0.0f, 2.5f, 2.5f / std::sqrt(3.0f)),
            Vector3(0.0f, 0.0f, -1.0f), 1.0f,
            // if energy was conserved, the reflectiveness should influence
            // the intensity of the specular highlights -> not the case ATM
            Material(Color(1.0f, 0.5f, 0.0f), 1.0f, 60.0f)};

        const Color resultingColor{
            light.illuminate(intersection, Scene(), observerLocation)};

        const Color expectedDiffuse{0.25f, 0.125f, 0.0f};
        const Color expectedSpecular{0.5f};
        REQUIRE((expectedDiffuse + expectedSpecular) == resultingColor);
    }
}