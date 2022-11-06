#ifndef GLTF_SAMPLER_H
#define GLTF_SAMPLER_H

#include <3D/model/gltf/types.h>

u64 gltf_getSamplersSize(zj_Value* accessors_json);

i8 gltf_fillSamplersBuffer(zj_Value* accessors_json, void* bufferPointer);

#endif