#ifndef GLTF_MATERIAL_H
#define GLTF_MATERIAL_H

#include <3D/model/gltf/types.h>

u64 gltf_getMaterialsSize(zj_Value* accessors_json);

i8 gltf_fillMaterialsBuffer(zj_Value* accessors_json, void* bufferPointer);

#endif