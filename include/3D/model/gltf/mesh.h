#ifndef GLTF_MESH_H
#define GLTF_MESH_H

#include <3D/model/gltf/types.h>

u64 gltf_getMeshesSize(zj_Value* accessors_json);

i8 gltf_fillMeshesBuffer(zj_Value* accessors_json, void* bufferPointer);

#endif