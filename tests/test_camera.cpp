#include <catch2/catch.hpp>
#include <cmath>

#include "../src/camera.hpp"
#include "../src/ray.hpp"
#include "../src/utils.hpp"
#include "../src/vectors.hpp"

TEST_CASE("PerspectiveCamera") {
    SECTION("makeRay produces the correct Ray") {
        const PerspectiveCamera cam{Point3(0.0f), Vector3(0.0f, 1.0f, 0.0f),
                                    Vector3(0.0f, 0.0f, 1.0f), Math::PI / 6,
                                    1.0f};

        const auto ray = cam.makeRay(Point2(1.0f, 0.0f));

        REQUIRE(ray.origin == Point3(0.0f));
        REQUIRE(ray.direction.normalized() ==
                Vector3(0.5f, std::sqrt(3.0f) / 2.0f, 0.0f).normalized());
    }
}