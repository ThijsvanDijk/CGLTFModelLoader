#ifndef GLTF_SCENE_H
#define GLTF_SCENE_H

#include <3D/model/gltf/types.h>

u64 gltf_getScenesSize(zj_Value* scenes, GLTFModel* model);

byte* gltf_fillScenesBuffer(zj_Value* scenes, GLTFModel* model, byte* bufferPointer);

#endif