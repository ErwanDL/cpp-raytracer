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

    Image img{width, height};

    PerspectiveCamera camera{
        Point3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, -1.0f),
        Vector3(0.0f, 1.0f, 0.0f), Math::PI / 4,
        static_cast<float>(width) / static_cast<float>(height)};

    Scene scene{};

    Plane floor(Point3(0.0f, -10.0f, 0.0f), Vector3(0.0f, 1.0f, -0.2f),
                Color(0.5f, 1.0f, 0.5f));
    scene.addShape(&floor);

    Sphere sphere{Point3(0.0f, 3.0f, -5.0f), 1.0f, Color(0.5f, 0.5f, 1.0f)};
    scene.addShape(&sphere);

    Light light{Point3(3.0f, 3.0f, 0.0f), Color(1.0f)};

    img.rayTrace(camera, scene, light);

    img.saveImage("scene.ppm");

    return 0;
}