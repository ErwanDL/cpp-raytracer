#include <iostream>
#include "vector3.hpp"

int main()
{
    Vector3 v1{0.0f, 1.0f, 2.0f};

    v1.setX(1.0f);

    std::cout << v1.length() << '\n';

    return 0;
}