cmake_minimum_required(VERSION 3.10)

project(CGLTFModelLoader VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_FLAGS -O3)
add_compile_options(-Wall -Wextra)

include_directories(include)
link_directories(lib)

add_executable(CGLTFModelLoader src/main.c src/modules/3D/camera/camera.c src/modules/3D/shader/shader.c src/modules/3D/model/gltfLoader.c src/modules/glad/glad.c)

target_link_libraries(CGLTFModelLoader PRIVATE glfw3 cglm m X11)
