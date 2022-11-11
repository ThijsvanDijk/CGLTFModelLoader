#include <3D/model/gltf/node.h>

u64 gltf_getNodesSize(zj_Value* nodes, GLTFModel* model){
    u64 bufferLengthAccumulator = 0;

    model->nodesCount = 0;
    zj_Value* currentNode;
    while(currentNode = zj_ArrayGet(nodes, model->nodesCount)){
        model->nodesCount++;

        // Count the children
        zj_Value* children = zj_ObjGet(currentNode, "children");
        if(children){
            u32 child_counter = 0;
            zj_Value* currentChild;
            while(currentChild = zj_ArrayGet(children, child_counter)){
                child_counter++;
                bufferLengthAccumulator += sizeof(u32);
            }
        }

        zj_Value* weights = zj_ObjGet(currentNode, "weights");
        if(weights){
            u32 weight_counter = 0;
            zj_Value* currentWeight;
            while(currentWeight = zj_ArrayGet(weights, weight_counter)){
                weight_counter++;
                bufferLengthAccumulator += sizeof(float);
            }
        }

        // Get length of name string
        zj_Value* node_name = zj_ObjGet(currentNode, "name");
        if(node_name) bufferLengthAccumulator += strlen(zj_GetStr(node_name)) + 1;        
    }

    bufferLengthAccumulator += model->nodesCount * sizeof(GLTFNode);
    return bufferLengthAccumulator;
}

byte* gltf_fillNodesBuffer(zj_Value* nodes, GLTFModel* model, byte* bufferPointer){
    model->nodes = (GLTFNode*)bufferPointer;
    bufferPointer += model->nodesCount * sizeof(GLTFNode);

    zj_Value* currentNode;
    for(uint32_t i = 0; i < model->nodesCount; i++){
        currentNode = zj_ArrayGet(nodes, i);
        
        // Children
        model->nodes[i].childrenLength = 0;
        zj_Value * children = zj_ObjGet(currentNode, "children");
        if(children){
            byte* tempBufferPointer = bufferPointer;
            zj_Value* currentChild;
            while(currentChild = zj_ArrayGet(children, model->nodes[i].childrenLength)){
                model->nodes[i].childrenLength++;
                memcpy(bufferPointer, zj_GetInt(currentChild), sizeof(u32));
                bufferPointer += sizeof(u32);
            }
            model->nodes[i].children = (u32*)tempBufferPointer;
        }

        // Weights
        model->nodes[i].weightsLength = 0;
        zj_Value* weights = zj_ObjGet(currentNode, "weights");
        if(weights){
            byte* tempBufferPointer = bufferPointer;
            zj_Value* currentWeight;
            while(currentWeight = zj_ArrayGet(weights, model->nodes[i].weightsLength)){
                model->nodes[i].weightsLength++;
                float value = (float)*zj_GetDouble(currentWeight);
                memcpy(bufferPointer, &value, sizeof(float));
                bufferPointer += sizeof(float); 
            }
            model->nodes[i].weights = (float *)tempBufferPointer;
        }

        // Name
        model->nodes[i].nameLength = 0;
        zj_Value* node_name = zj_ObjGet(currentNode, "name");
        if(node_name){
            const char * name_str = zj_GetStr(node_name);
            uint32_t name_str_length = strlen(zj_GetStr(node_name)) + 1;
            strncpy(bufferPointer, name_str, name_str_length);
            model->nodes[i].nameLength = name_str_length;
            model->nodes[i].name = bufferPointer;
            bufferPointer += name_str_length;
        }

        // Mesh
        model->nodes[i].hasMesh = false;
        zj_Value* node_mesh = zj_ObjGet(currentNode, "mesh");
        if(node_mesh){
            model->nodes[i].mesh = *zj_GetInt(node_mesh);
            model->nodes[i].hasMesh = true;
        }

        // Camera
        model->nodes[i].hasCamera = false;
        zj_Value* node_camera = zj_ObjGet(currentNode, "camera");
        if(node_camera){
            model->nodes[i].camera = *zj_GetInt(node_camera);
            model->nodes[i].hasCamera = true;
        }

        // Skin
        model->nodes[i].hasSkin = false;
        zj_Value* node_skin = zj_ObjGet(currentNode, "skin");
        if(node_skin){
            model->nodes[i].skin = *zj_GetInt(node_skin);
            model->nodes[i].hasSkin = true;
        }

        // Matrix
        model->nodes[i].hasMatrix = false;
        mat4 identity = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
        zj_Value* node_matrix = zj_ObjGet(currentNode, "matrix");
        if(node_matrix){
            for(uint8_t j = 0; j < sizeof(mat4)/sizeof(vec4); j++){
                for(u8 k = 0; k < sizeof(vec4)/sizeof(float); k++){
                    double doubleValue = *zj_GetDouble(zj_ArrayGet(node_matrix, j * 4 + k));
                    float floatValue = (float)doubleValue;
                    memcpy(&model->nodes[i].matrix[j][k], &floatValue, sizeof(float));
                }
            }
            model->nodes[i].hasMatrix = true;
        }else{
            memcpy(&model->nodes[i].matrix, &identity, sizeof(mat4)); // Set default
        }

        // TRS
        model->nodes[i].hasTranslation = false;
        model->nodes[i].hasRotation = false;
        model->nodes[i].hasScale = false;

        vec3 translation_default = {0, 0, 0};
        vec4 rotation_default = {0, 0, 0, 1};
        vec3 scale_default = {1, 1, 1};

        memcpy(model->nodes[i].translation, translation_default, sizeof(vec3));
        memcpy(model->nodes[i].rotation, rotation_default, sizeof(vec4));
        memcpy(model->nodes[i].scale, scale_default, sizeof(vec3));

        zj_Value* node_translation = zj_ObjGet(currentNode, "translation");
        zj_Value* node_rotation = zj_ObjGet(currentNode, "rotation");
        zj_Value* node_scale = zj_ObjGet(currentNode, "scale");
        
        if(node_translation){
            for(uint8_t j = 0; j < sizeof(vec3)/sizeof(float); j++){
                memcpy(&model->nodes[i].translation[j], (float*)zj_GetInt(zj_ArrayGet(node_translation, j)), sizeof(float));
            }
            model->nodes[i].hasTranslation = true;
        }
        if(node_rotation){
            for(uint8_t j = 0; j < sizeof(vec4)/sizeof(float); j++){
                memcpy(&model->nodes[i].rotation[j], (float*)zj_GetInt(zj_ArrayGet(node_rotation, j)), sizeof(float));
            }
            model->nodes[i].hasRotation = true;
        }
        if(node_scale){
            for(uint8_t j = 0; j < sizeof(vec3)/sizeof(float); j++){
                memcpy(&model->nodes[i].scale[j], (float*)zj_GetInt(zj_ArrayGet(node_scale, j)), sizeof(float));
            }
            model->nodes[i].hasScale = true;
        }
    }

    return bufferPointer;
}