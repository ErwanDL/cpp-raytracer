#include <iostream>
#include "camera.hpp"
#include "image.hpp"
#include "light.hpp"
#include "material.hpp"
#include "math_utils.hpp"
#include "shape.hpp"
#include "vectors.hpp"

int main() {
    constexpr int width{720};
    constexpr int height{480};

    Image img{width, height, 1.0f, 2.0f};

    PerspectiveCamera camera{
        Point3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, -1.0f),
        Vector3(0.0f, 1.0f, 0.0f), Math::PI / 4,
        static_cast<float>(width) / static_cast<float>(height)};

    Scene scene{};

    Plane floor(Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, -0.2f),
                Color(0.5f, 1.0f, 0.5f));
    scene.addShape(&floor);

    Sphere sphere{Point3(0.0f, 3.0f, -5.0f), 1.0f, Color(0.5f, 0.5f, 1.0f)};
    scene.addShape(&sphere);

    LightRack lightRack{};

    Spotlight light{Point3(4.0f, 5.0f, 0.0f), Color(0.6f)};
    lightRack.addLight(&light);

    img.rayTrace(camera, scene, lightRack);

    img.saveImage("scene.ppm");

    return 0;
}