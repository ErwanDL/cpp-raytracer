#include <catch2/catch.hpp>

#include "../src/ray.hpp"
#include "../src/vectors.hpp"

TEST_CASE("Ray") {
    SECTION("pointAtDistance returns correct point") {
        const Ray ray{Point3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 2.0f)};

        const auto point = ray.pointAtDistance(1.5f);

        REQUIRE(point == Point3(2.5f, 1.5f, 3.0f));
    }
}
