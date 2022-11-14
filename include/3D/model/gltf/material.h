#ifndef GLTF_MATERIAL_H
#define GLTF_MATERIAL_H

#include <3D/model/gltf/types.h>

u64 gltf_getMaterialsSize(zj_Value* materials, GLTFModel* model);

byte* gltf_fillMaterialsBuffer(zj_Value* materials, GLTFModel* model, byte* bufferPointer);

#endif