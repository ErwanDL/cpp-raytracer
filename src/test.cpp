#include <iostream>

#include "vector3.hpp"

int main() {
    for (int i = 0; i < 10; ++i) {
        auto v = Vector3(1.0f, 0.0f, 0.0f).sampleHemisphere(0.5f);
        std::cout << v << "\t" << v.length() << std::endl;
    }
}