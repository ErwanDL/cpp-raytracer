#include <iostream>
#include <string>

#include "camera.hpp"
#include "image.hpp"
#include "light.hpp"
#include "material.hpp"
#include "shape.hpp"
#include "utils.hpp"
#include "vectors.hpp"

int main() {
    constexpr int width{20};
    constexpr int height{15};

    Image img{width, height, 1.0f, 1.8f};

    PerspectiveCamera camera{
        Point3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, -1.0f),
        Vector3(0.0f, 1.0f, 0.0f), Math::PI / 6,
        static_cast<float>(width) / static_cast<float>(height)};

    Scene scene{};

    Plane floor(Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f),
                Material(Color(0.3f, 0.5f, 0.3f), 0.1f, 5.0f));
    scene.addShape(&floor);

    Sphere sphere{Point3(1.0f, 2.5f, -5.0f), 0.7f,
                  Material(Color(0.5f, 0.5f, 0.8f))};
    scene.addShape(&sphere);

    LightRack lightRack{0.6f};

    PointLight light{Point3(2.0f, 4.0f, 0.0f), Color(0.5f)};
    lightRack.addLight(&light);
    // constexpr int width{1280};
    // constexpr int height{720};

    // Image img{width, height, 1.0f, 1.8f};

    // PerspectiveCamera camera{
    //     Point3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, -1.0f),
    //     Vector3(0.0f, 1.0f, 0.0f), Math::PI / 6,
    //     static_cast<float>(width) / static_cast<float>(height)};

    // Scene scene{};

    // Plane floor(Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f),
    //             Material(Color(0.3f, 0.5f, 0.3f), 0.1f, 5.0f));
    // scene.addShape(&floor);

    // Plane backwall(Point3(0.0f, 0.0f, -15.0f), Vector3(0.0f, 0.0f, 1.0f),
    //                Material(Color(0.5f, 0.3f, 0.3f), 0.1f, 5.0f));
    // scene.addShape(&backwall);

    // Sphere sphere{Point3(1.0f, 2.5f, -5.0f), 0.7f,
    //               Material(Color(0.5f, 0.5f, 0.8f))};
    // scene.addShape(&sphere);
    // Sphere sphere2{Point3(-1.5f, 0.5f, -2.5f), 0.5f,
    //                Material(Color(0.7f, 0.7f, 0.0f))};
    // scene.addShape(&sphere2);

    // LightRack lightRack{0.6f};

    // PointLight light{Point3(2.0f, 4.0f, 0.0f), Color(0.5f)};
    // lightRack.addLight(&light);

    // PointLight light2{Point3(-1.0f, 10.0f, -5.0f), Color(0.25f)};
    // lightRack.addLight(&light2);

    auto render = img.rayTrace(camera, scene, lightRack);
    img.saveImage(render, "scene.ppm");

    return 0;
}
