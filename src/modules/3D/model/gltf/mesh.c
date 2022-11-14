#include <3D/model/gltf/mesh.h>

u64 gltf_getMeshesSize(zj_Value* meshes, GLTFModel* model){
    u64 bufferLengthAccumulator = 0;

    u32 meshesCount = 0;
    zj_Value* currentMesh;
    while(currentMesh = zj_ArrayGet(meshes, meshesCount)){
        meshesCount++;

        // Primitives
        zj_Value* primitives = zj_ObjGet(currentMesh, "primitives");
        if(primitives){
            zj_Value* currentPrimitive;
            u32 primitivesCounter = 0;
            while(currentPrimitive = zj_ArrayGet(primitives, primitivesCounter)) primitivesCounter++;
            bufferLengthAccumulator += primitivesCounter * sizeof(GLTFMeshPrimitive);
        }

        // Weights
        zj_Value* weights = zj_ObjGet(currentMesh, "weights");
        if(weights){
            zj_Value* currentWeight;
            u32 weightsCounter = 0;
            while(currentWeight = zj_ArrayGet(weights, weightsCounter)) weightsCounter++;
            bufferLengthAccumulator += weightsCounter * sizeof(float);
        }

        // Name
        zj_Value* name = zj_ObjGet(currentMesh, "name");
        if(name) bufferLengthAccumulator += strlen(zj_GetStr(name)) + 1;
    }
    model->meshesCount = meshesCount;
    bufferLengthAccumulator += meshesCount * sizeof(GLTFMesh);

    return bufferLengthAccumulator;
}

byte* gltf_fillMeshesBuffer(zj_Value* meshes, GLTFModel* model, byte* bufferPointer){

    model->meshes = (GLTFMesh*)bufferPointer;                // Start of Buffer View area in data block
    bufferPointer += sizeof(GLTFMesh) * model->meshesCount;  // Leave space for the structs

    zj_Value* currentMesh;   
    for(u32 i = 0; i < model->meshesCount; i++){
        currentMesh = zj_ArrayGet(meshes, i);

        // Primitives
        model->meshes[i].primitives = (GLTFMeshPrimitive*)bufferPointer;
        model->meshes[i].primitivesLength = 0;
        zj_Value* primitives = zj_ObjGet(currentMesh, "primitives");
        if(primitives){
            zj_Value* currentPrimitive;

            // Get amount of primitives
            while(currentPrimitive = zj_ArrayGet(primitives, model->meshes[i].primitivesLength)){
                model->meshes[i].primitivesLength++;
            } 
            bufferPointer += model->meshes[i].primitivesLength * sizeof(GLTFMeshPrimitive);

            for(u32 primitiveIndex = 0; primitiveIndex < model->meshes[i].primitivesLength; primitiveIndex++){
                currentPrimitive = zj_ArrayGet(primitives, primitiveIndex);
                
                // Indices
                model->meshes[i].primitives[primitiveIndex].hasIndices = false;
                zj_Value * indices = zj_ObjGet(currentPrimitive, "indices");
                if(indices){
                    model->meshes[i].primitives[primitiveIndex].indices = (u32)*zj_GetInt(indices);
                    model->meshes[i].primitives[primitiveIndex].hasIndices = true;
                } 

                // Material
                model->meshes[i].primitives[primitiveIndex].hasMaterial = false;
                zj_Value * material = zj_ObjGet(currentPrimitive, "material");
                if(material){
                    model->meshes[i].primitives[primitiveIndex].material = (u32)*zj_GetInt(indices);
                    model->meshes[i].primitives[primitiveIndex].hasMaterial = true;
                } 

                // Mode
                model->meshes[i].primitives[primitiveIndex].mode = GLTF_MESH_PRIMITIVE_MODE_TRIANGLES;
                zj_Value * mode = zj_ObjGet(currentPrimitive, "mode");
                if(mode) model->meshes[i].primitives[primitiveIndex].mode = (u32)*zj_GetInt(mode);

                // Attributes
                zj_Value* attributes = zj_ObjGet(currentPrimitive, "attributes");
                if(attributes){
                    // Position
                    model->meshes[i].primitives[primitiveIndex].attributes.hasPosition = false;
                    zj_Value* position = zj_ObjGet(attributes, "POSITION");
                    if(position){
                        model->meshes[i].primitives[primitiveIndex].attributes.position = (u32)*zj_GetInt(position);
                        model->meshes[i].primitives[primitiveIndex].attributes.hasPosition = true;
                    }

                    // Normal
                    model->meshes[i].primitives[primitiveIndex].attributes.hasNormal = false;
                    zj_Value* normal = zj_ObjGet(attributes, "NORMAL");
                    if(normal){
                        model->meshes[i].primitives[primitiveIndex].attributes.normal = (u32)*zj_GetInt(normal);
                        model->meshes[i].primitives[primitiveIndex].attributes.hasNormal = true;
                    }

                    // Tangent
                    model->meshes[i].primitives[primitiveIndex].attributes.hasTangent = false;
                    zj_Value* tangent = zj_ObjGet(attributes, "TANGENT");
                    if(tangent){
                        model->meshes[i].primitives[primitiveIndex].attributes.tangent = (u32)*zj_GetInt(tangent);
                        model->meshes[i].primitives[primitiveIndex].attributes.hasTangent = true;
                    }

                    // Texcoord0
                    model->meshes[i].primitives[primitiveIndex].attributes.hasTexcoord0 = false;
                    zj_Value* texcoord0 = zj_ObjGet(attributes, "TEXCOORD_0");
                    if(texcoord0){
                        model->meshes[i].primitives[primitiveIndex].attributes.texcoord0 = (u32)*zj_GetInt(texcoord0);
                        model->meshes[i].primitives[primitiveIndex].attributes.hasTexcoord0 = true;
                    }

                    // Texcoord1
                    model->meshes[i].primitives[primitiveIndex].attributes.hasTexcoord1 = false;
                    zj_Value* texcoord1 = zj_ObjGet(attributes, "TEXCOORD_1");
                    if(texcoord1){
                        model->meshes[i].primitives[primitiveIndex].attributes.texcoord1 = (u32)*zj_GetInt(texcoord1);
                        model->meshes[i].primitives[primitiveIndex].attributes.hasTexcoord1 = true;
                    }

                    // Color
                    model->meshes[i].primitives[primitiveIndex].attributes.hasColor = false;
                    zj_Value* color = zj_ObjGet(attributes, "COLOR_0");
                    if(color){
                        model->meshes[i].primitives[primitiveIndex].attributes.color = (u32)*zj_GetInt(color);
                        model->meshes[i].primitives[primitiveIndex].attributes.hasColor = true;
                    }

                    // Joints
                    model->meshes[i].primitives[primitiveIndex].attributes.hasJoints = false;
                    zj_Value* joints = zj_ObjGet(attributes, "JOINTS_0");
                    if(joints){
                        model->meshes[i].primitives[primitiveIndex].attributes.joints = (u32)*zj_GetInt(joints);
                        model->meshes[i].primitives[primitiveIndex].attributes.hasJoints = true;
                    }

                    // Weights
                    model->meshes[i].primitives[primitiveIndex].attributes.hasWeights = false;
                    zj_Value* weights = zj_ObjGet(attributes, "WEIGHTS_0");
                    if(weights){
                        model->meshes[i].primitives[primitiveIndex].attributes.weights = (u32)*zj_GetInt(weights);
                        model->meshes[i].primitives[primitiveIndex].attributes.hasWeights = true;
                    }
                }

            }
        }

        

        // Weights
        model->meshes[i].weights = (float*)bufferPointer;
        model->meshes[i].weightsLength = 0;
        zj_Value* weights = zj_ObjGet(currentMesh, "weights");
        if(weights){
            zj_Value* currentWeight;
            while(currentWeight = zj_ArrayGet(weights, model->meshes[i].weightsLength)){
                model->meshes[i].weightsLength++;
                float value = (float)*zj_GetNum(currentWeight);
                memcpy(bufferPointer, &value, sizeof(float));
                bufferPointer += sizeof(float);
            }
        }
        
        // Name              
        model->meshes[i].nameLength = 0;
        zj_Value* name = zj_ObjGet(currentMesh, "name");
        if(name){
            const string nameString = zj_GetStr(name);
            u16 nameLength = strlen(nameString) + 1;
            strncpy(bufferPointer, nameString, nameLength);
            model->meshes[i].nameLength = nameLength;
            model->meshes[i].name = bufferPointer;
            bufferPointer += nameLength;
        }
    }

    return bufferPointer;
}