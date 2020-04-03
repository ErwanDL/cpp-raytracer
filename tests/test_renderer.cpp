#include <catch2/catch.hpp>

#include "../src/light.hpp"
#include "../src/material.hpp"
#include "../src/ray.hpp"
#include "../src/renderer.hpp"
#include "../src/vectors.hpp"

TEST_CASE("Renderer") {
    const Plane leftPlane{Point3(-1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f),
                          Material(Color(1.0f, 0.0f, 0.0f), 0.5f)};
    const Plane rightPlane{Point3(1.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f),
                           Material(Color(0.0f, 0.0f, 1.0f), 0.5f)};
    Scene scene;
    scene.addShape(leftPlane);
    scene.addShape(rightPlane);
    const LightRack lights{Color(0.5f)};
    const Renderer renderer{scene, lights, 400, 300};

    const Ray rayTowardsLeftPlane{Point3(0.0f), Vector3(-1.0f, 0.0f, -2.0f)};

    SECTION("Shoot ray works correctly with no bounces") {
        const Color expectedColor{0.5f, 0.0f, 0.0f};
        REQUIRE(expectedColor ==
                renderer.shootRayRecursively(rayTowardsLeftPlane, 0));
    }

    SECTION("Shoot ray works correctly with 1 bounce") {
        const Color expectedColor{0.5f, 0.0f, 0.25f};
        REQUIRE(expectedColor ==
                renderer.shootRayRecursively(rayTowardsLeftPlane, 1));
    }

    SECTION("Shoot ray works correctly with 2 bounces") {
        const Color expectedColor{0.625f, 0.0f, 0.25f};
        REQUIRE(expectedColor ==
                renderer.shootRayRecursively(rayTowardsLeftPlane, 2));
    }
}