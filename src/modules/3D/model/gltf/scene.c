#include <3D/model/gltf/scene.h>

u64 gltf_getScenesSize(zj_Value* scenes, GLTFModel* model){
    u64 bufferLengthAccumulator = 0;

    // Go through array
    zj_Value* currentScene;
    u32 scenesCount = 0;
    while(currentScene = zj_ArrayGet(scenes, scenesCount)){
        scenesCount++;
        // Count the nodes
        zj_Value* nodes = zj_ObjGet(currentScene, "nodes");
        if(nodes){
            uint32_t node_counter = 0;
            zj_Value* currentNode;
            while(currentNode = zj_ArrayGet(nodes, node_counter)){
                node_counter++;
                bufferLengthAccumulator += sizeof(uint32_t);
            }
        }

        // Check for scene name
        zj_Value* scene_name = zj_ObjGet(currentScene, "name");
        if(scene_name) bufferLengthAccumulator += strlen(zj_GetStr(scene_name)) + 1;

        // Add size of scene struct to buffer length
        bufferLengthAccumulator += sizeof(GLTFScene);
    }
    model->scenesCount = scenesCount;
    return bufferLengthAccumulator;
}

byte* gltf_fillScenesBuffer(zj_Value* scenes, GLTFModel* model, byte* bufferPointer){
    model->scenes = (GLTFScene *)bufferPointer;
    bufferPointer += model->scenesCount * sizeof(GLTFScene);

    zj_Value* currentScene;
    for(u32 i = 0; i < model->scenesCount; i++){
        currentScene = zj_ArrayGet(scenes, i);
        // Count the nodes
        model->scenes[i].nodesLength = 0;
        zj_Value* nodes = zj_ObjGet(currentScene, "nodes");
        if(nodes){
            byte* tempBufferPointer = bufferPointer;
            zj_Value* currentNode;
            while(currentNode = zj_ArrayGet(nodes, model->scenes[i].nodesLength)){
                model->scenes[i].nodesLength++;
                memcpy(bufferPointer, zj_GetInt(currentNode), sizeof(u32));
                bufferPointer += sizeof(u32);
            }
            model->scenes[i].nodes = (u32*)tempBufferPointer;
        }

        // Check for scene name
        model->scenes[i].nameLength = 0;
        zj_Value* scene_name = zj_ObjGet(currentScene, "name");
        if(scene_name){
            const char* nameString = zj_GetStr(scene_name);
            u32 nameLength = strlen(nameString) + 1;
            strncpy(bufferPointer, nameString, nameLength);
            model->scenes[i].nameLength = nameLength;
            model->scenes[i].name = bufferPointer;
            bufferPointer += nameLength;
        }
    }
    return bufferPointer;
}