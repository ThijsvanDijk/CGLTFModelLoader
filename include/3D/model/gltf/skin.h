#ifndef GLTF_SKIN_H
#define GLTF_SKIN_H

#include <3D/model/gltf/types.h>

u64 gltf_getSkinsSize(zj_Value* skins, GLTFModel* model);

byte* gltf_fillSkinsBuffer(zj_Value* skins, GLTFModel* model, byte* bufferPointer);

#endif
