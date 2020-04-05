#include <catch2/catch.hpp>

#include "../src/ray.hpp"
#include "../src/shape.hpp"
#include "../src/utils.hpp"
#include "../src/vectors.hpp"

TEST_CASE("Plane") {
    SECTION("intersect returns false if plane is parallel to ray") {
        const Plane plane{Point3(1.0f, 0.5f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = plane.intersect(ray);

        REQUIRE(!intersection);
    }
    SECTION(
        "intersect returns false if plane isnt parallel but is behind ray "
        "origin") {
        const Plane plane{Point3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f)};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = plane.intersect(ray);

        REQUIRE(!intersection);
    }

    SECTION(
        "intersect returns true and updates intersection if plane isnt "
        "parallel and is in front of ray") {
        const Plane plane{Point3(0.0f, 2.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f)};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = plane.intersect(ray);

        REQUIRE(intersection);
        REQUIRE(intersection->location == Point3(0.0f, 2.0f, 0.0f));
    }

    SECTION("intersect returns reference to intersected shape") {
        const Plane plane{Point3(0.0f, 2.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f)};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = plane.intersect(ray);

        REQUIRE(intersection->intersectedShape == &plane);
    }

    const Sphere intersectedShape{Point3(0.0f), 1.0f};
    const Intersection i{Point3(0.0f, -1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f),
                         1.0f, intersectedShape.getMaterial(),
                         intersectedShape};
    SECTION(
        "distanceTo returns infinity if plane isn't facing the intersection") {
        const Plane plane{Point3(1.0f, 1.5f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};
        const Plane planeMirorredNormal{Point3(1.0f, 1.5f, 0.0f),
                                        Vector3(0.0f, 1.0f, 0.0f)};

        const float distance = plane.distanceTo(i);

        REQUIRE(distance == Math::INF);
    }

    SECTION(
        "distanceTo returns correct distance if shape is facing the "
        "intersection") {
        const Plane plane{Point3(1.0f, -1.5f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        const float distance = plane.distanceTo(i);

        REQUIRE(distance == Approx(0.5f));
    }
}

TEST_CASE("Sphere") {
    SECTION("intersect returns false if sphere is not on the ray's path") {
        const Sphere sphere{Point3(-1.5f, 3.0f, 0.0f), 1.0f};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = sphere.intersect(ray);

        REQUIRE(!intersection);
    }

    SECTION("intersect returns false if sphere is fully behind ray origin") {
        const Sphere sphere{Point3(-1.5f, -2.0f, 0.0f), 1.0f};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = sphere.intersect(ray);

        REQUIRE(!intersection);
    }

    SECTION(
        "intersect returns true and updates intersection if sphere is on the "
        "ray's path and in front of ray origin") {
        const Sphere sphere{Point3(-0.5f, 2.0f, 0.0f), 1.0f};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = sphere.intersect(ray);

        float expectedIntersectionY = 2.0f - std::sqrt(3.0f) / 2.0f;

        REQUIRE(intersection);
        REQUIRE(intersection->location ==
                Point3(0.0f, expectedIntersectionY, 0.0f));
    }

    SECTION(
        "intersect returns true and updates intersection if ray origin is "
        "inside the sphere") {
        const Sphere sphere{Point3(0.0f, -0.5f, 0.0f), 1.0f};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = sphere.intersect(ray);

        REQUIRE(intersection);
        REQUIRE(intersection->location == Point3(0.0f, 0.5f, 0.0f));
    }

    SECTION("intersect returns ref to intersected shape") {
        const Sphere sphere{Point3(0.0f, -0.5f, 0.0f), 1.0f};
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};

        auto intersection = sphere.intersect(ray);

        REQUIRE(intersection->intersectedShape == &sphere);
    }

    const Sphere intersectedShape{Point3(0.0f), 1.0f};
    const Intersection i{Point3(0.0f, -1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f),
                         1.0f, intersectedShape.getMaterial(),
                         intersectedShape};

    SECTION(
        "distanceTo returns inf if sphere isn't facing intersection normal") {
        const Sphere sphere{Point3(2.0f, 4.0f, 0.0f), 1.0f};

        const float distance = sphere.distanceTo(i);

        REQUIRE(distance == Math::INF);
    }

    SECTION(
        "distanceTo returns correct distance if sphere is facing intersection "
        "normal") {
        const Sphere sphere{Point3(3.0f, -5.0f, 0.0f), 1.0f};

        const float distance = sphere.distanceTo(i);

        REQUIRE(distance == Approx(4.0f));
    }
}

TEST_CASE("Scene") {
    SECTION("intersect returns false if nothing in scene") {
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};
        Scene scene;

        auto intersection = scene.intersect(ray);

        REQUIRE(!intersection);
    }

    SECTION("intersect returns false if no shape intersects ray") {
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};
        Scene scene;
        const Sphere sphere{Point3(3.0f, 5.0f, 0.0f), 1.0f};
        const Plane plane{Point3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f)};
        scene.addShape(sphere);
        scene.addShape(plane);

        auto intersection = scene.intersect(ray);

        REQUIRE(!intersection);
    }

    SECTION("intersect returns correct intersection if one shape intersects") {
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};
        Scene scene;
        const Sphere sphere{Point3(3.0f, 5.0f, 0.0f), 1.0f};
        const Plane plane{Point3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f)};
        scene.addShape(sphere);
        scene.addShape(plane);

        auto intersection = scene.intersect(ray);

        REQUIRE(intersection);
        REQUIRE(intersection->location == Point3(0.0f, 1.0f, 0.0f));
    }

    SECTION(
        "intersect returns closest intersection if several shapes intersect") {
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};
        Scene scene;
        const Sphere sphere{Point3(0.0f, 5.0f, 0.0f), 1.0f};
        const Plane plane{Point3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f)};
        scene.addShape(sphere);
        scene.addShape(plane);

        auto intersection = scene.intersect(ray);

        REQUIRE(sphere.intersect(ray));
        REQUIRE(intersection);
        REQUIRE(intersection->location == Point3(0.0f, 1.0f, 0.0f));
    }

    SECTION("intersect returns ref to intersected shape") {
        const Ray ray{Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)};
        Scene scene;
        const Plane plane{Point3(0.0f, 2.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f)};
        scene.addShape(plane);

        auto intersection = scene.intersect(ray);

        REQUIRE(intersection->intersectedShape == &plane);
    }

    const Sphere intersectedShape{Point3(0.0f), 1.0f};
    const Intersection i{Point3(0.0f, -1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f),
                         1.0f, intersectedShape.getMaterial(),
                         intersectedShape};

    SECTION(
        "distanceTo returns inf if the only shape is the intersection's "
        "owner") {
        Scene scene;
        scene.addShape(intersectedShape);

        const float distance = scene.distanceTo(i);

        REQUIRE(distance == Math::INF);
    }

    SECTION(
        "distanceTo returns inf if the other shapes are not facing the "
        "intersection normal") {
        Scene scene;
        scene.addShape(intersectedShape);
        const Sphere sphere{Point3(3.0f, 4.0f, -1.0f), 1.5f};
        scene.addShape(sphere);

        const float distance = scene.distanceTo(i);

        REQUIRE(distance == Math::INF);
    }

    SECTION(
        "distanceTo returns closest distance if there are shapes facing the "
        "intersection normal") {
        Scene scene;
        scene.addShape(intersectedShape);
        const Sphere sphere{Point3(0.0f, -4.0f, 0.0f), 1.5f};
        scene.addShape(sphere);
        const Plane plane{Point3(6.0f, -8.0f, 2.0f), Vector3(0.5, 1.0f, 0.1f)};

        const float distance = scene.distanceTo(i);

        REQUIRE(distance == Approx(1.5f));
    }
}