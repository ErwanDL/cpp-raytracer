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
    Material wallMat{Color(0.9f)};
    std::vector<std::shared_ptr<Shape>> shapes{{
        std::make_shared<Plane>(Point3(0.0f, 0.0f, -10.0f), Vector3(0.0f, 1.0f, 0.0f), // floor
                                Color(0.35f, 0.55f, 0.3f)),
        // std::make_shared<Plane>(Point3(0.0f, 0.0f, -10.0f),
        //                         Vector3(0.0f, 0.0f, -1.0f), // front wall
        //                         wallMat),
        // std::make_shared<Plane>(Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), // back
        // wall
        //                         wallMat),
        // std::make_shared<Plane>(Point3(0.0f, 9.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), // ceiling
        //                         wallMat),
        // std::make_shared<Plane>(Point3(-4.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), // left
        // wall
        //                         Material(Color(0.9f, 0.3f, 0.3f))),
        // std::make_shared<Plane>(Point3(4.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), // right
        // wall
        //                         Material(Color(0.3f, 0.9f, 0.3f))),
        std::make_shared<Sphere>(Point3(-2.0f, 1.0f, -6.0f), 1.0f, // glossy sphere
                                 Material(Color(0.9f, 0.6f, 0.4f), 25.0f, true)),
        std::make_shared<Sphere>(Point3(0.0f, 1.0f, -7.0f), 1.0f, // diffuse sphere
                                 Material(Color(0.3, 0.4f, 0.9f), 10.0f)),
        std::make_shared<Sphere>(Point3(2.0f, 1.0f, -6.0f), 1.0f, // mirror sphere
                                 Material(Color(0.7f), 100000.0f, true)),
    }};
    std::vector<std::shared_ptr<Light>> lights{
        std::make_shared<SphereLight>(Point3(1.0f, 3.0f, -4.0f), Color(0.8f, 0.8f, 0.7f), 0.25f),
    };

    Scene scene{shapes, lights, Color::WHITE};

    RenderParams params{1280, 720, 2, 100};

    PerspectiveCamera camera{Point3(0.0f, 3.0f, 0.0f), Point3(0.0f, 0.5f, -7.0f), Utils::PI / 4};

    auto render = rayTrace(camera, scene, params);
    saveRenderToPPM(render, "scene.ppm");

    return 0;
}
