#ifndef GLTF_SCENE_H
#define GLTF_SCENE_H

#include <3D/model/gltf/types.h>

u64 gltf_getScenesSize(zj_Value* accessors_json);

i8 gltf_fillScenesBuffer(zj_Value* accessors_json, void* bufferPointer);

#endif