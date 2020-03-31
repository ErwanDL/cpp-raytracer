#include <catch2/catch.hpp>

#include "../src/ray.hpp"
#include "../src/shape.hpp"
#include "../src/vectors.hpp"

TEST_CASE("Plane") {
    SECTION("intersect returns false if plane is parallel to ray") {
        const Plane plane{Point3(1.0f, 0.5f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)};
        Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        bool hasIntersected = plane.intersect(ray);

        REQUIRE(hasIntersected == false);
    }
    SECTION(
        "intersect returns false if plane isnt parallel but is behind ray "
        "origin") {
        const Plane plane{Point3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f)};
        Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        bool hasIntersected = plane.intersect(ray);

        REQUIRE(hasIntersected == false);
    }

    SECTION(
        "intersect returns true and updates intersection if plane isnt "
        "parallel and is in front of ray") {
        const Plane plane{Point3(0.0f, 2.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f)};
        Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        bool hasIntersected = plane.intersect(ray);

        REQUIRE(hasIntersected == true);
        REQUIRE(ray.pointOfIntersection() == Point3(0.0f, 2.0f, 0.0f));
    }
}

TEST_CASE("Sphere") {
    SECTION("intersect returns false if sphere is not on the ray's path") {
        const Sphere sphere{Point3(-1.5f, 3.0f, 0.0f), 1.0f};
        Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        bool hasIntersected = sphere.intersect(ray);

        REQUIRE(hasIntersected == false);
    }

    SECTION("intersect returns false if sphere is fully behind ray origin") {
        const Sphere sphere{Point3(-1.5f, -2.0f, 0.0f), 1.0f};
        Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        bool hasIntersected = sphere.intersect(ray);

        REQUIRE(hasIntersected == false);
    }

    SECTION(
        "intersect returns true and updates intersection if sphere is on the "
        "ray's path and in front of ray origin") {
        const Sphere sphere{Point3(-0.5f, 2.0f, 0.0f), 1.0f};
        Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        bool hasIntersected = sphere.intersect(ray);

        float expectedIntersectionY = 2.0f - std::sqrt(3.0f) / 2.0f;
        REQUIRE(hasIntersected == true);
        REQUIRE(ray.pointOfIntersection() ==
                Point3(0.0f, expectedIntersectionY, 0.0f));
    }

    SECTION(
        "intersect returns true and updates intersection if ray origin is "
        "inside the sphere") {
        const Sphere sphere{Point3(0.0f, -0.5f, 0.0f), 1.0f};
        Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        bool hasIntersected = sphere.intersect(ray);

        REQUIRE(hasIntersected == true);
        REQUIRE(ray.pointOfIntersection() == Point3(0.0f, 0.5f, 0.0f));
    }
}