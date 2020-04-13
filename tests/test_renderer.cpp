#include <catch2/catch.hpp>
#include <exception>
#include <iostream>
#include <utility>
#include <vector>

#include "../src/light.hpp"
#include "../src/material.hpp"
#include "../src/ray.hpp"
#include "../src/renderer.hpp"
#include "../src/vectors.hpp"

TEST_CASE("Deterministic Supersampler") {
    SECTION("Throws error if initialized with invalid value") {
        REQUIRE_THROWS_AS(DeterministicSupersampler(0), std::domain_error);
        REQUIRE_THROWS_AS(DeterministicSupersampler(-2), std::domain_error);
    }
    SECTION("Returns vector with only {0, 0} if rate is 1") {
        const DeterministicSupersampler superSampler{1};
        const auto offsets = superSampler.getSupersamplingOffsets();
        REQUIRE(offsets == std::vector<std::pair<float, float>>{{0.0f, 0.0f}});
    }

    SECTION("Returns correct values if rate is 2") {
        const DeterministicSupersampler superSampler{2};
        const auto offsets = superSampler.getSupersamplingOffsets();
        const std::vector<std::pair<float, float>> expected{
            {-0.25f, -0.25f}, {-0.25f, 0.25f}, {0.25f, -0.25f}, {0.25f, 0.25f}};
        REQUIRE(offsets.size() == expected.size());
        for (const auto expectedValue : expected) {
            REQUIRE(std::find(offsets.begin(), offsets.end(), expectedValue) !=
                    offsets.end());
        }
    }
}

TEST_CASE("Stochastic Supersampler") {
    SECTION("Throws error if initialized with invalid value") {
        REQUIRE_THROWS_AS(StochasticSupersampler(0), std::domain_error);
        REQUIRE_THROWS_AS(StochasticSupersampler(-2), std::domain_error);
    }
    SECTION("Returns correct number of values, within correct range") {
        const StochasticSupersampler superSampler{7};
        const auto offsets = superSampler.getSupersamplingOffsets();
        REQUIRE(offsets.size() == 7);

        std::cout << "Stochastic supersampler test values :\n";
        for (const auto offset : offsets) {
            std::cout << offset.first << '\t' << offset.second << '\n';
            REQUIRE((offset.first >= -0.5f && offset.first <= 0.5f));
            REQUIRE((offset.second >= -0.5f && offset.second <= 0.5f));
        }
    }
}

TEST_CASE("Renderer") {
    Scene scene;
    const Plane leftPlane{Point3(-1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f),
                          Lambertian(Color(1.0f, 0.0f, 0.0f), 0.5f)};
    const Plane rightPlane{Point3(1.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f),
                           Lambertian(Color(0.0f, 0.0f, 1.0f), 0.5f)};
    scene.addShape(leftPlane);
    scene.addShape(rightPlane);

    LightRack lights;
    const AmbientLight ambientLight{Color(0.5f)};
    lights.addLight(ambientLight);

    const Renderer renderer{scene, lights, 400, 300,
                            DeterministicSupersampler(1)};
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