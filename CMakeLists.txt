cmake_minimum_required(VERSION 3.10)

project(CGLTFModelLoader VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(include)
link_directories(lib)

add_executable(CGLTFModelLoader src/main.c src/modules/vec3.c)
target_link_libraries(CGLTFModelLoader PRIVATE glfw)