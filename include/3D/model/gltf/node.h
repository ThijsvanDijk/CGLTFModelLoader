#ifndef GLTF_NODE_H
#define GLTF_NODE_H

#include <3D/model/gltf/types.h>

u64 gltf_getNodesSize(zj_Value* accessors_json);

i8 gltf_fillNodesBuffer(zj_Value* accessors_json, void* bufferPointer);

#endif