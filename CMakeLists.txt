cmake_minimum_required(VERSION 3.10)

project(cgltf VERSION 0.1)

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

add_executable( cgltf 
                src/main.c 
                src/modules/3D/camera/camera.c 
                src/modules/3D/shader/shader.c 
                src/modules/3D/model/gltf/accessor.c 
                src/modules/3D/model/gltf/animation.c
                src/modules/3D/model/gltf/asset.c
                src/modules/3D/model/gltf/buffer.c
                src/modules/3D/model/gltf/bufferView.c
                src/modules/3D/model/gltf/camera.c
                src/modules/3D/model/gltf/errors.c
                src/modules/3D/model/gltf/gltf.c
                src/modules/3D/model/gltf/header.c
                src/modules/3D/model/gltf/image.c
                src/modules/3D/model/gltf/material.c
                src/modules/3D/model/gltf/mesh.c
                src/modules/3D/model/gltf/node.c
                src/modules/3D/model/gltf/sampler.c
                src/modules/3D/model/gltf/scene.c
                src/modules/3D/model/gltf/skin.c
                src/modules/3D/model/gltf/texture.c
                src/modules/glad/glad.c)


if(WIN32)
    target_link_libraries(cgltf PRIVATE glfw m)
else()
    target_link_libraries(cgltf PRIVATE glfw m X11)
endif()    