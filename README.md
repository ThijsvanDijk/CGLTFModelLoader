# CGLTFModelLoader
This is a GLTF Model Loader written in C, which is focused on reducing mesh loading times.
Currently only supported on Linux, I'm working on easy cross-platform building for Windows.

## Features
- Supports both GLTF/GLB formats
- Supports a custom GLB buffer format designed to be read directly from storage into OpenGL
- Reads both position and normal data for now, working on including texture coordinates now

## Dependencies
- CGLM for fast math
- ZZZJSON fastest JSON parser I could find
- GLAD for loading OpenGL functions
- GLFW for managing windows and user input

## Build
The library can be build with cmake like any other:
```bash
mkdir build && cd build
cmake ../
cmake --build .
```
This will generate a .a file for use with gcc compiler

