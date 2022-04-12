cmake_minimum_required(VERSION 3.10)

project(CGLTFModelLoader VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_FLAGS -O2)
add_compile_options(-Wall -Wextra)

include_directories(include)
link_directories(lib)

add_executable(CGLTFModelLoader src/main.c src/modules/camera.c src/modules/shader.c src/modules/mesh.c src/modules/model.c src/deps/stb.c src/deps/gl.c)

IF(WIN32)
target_link_libraries(CGLTFModelLoader PRIVATE glfw3 cglm assimp m)
ELSE()
target_link_libraries(CGLTFModelLoader PRIVATE glfw cglm assimp m)
ENDIF()