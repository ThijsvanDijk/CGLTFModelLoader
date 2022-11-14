#ifndef GLTF_CAMERA_H
#define GLTF_CAMERA_H

#include <3D/model/gltf/types.h>

u64 gltf_getCamerasSize(zj_Value* cameras, GLTFModel* model);

byte* gltf_fillCamerasBuffer(zj_Value* cameras, GLTFModel* model, byte* bufferPointer);

#endif