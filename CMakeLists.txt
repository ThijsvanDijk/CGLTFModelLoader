cmake_minimum_required(VERSION 3.10)

project(CGLTFModelLoader VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_FLAGS -O3)

set(BUILD_SHARED_LIBS False)
set(GLFW_BUILD_TESTS False)
set(GLFW_BUILD_EXAMPLES False)
set(GLFW_BUILD_DOCS False)

add_compile_options(-O3)

add_subdirectory(${PROJECT_SOURCE_DIR}/deps/glfw)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/glfw/include)

link_directories(${PROJECT_SOURCE_DIR}/build/deps/glfw/src)

add_executable(CGLTFModelLoader src/main.c src/modules/3D/camera/camera.c src/modules/3D/shader/shader.c src/modules/3D/model/gltfLoader.c src/modules/glad/glad.c)

target_link_libraries(CGLTFModelLoader PRIVATE glfw m X11)