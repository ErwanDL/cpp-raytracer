cmake_minimum_required(VERSION 3.0)

project(CppRaytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC_FILES
    "src/*.cpp"
)

list(REMOVE_ITEM SRC_FILES "${CMAKE_SOURCE_DIR}/src/main.cpp")

file(GLOB TEST_FILES
    "tests/*.cpp"
)

add_executable(raytracer ${SRC_FILES} src/main.cpp)

add_executable(runtests ${SRC_FILES} ${TEST_FILES})

find_package(Catch2 REQUIRED)
target_link_libraries(runtests Catch2::Catch2)