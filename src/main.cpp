#include <iostream>
#include <string>

#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "renderer.hpp"
#include "shape.hpp"
#include "utils.hpp"
#include "vectors.hpp"

int main() {
    Scene scene{};

    const Plane floor(Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f),
                      Material(Color(0.3f, 0.5f, 0.3f), 0.0f));
    scene.addShape(&floor);

    const Sphere blueSphere{Point3(-2.0f, 1.0f, -5.0f), 0.95f,
                            Material(Color(0.4f, 0.5f, 0.8f), 0.05f)};
    scene.addShape(&blueSphere);

    const Sphere metalSphere{Point3(0.0f, 1.0f, -5.0f), 0.95f,
                             Material(Color(0.0f), 0.8f, 100.0f)};
    scene.addShape(&metalSphere);

    const Sphere metalSphere2{Point3(-2.0f, 1.0f, -7.0f), 0.95f,
                              Material(Color(0.0f), 0.8f, 100.0f)};
    scene.addShape(&metalSphere2);

    const Sphere yellowSphere{Point3(2.0f, 1.5f, -5.0f), 0.95f,
                              Material(Color(0.7f, 0.7f, 0.4f), 0.0f)};
    scene.addShape(&yellowSphere);

    const Sphere redSphere{Point3(-0.5f, 2.0f, -2.0f), 0.95f,
                           Material(Color(0.65f, 0.35f, 0.35f), 0.0f)};
    scene.addShape(&redSphere);

    const Sphere orangeSphere{Point3(0.5f, 2.0f, -7.5f), 0.95f,
                              Material(Color(0.7f, 0.45f, 0.35f), 0.0f)};
    scene.addShape(&orangeSphere);

    LightRack lightRack{Color(0.7f)};

    const PointLight light{Point3(2.0f, 14.0f, 0.0f), Color(0.8f)};
    lightRack.addLight(light);

    constexpr int width{720};
    constexpr int height{480};

    PerspectiveCamera camera{
        Point3(0.0f, 10.0f, 0.0f), Vector3(0.0f, 1.0f, -5.0f),
        Vector3(0.0f, 1.0f, 0.0f), Math::PI / 8,
        static_cast<float>(width) / static_cast<float>(height)};

    Renderer renderer{scene, lightRack, width, height, 1.0f, 1.8f};

    auto render = renderer.rayTrace(camera);
    renderer.saveRenderer(render, "scene.ppm");

    return 0;
}
