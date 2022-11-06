#ifndef GLTF_BUFFERVIEW_H
#define GLTF_BUFFERVIEW_H

#include <3D/model/gltf/types.h>

u64 gltf_getBufferViewsSize(zj_Value* accessors_json);

i8 gltf_fillBufferViewsBuffer(zj_Value* accessors_json, void* bufferPointer);

#endif