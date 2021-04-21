#include <iostream>

#include "utils.hpp"

int main() {
    for (int i = 0; i < 10; ++i) {
        auto v = Math::sampleHemisphere(Vector3(1.0f, 0.0f, 0.0f), 0.5f);
        std::cout << v << "\t" << v.length() << std::endl;
    }
}