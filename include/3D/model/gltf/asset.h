#ifndef GLTF_ASSET_H
#define GLTF_ASSET_H

#include <3D/model/gltf/types.h>

u64 gltf_getAssetSize(zj_Value* asset);

byte* gltf_fillAssetBuffer(zj_Value* accessors_json, GLTFModel* model, byte* bufferPointer);

#endif