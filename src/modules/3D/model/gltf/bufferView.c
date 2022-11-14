#include <3D/model/gltf/bufferView.h>

u64 gltf_getBufferViewsSize(zj_Value* bufferViews, GLTFModel* model){
    u64 bufferLengthAccumulator = 0;

    u32 bufferViewsCount = 0;
    zj_Value* currentBufferView;
    while(currentBufferView = zj_ArrayGet(bufferViews, bufferViewsCount)){
        bufferViewsCount++;

        zj_Value* name = zj_ObjGet(currentBufferView, "name");
        if(name != 0) bufferLengthAccumulator += strlen(zj_GetStr(name)) + 1;
    }
    model->bufferViewsCount = bufferViewsCount;
    bufferLengthAccumulator += sizeof(GLTFBufferView) * bufferViewsCount;

    return bufferLengthAccumulator;
}

byte* gltf_fillBufferViewsBuffer(zj_Value* bufferViews, GLTFModel* model, byte* bufferPointer){
    model->bufferViews = (GLTFBufferView*)bufferPointer;                // Start of Buffer View area in data block
    bufferPointer += sizeof(GLTFBufferView) * model->bufferViewsCount;  // Leave space for the structs

    zj_Value* currentBufferView;
    for(u32 i = 0; i < model->bufferViewsCount; i++){
        currentBufferView = zj_ArrayGet(bufferViews, i);

        model->bufferViews[i].buffer = *zj_GetInt(zj_ObjGet(currentBufferView, "buffer"));
        model->bufferViews[i].byteLength = *zj_GetInt(zj_ObjGet(currentBufferView, "byteLength"));

        model->bufferViews[i].byteOffset = 0;
        zj_Value* byteOffset = zj_ObjGet(currentBufferView, "byteOffset");
        if(byteOffset != 0) model->bufferViews[i].byteOffset = *zj_GetInt(byteOffset);

        model->bufferViews[i].byteStride = 0;
        zj_Value* byteStride = zj_ObjGet(currentBufferView, "byteStride");
        if(byteStride != 0) model->bufferViews[i].byteStride = *zj_GetInt(byteStride);

        model->bufferViews[i].hasTarget = false;
        zj_Value* target = zj_ObjGet(currentBufferView, "target");
        if(target){
            model->bufferViews[i].target = *zj_GetInt(target);
            model->bufferViews[i].hasTarget = true;
        }  

        model->bufferViews[i].nameLength = 0;
        zj_Value* name = zj_ObjGet(currentBufferView, "name");
        if(name != 0){
            const string nameString = zj_GetStr(name);
            u16 nameLength = strlen(nameString) + 1;
            strncpy(bufferPointer, nameString, nameLength);
            model->bufferViews[i].nameLength = nameLength;
            model->bufferViews[i].name = bufferPointer;
            bufferPointer += nameLength;
        }
    }

    return bufferPointer;
}