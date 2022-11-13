#ifndef GTLF_ACCESSOR_H
#define GLTF_ACCESSOR_H

#include <3D/model/gltf/types.h>

u64 gltf_getAccessorsSize(zj_Value* accessors_json, GLTFModel* model);

i8 gltf_fillAccessorsBuffer(zj_Value* accessors_json, GLTFModel* model, void* bufferPointer);



#endif