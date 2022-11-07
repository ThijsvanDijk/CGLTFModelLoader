#ifndef GLTF_BUFFER_H
#define GLTF_BUFFER_H

#include <3D/model/gltf/types.h>

u64 gltf_getBuffersSize(zj_Value* accessors_json, GLTFModel* model);

byte* gltf_fillBuffersBuffer(zj_Value* accessors_json, GLTFModel* model, byte* bufferPointer);

#endif