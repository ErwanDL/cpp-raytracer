#include <iostream>

#include "intersectable.hpp"
#include "material.hpp"
#include "vector3.hpp"

int main() {
    Sphere s = Sphere(Point3(0.0f, 0.0f, 0.0f), 1.0f, Material::Diffuse(Color::WHITE));
    for (int i = 0; i < 10; ++i) {
        auto p = s.sampleVisibleSurface(Point3(2.0f, 0.0f, 0.0f));
        std::cout << "nice";
    }
}