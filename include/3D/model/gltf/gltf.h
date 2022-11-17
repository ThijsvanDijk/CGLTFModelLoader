#ifndef GLTF_H
#define GLTF_H

// Libraries
#ifndef WIN32
#include <fcntl.h>
#include <unistd.h>
#endif

// Types
#include <3D/model/gltf/header.h>
#include <3D/model/gltf/types.h>

// Functions
#include <3D/model/gltf/accessor.h>
#include <3D/model/gltf/animation.h>
#include <3D/model/gltf/asset.h>
#include <3D/model/gltf/buffer.h>
#include <3D/model/gltf/bufferView.h>
#include <3D/model/gltf/camera.h>
#include <3D/model/gltf/errors.h>
#include <3D/model/gltf/image.h>
#include <3D/model/gltf/material.h>
#include <3D/model/gltf/mesh.h>
#include <3D/model/gltf/node.h>
#include <3D/model/gltf/sampler.h>
#include <3D/model/gltf/scene.h>
#include <3D/model/gltf/skin.h>
#include <3D/model/gltf/texture.h>

// Definitions
#define GLTF_MAX_SUPPORTED_VERSION_MAJOR 2
#define GLTF_MAX_SUPPORTED_VERSION_MINOR 0
#define GLTF_MIN_SUPPORTED_VERSION_MAJOR 2
#define GLTF_MIN_SUPPORTED_VERSION_MINOR 0

i8 gltf_modelLoad(const char* filename, GLTFModel* model);

/**
 * @brief Print comprehensive info about all properties of the model to terminal
 * 
 */
void print_model(GLTFModel* model);

#endif