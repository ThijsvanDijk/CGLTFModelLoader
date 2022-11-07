#ifndef GLTF_BUFFERVIEW_H
#define GLTF_BUFFERVIEW_H

#include <3D/model/gltf/types.h>

u64 gltf_getBufferViewsSize(zj_Value* bufferViews_json, GLTFModel* model);

byte* gltf_fillBufferViewsBuffer(zj_Value* bufferViews_json, GLTFModel* model, byte* bufferPointer);

#endif