#include <3D/model/gltf/buffer.h>

u64 gltf_getBuffersSize(zj_Value* buffers_json, GLTFModel* model){
    u64 bufferLengthAccumulator = 0;

    u32 buffersCount = 0;
    zj_Value* currentBuffer;
    while(currentBuffer = zj_ArrayGet(buffers_json, buffersCount)){
        buffersCount++;

        zj_Value* uri = zj_ObjGet(currentBuffer, "uri");
        if(uri != 0) bufferLengthAccumulator += strlen(zj_GetStr(uri)) + 1;

        zj_Value* name = zj_ObjGet(currentBuffer, "name");
        if(name != 0) bufferLengthAccumulator += strlen(zj_GetStr(name)) + 1;

        bufferLengthAccumulator += sizeof(GLTFBuffer);
    }
    model->buffersCount = buffersCount;

    return bufferLengthAccumulator;
}

byte* gltf_fillBuffersBuffer(zj_Value* buffers_json, GLTFModel* model, byte* bufferPointer){
    model->buffers = bufferPointer;                             // Start of Buffer area in data block
    bufferPointer += sizeof(GLTFBuffer) * model->buffersCount;  // Leave space for the structs

    zj_Value* currentBuffer;

    for(u32 i = 0; i < model->buffersCount; i++){               // Go through all the buffers

        currentBuffer = zj_ArrayGet(buffers_json, i);

        model->buffers[i].byteLength = *zj_GetInt(zj_ObjGet(currentBuffer, "byteLength"));

        model->buffers[i].uriLength = 0;
        zj_Value* uri = zj_ObjGet(currentBuffer, "uri");
        if(uri != 0){
            const string uriString = zj_GetStr(uri);
            u16 uriLength = strlen(uriString) + 1;
            strncpy(bufferPointer, uriString, uriLength);
            model->buffers[i].uriLength = uriLength;
            model->buffers[i].uri = bufferPointer;
            bufferPointer += uriLength;
        }

        model->buffers[i].nameLength = 0;
        zj_Value* name = zj_ObjGet(currentBuffer, "name");
        if(name != 0){
            const string nameString = zj_GetStr(name);
            u16 nameLength = strlen(nameString) + 1;
            strncpy(bufferPointer, nameString, nameLength);
            model->buffers[i].nameLength = nameLength;
            model->buffers[i].name = bufferPointer;
            bufferPointer += nameLength;
        }
    }

    return bufferPointer;
}