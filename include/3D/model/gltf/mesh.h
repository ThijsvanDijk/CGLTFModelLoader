#ifndef GLTF_MESH_H
#define GLTF_MESH_H

#include <3D/model/gltf/types.h>

u64 gltf_getMeshesSize(zj_Value* meshes, GLTFModel* model);

byte* gltf_fillMeshesBuffer(zj_Value* meshes, GLTFModel* model, byte* bufferPointer);

#endif