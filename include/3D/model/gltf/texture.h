#ifndef GLTF_TEXTURE_H
#define GLTF_TEXTURE_H

#include <3D/model/gltf/types.h>

u64 gltf_getTexturesSize(zj_Value* textures, GLTFModel* model);

byte* gltf_fillTexturesBuffer(zj_Value* textures, GLTFModel* model, byte* bufferPointer);

#endif