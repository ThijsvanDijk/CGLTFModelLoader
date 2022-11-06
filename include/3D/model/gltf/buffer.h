#ifndef GLTF_BUFFER_H
#define GLTF_BUFFER_H

#include <3D/model/gltf/types.h>

u64 gltf_getBuffersSize(zj_Value* accessors_json);

i8 gltf_fillBuffersBuffer(zj_Value* accessors_json, void* bufferPointer);

#endif