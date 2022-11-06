#ifndef GLTF_SKIN_H
#define GLTF_SKIN_H

#include <3D/model/gltf/types.h>

u64 gltf_getSkinsSize(zj_Value* accessors_json);

i8 gltf_fillSkinsBuffer(zj_Value* accessors_json, void* bufferPointer);

#endif
