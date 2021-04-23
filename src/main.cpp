#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "save_render.hpp"
#include "scene.hpp"
#include "shape.hpp"
#include "trace.hpp"
#include "utils.hpp"
#include "vector3.hpp"

int main() {
    std::vector<std::shared_ptr<Shape>> shapes{{
        std::make_shared<Plane>(Point3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 1.0f, 0.0f), // floor
                                Color::WHITE),
        std::make_shared<Plane>(Point3(0.0f, 0.0f, -10.0f),
                                Vector3(0.0f, 0.0f, -1.0f), // front wall
                                Color::WHITE),
        std::make_shared<Plane>(Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), // back wall
                                Color::WHITE),
        std::make_shared<Plane>(Point3(0.0f, 9.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), // ceiling
                                Color::WHITE),
        std::make_shared<Plane>(Point3(-4.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), // left wall
                                Material(Color(0.9f, 0.4f, 0.4f))),
        std::make_shared<Plane>(Point3(4.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), // right wall
                                Material(Color(0.4f, 0.9f, 0.4f))),
        std::make_shared<Sphere>(Point3(-2.0f, 1.0f, -6.0f), 1.0f, // glossy sphere
                                 Material(Color(0.8f, 0.6f, 0.4f), 10.0f, true)),
        std::make_shared<Sphere>(Point3(0.0f, 1.0f, -6.0f), 1.0f, // diffuse sphere
                                 Material(Color(0.35f, 0.45f, 0.8f))),
        std::make_shared<Sphere>(Point3(2.0f, 1.0f, -6.0f), 1.0f, // mirror sphere
                                 Material(Color(0.5f), 100000.0f, true)),
    }};
    std::vector<std::shared_ptr<Light>> lights{
        std::make_shared<AmbientLight>(Color(0.2f)),
        std::make_shared<PointLight>(Point3(3.0f, 7.0f, -3.0f), Color(0.4f)),
        std::make_shared<PointLight>(Point3(-3.0f, 7.0f, -3.0f), Color(0.4f)),
    };

    Scene scene{shapes, lights};

    RenderParams params{720, 480, 3, 20};

    PerspectiveCamera camera{Point3(0.0f, 4.0f, 0.0f), Vector3(0.0f, 0.0f, -10.0f), Math::PI / 4};

    auto render = rayTrace(camera, scene, params);
    saveRenderToPPM(render, "scene.ppm");

    return 0;
}
