#ifndef GLTF_IMAGE_H
#define GLTF_IMAGE_H

#include <3D/model/gltf/types.h>

#define GLTF_IMAGE_START_PATTERN_PNG (uint8_t[]){0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a}
#define GLTF_IMAGE_START_PATTERN_JPEG (uint8_t[]){0xff, 0xd8, 0xff}

u64 gltf_getImagesSize(zj_Value* accessors_json);

i8 gltf_fillImagesBuffer(zj_Value* accessors_json, void* bufferPointer);

#endif