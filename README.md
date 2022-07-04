# CGLTFModelLoader
This is a GLTF Model Loader written in C, which is focused on reducing mesh loading times.
Currently only supported on Linux, I'm working on easy cross-platform building for Windows (now done!).

## Features
- Supports both GLTF/GLB formats
- Supports a custom GLB buffer format designed to be read directly from storage into OpenGL
- Reads both position and normal data for now, working on including texture coordinates now

## Dependencies
- CGLM for quick maths
- ZZZJSON fastest JSON parser I could find
- GLAD for loading OpenGL functions
- GLFW for managing windows and user input

## Building
There is an easy build script for Linux users that builds the entire project called: build-linux.sh

There is an easy build script for Windows users that builds the entire project called: build-windows.sh
