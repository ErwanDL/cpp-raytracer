cmake_minimum_required(VERSION 3.0)

project(CppRaytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-g -Wall -Wextra -pedantic)
endif()

file(GLOB SRC_FILES
    "src/*.cpp"
)


list(REMOVE_ITEM SRC_FILES "${CMAKE_SOURCE_DIR}/src/main.cpp" "${CMAKE_SOURCE_DIR}/src/test.cpp")

find_package(OpenMP REQUIRED)
include_directories(lodepng)

add_executable(raytracer ${SRC_FILES} lodepng/lodepng.cpp src/main.cpp)

target_link_libraries(raytracer PRIVATE OpenMP::OpenMP_CXX)

add_executable(raytracer_debug ${SRC_FILES} lodepng/lodepng.cpp src/main.cpp)
add_executable(test ${SRC_FILES} src/test.cpp)
