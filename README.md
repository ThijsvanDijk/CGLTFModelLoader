# CGLTFModelLoader
This is a GLTF Model Loader written in C, which is focused on reducing mesh loading times.
This is achieved by storing all GLTF Scene related data in one contiguous block of memory.

## Features
- Supports GLB format only for now
- Cross-platform with easy build scripts

## Currently working on:
- Expansion
There are a lot of properties in the GLTF Specification and currently only some are supported.
I'm working on a system that can be easily scaled so the work it takes to implement a new property is minimal.

## Dependencies
- CGLM for quick maths
- ZZZJSON fastest JSON parser I could find
- GLAD for loading OpenGL functions
- GLFW for managing windows and user input

## Building
There is an easy build script for Linux users that builds the entire project called: build-linux.sh

There is an easy build script for Windows users that builds the entire project called: build-windows.bat
