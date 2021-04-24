# Raytracing in C++

This is a personal project in which I incrementally build a raytracer, that I started to learn modern C++ and computer graphics. Going back at it from time to time !

![Example image](./scene.png)

## Running the project

-   Requirements : CMake, Make, a modern C++ compiler (tested with G++ >= 9), OpenMP library (for multithreading).
-   Generate the Makefile by doing :
    ```bash
    $ mkdir build
    $ cd build
    $ cmake ..
    ```
-   You can then compile and run the project with :
    ```bash
    $ make raytracer
    $ ./raytracer
    ```

If you want to change the rendered scene, you can do so in `src/main.cpp`.

## Features supported

-   [x] Global illumination (path tracing, but also using direct light sampling)
-   [x] Soft shadows (non-ponctual lights)
-   [x] Single PBR Material (inspired by Disney's & Blender's Principled BSDF although much less complete)
-   [x] Multithreading (using OpenMP)
