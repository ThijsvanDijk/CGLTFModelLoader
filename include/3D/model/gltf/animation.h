#ifndef GLTF_ANIMATION_H
#define GLTF_ANIMATION_H

#include <3D/model/gltf/types.h>

u64 gltf_getAnimationsSize(zj_Value* accessors_json);

i8 gltf_fillAnimationsBuffer(zj_Value* accessors_json, void* bufferPointer);

#endif