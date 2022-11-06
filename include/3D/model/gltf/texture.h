#ifndef GLTF_TEXTURE_H
#define GLTF_TEXTURE_H

#include <3D/model/gltf/types.h>

u64 gltf_getTexturesSize(zj_Value* accessors_json);

i8 gltf_fillTexturesBuffer(zj_Value* accessors_json, void* bufferPointer);

#endif