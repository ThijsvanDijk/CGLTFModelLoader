#include <3D/model/gltf/gltf.h>

// #define WIN32 // Disables Linux specific optimised code

/*
    Main function for loading all different properties
    of a glTF file. It does two passes over the file.
    First it determines the size of the model with all
    of it's properties and allocates that amount of space.
    After that it fills the properties in their respective
    assigned spaces in this allocated buffer.
*/

static u64 gltf_getModelBufferSize(zj_Value* parsed_json, GLTFModel* model){
    u64 bufferSizeAccumulator = 0;
    zj_Value* accessors = zj_ObjGet(parsed_json, "accessors");
    // zj_Value* animations = zj_ObjGet(parsed_json, "animations");
    zj_Value* asset = zj_ObjGet(parsed_json, "asset");
    zj_Value* buffers = zj_ObjGet(parsed_json, "buffers");
    zj_Value* bufferViews = zj_ObjGet(parsed_json, "bufferViews");
    // zj_Value* cameras = zj_ObjGet(parsed_json, "cameras");
    // zj_Value* images = zj_ObjGet(parsed_json, "images");
    // zj_Value* materials = zj_ObjGet(parsed_json, "materials");
    zj_Value* meshes = zj_ObjGet(parsed_json, "meshes");
    zj_Value* nodes = zj_ObjGet(parsed_json, "nodes");
    // zj_Value* samplers = zj_ObjGet(parsed_json, "samplers");
    zj_Value* scenes = zj_ObjGet(parsed_json, "scenes");
    // zj_Value* skins = zj_ObjGet(parsed_json, "skins");
    // zj_Value* textures = zj_ObjGet(parsed_json, "textures");


    if(accessors != NULL) bufferSizeAccumulator += gltf_getAccessorsSize(accessors, model);
    // if(animations != NULL) bufferSizeAccumulator += gltf_getAnimationsSize(animations);
    bufferSizeAccumulator += gltf_getAssetSize(asset);
    if(buffers != NULL) bufferSizeAccumulator += gltf_getBuffersSize(buffers, model);
    if(bufferViews != NULL) bufferSizeAccumulator += gltf_getBufferViewsSize(bufferViews, model);
    // if(cameras != NULL) bufferSizeAccumulator += gltf_getCamerasSize(cameras);
    // if(images != NULL) bufferSizeAccumulator += gltf_getImagesSize(images);
    // if(materials != NULL) bufferSizeAccumulator += gltf_getMaterialsSize(materials);
    if(meshes != NULL) bufferSizeAccumulator += gltf_getMeshesSize(meshes, model);
    if(nodes != NULL) bufferSizeAccumulator += gltf_getNodesSize(nodes, model);
    // if(samplers != NULL) bufferSizeAccumulator += gltf_getSamplersSize(samplers);
    if(scenes != NULL) bufferSizeAccumulator += gltf_getScenesSize(scenes, model);
    // if(skins != NULL) bufferSizeAccumulator += gltf_getSkinsSize(skins);
    // if(textures != NULL) bufferSizeAccumulator += gltf_getTexturesSize(textures);
    
    return bufferSizeAccumulator;
}

static byte* gltf_fillModelData(zj_Value* parsed_json, GLTFModel* model){
    byte* bufferPointer = model->data;
    zj_Value* rootScene = zj_ObjGet(parsed_json, "scene");
    zj_Value* accessors = zj_ObjGet(parsed_json, "accessors");
    // zj_Value* animations = zj_ObjGet(parsed_json, "animations");
    zj_Value* asset = zj_ObjGet(parsed_json, "asset");
    zj_Value* buffers = zj_ObjGet(parsed_json, "buffers");
    zj_Value* bufferViews = zj_ObjGet(parsed_json, "bufferViews");
    // zj_Value* cameras = zj_ObjGet(parsed_json, "cameras");
    // zj_Value* images = zj_ObjGet(parsed_json, "images");
    // zj_Value* materials = zj_ObjGet(parsed_json, "materials");
    zj_Value* meshes = zj_ObjGet(parsed_json, "meshes");
    zj_Value* nodes = zj_ObjGet(parsed_json, "nodes");
    // zj_Value* samplers = zj_ObjGet(parsed_json, "samplers");
    zj_Value* scenes = zj_ObjGet(parsed_json, "scenes");
    // zj_Value* skins = zj_ObjGet(parsed_json, "skins");
    // zj_Value* textures = zj_ObjGet(parsed_json, "textures");
    model->hasScene = false;
    if(rootScene){
        model->scene = (u32)*zj_GetInt(rootScene);
        model->hasScene = true;
    }  
    bufferPointer = gltf_fillAssetBuffer(asset, model, bufferPointer); 
    if(scenes) bufferPointer = gltf_fillScenesBuffer(scenes, model, bufferPointer);
    if(nodes) bufferPointer = gltf_fillNodesBuffer(nodes, model, bufferPointer);
    if(meshes) bufferPointer = gltf_fillMeshesBuffer(meshes, model, bufferPointer);
    if(accessors) bufferPointer = gltf_fillAccessorsBuffer(accessors, model, bufferPointer); 
    if(bufferViews) bufferPointer = gltf_fillBufferViewsBuffer(bufferViews, model, bufferPointer);
    if(buffers) bufferPointer = gltf_fillBuffersBuffer(buffers, model, bufferPointer);
    
    return bufferPointer;
}

i8 gltf_modelLoad(const char* filename, GLTFModel* model){
    #ifndef WIN32

    // Opening file
    int file = open(filename, O_RDONLY);
    if(file == -1) return gltf_error(0);

    // Get file length
    off_t fileLength = lseek(file, 0, SEEK_END);
    if(fileLength < (sizeof(u32) * 5)) gltf_error(1);

    // Read entire file into a buffer
    byte* fileBuffer = malloc(fileLength);

    lseek(file, 0, SEEK_SET);
    read(file, fileBuffer, fileLength);
    close(file);

    #else

    // Opening file
    FILE* file = fopen(filename, "rb");
    if (file == NULL) return gltf_error(0);

    // Get file length
    fseek(file, 0, SEEK_END);
    u64 fileLength = ftell(file);
    if(fileLength < (sizeof(u32) * 5)) gltf_error(1);

    // Read entire file into a buffer
    byte* fileBuffer = malloc(fileLength);
    fseek(file, 0, SEEK_SET);
    fread(fileBuffer, sizeof(u8), fileLength, file);
    fclose(file);

    #endif

    u32* fileBuffer_u32ptr = (u32*)fileBuffer;
    // Check if glTF
    if(fileBuffer_u32ptr[0] != GLB_MAGIC) return gltf_error(1);

    zj_Allocator* allocator = zj_NewAllocator();
    zj_Value* parsed_json = zj_NewValue(allocator);
    if(zj_ParseLen(parsed_json, (const char *)&fileBuffer_u32ptr[5], fileBuffer_u32ptr[3]) != true) return gltf_error(2);

    // Get Buffer Size
    u64 bufferSize = gltf_getModelBufferSize(parsed_json, model);
    model->data = malloc(bufferSize);
    model->dataLength = bufferSize;

    // Fill buffer
    byte* bufferPointer = gltf_fillModelData(parsed_json, model);

    // Finally add binary section to data buffer
    byte* bufferSection = fileBuffer + sizeof(GLTFHeader) + 2 * sizeof(GLTFChunkHeader) + fileBuffer_u32ptr[3]; // Start of bin section
    u32* binLength = (u32*)(bufferSection - 2 * sizeof(u32));
    memcpy(bufferPointer, bufferSection, *binLength);
    model->binDataStart = bufferPointer;

    free(fileBuffer);
    zj_ReleaseAllocator(allocator);
}

void print_model(GLTFModel* model){
    printf("========================================\nGLTF Model Info\nTotal Size in memory: %lu bytes\n", sizeof(GLTFModel) + model->dataLength);

    // Asset
    printf("----------------------------------------\n");
                                            printf("Asset:\n\tVersion:     %hhu.%hhu\n", model->asset.versionMajor, model->asset.versionMinor);
    if(model->asset.hasMinVersion)          printf("\tMin Version: %hhu.%hhu\n", model->asset.minVersionMajor, model->asset.minVersionMinor);
    if(model->asset.copyrightLength > 0)    printf("\tCopyright:   %s\n", model->asset.copyright);
    if(model->asset.generatorLength > 0)    printf("\tGenerator:   %s\n", model->asset.generator);

    // Scenes
    printf("----------------------------------------\n");
    printf("Root Scene: %u\n", model->scene);
    printf("----------------------------------------\n");

    for(u32 i = 0; i < model->scenesCount; i++){
        printf("Scene %u:\n", i);
        if(model->scenes[i].nodesLength > 0){
            printf("\tNodes: [");
            for(u32 j = 0; j < model->scenes[i].nodesLength; j++){
                printf("%u", model->scenes[i].nodes[j]);
                if(j != model->scenes[i].nodesLength - 1) printf(", ");
            }
            printf("]\n");
        }
        if(model->scenes[i].nameLength > 0) printf("\tName:  %s\n", model->scenes[i].name);        
    }

    // Nodes
    printf("----------------------------------------\n");
    for(u32 i = 0; i < model->nodesCount; i++){
        printf("Node %u:\n", i);
        
        // Children
        printf("\tChildren(%u): [", model->nodes[i].childrenLength);
        for(u32 j = 0; j < model->nodes[i].childrenLength; j++){
            printf("%u", model->nodes[i].children[j]);
            if(j != model->nodes[i].childrenLength - 1) printf(", ");
        }
        printf("]\n");

        // Name
        if(model->nodes[i].nameLength > 0) 
            printf("\tName:   %s\n", model->nodes[i].name);
        
        // Mesh
        if(model->nodes[i].hasMesh) 
            printf("\tMesh:   %u\n", model->nodes[i].mesh);

        // Camera
        if(model->nodes[i].hasCamera) 
            printf("\tCamera: %u\n", model->nodes[i].camera);
        
        // Translation
        printf("\tTranslation: [%.2f, %.2f, %.2f]\n", model->nodes[i].translation[0], model->nodes[i].translation[1], model->nodes[i].translation[2]);
        
        // Rotation
        printf("\tRotation: [%.2f, %.2f, %.2f, %.2f]\n", model->nodes[i].rotation[0], model->nodes[i].rotation[1], model->nodes[i].rotation[2], model->nodes[i].rotation[3]);
        
        // Scale
        printf("\tScale: [%.2f, %.2f, %.2f]\n", model->nodes[i].scale[0], model->nodes[i].scale[1], model->nodes[i].scale[2]);

        // Matrix
        vec4* matrix = model->nodes[i].matrix;
        printf("\tMatrix:   | %0.2f %0.2f %0.2f %0.2f |\n", matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0]);
        printf("\t          | %0.2f %0.2f %0.2f %0.2f |\n", matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1]);
        printf("\t          | %0.2f %0.2f %0.2f %0.2f |\n", matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2]);
        printf("\t          | %0.2f %0.2f %0.2f %0.2f |\n", matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]);

        // Weights
        printf("\tWeights(%u): [", model->nodes[i].weightsLength);
        for(u32 j = 0; j < model->nodes[i].weightsLength; j++){
            printf("%.2f", model->nodes[i].weights[j]);
            if(j != model->nodes[i].weightsLength - 1) printf(", ");
        }
        printf("]\n");
    }

    // Meshes
    printf("----------------------------------------\n");
    for(u32 i = 0; i < model->meshesCount; i++){
        printf("Mesh %u:\n", i);
        if(model->meshes[i].nameLength > 0) printf("\tName: %s\n", model->meshes[i].name);

        // Weights
        printf("\tWeights: [");
        for(u32 j = 0; j < model->meshes[i].weightsLength; j++){
            printf("%.2f", model->meshes[i].weights[j]);
            if(j != model->meshes[i].weightsLength - 1) printf(", ");
        }
        printf("]\n");

        // Primitives
        printf("\tPrimitives:\n");
        for(u32 j = 0; j < model->meshes[i].primitivesLength; j++){
            if(model->meshes[i].primitives[j].hasIndices) printf("\t\tIndices: %u\n", model->meshes[i].primitives[j].indices);
            if(model->meshes[i].primitives[j].hasMaterial) printf("\t\tMaterial: %u\n", model->meshes[i].primitives[j].material);
            printf("\t\tMode: %u\n", model->meshes[i].primitives[j].mode);
            printf("\t\tAttributes:\n");
            if(model->meshes[i].primitives[j].attributes.hasPosition) printf("\t\t\tPOSITION: %u\n", model->meshes[i].primitives[j].attributes.position);
            if(model->meshes[i].primitives[j].attributes.hasNormal) printf("\t\t\tNORMAL: %u\n", model->meshes[i].primitives[j].attributes.normal);
            if(model->meshes[i].primitives[j].attributes.hasTangent) printf("\t\t\tTANGENT: %u\n", model->meshes[i].primitives[j].attributes.tangent);
            if(model->meshes[i].primitives[j].attributes.hasTexcoord0) printf("\t\t\tTEXCOORD_0: %u\n", model->meshes[i].primitives[j].attributes.texcoord0);
            if(model->meshes[i].primitives[j].attributes.hasTexcoord1) printf("\t\t\tTEXCOORD_1: %u\n", model->meshes[i].primitives[j].attributes.texcoord1);
            if(model->meshes[i].primitives[j].attributes.hasColor) printf("\t\t\tCOLOR_0: %u\n", model->meshes[i].primitives[j].attributes.color);
            if(model->meshes[i].primitives[j].attributes.hasJoints) printf("\t\t\tJOINTS_0: %u\n", model->meshes[i].primitives[j].attributes.joints);
            if(model->meshes[i].primitives[j].attributes.hasWeights) printf("\t\t\tWEIGHTS_0: %u\n", model->meshes[i].primitives[j].attributes.weights);
        }
        printf("]\n");
    }

    // Accessors
    printf("----------------------------------------\n");
    for(u32 i = 0; i < model->accessorsCount; i++){
        printf("Accessor %u:\n", i);
        printf("\tComponentType: %u\n", model->accessors[i].componentType);
        printf("\tCount:         %u\n", model->accessors[i].count);
        printf("\tType:          %u\n", model->accessors[i].type);
        if(model->accessors[i].hasBufferView) printf("\tBufferView:    %u\n", model->accessors[i].bufferView);
        printf("\tByteOffset:    %lu\n", model->accessors[i].byteOffset);
        printf("\tNormalized:    %hhu\n", model->accessors[i].normalized);
        if(model->accessors[i].nameLength > 0) printf("\tName:          %s\n", model->accessors[i].name);
        if(model->accessors[i].hasSparse){
            printf("\tSparse:\n");
            printf("\t\tCount: %lu\n", model->accessors[i].sparse.count);
            printf("\t\tIndices:\n");
            printf("\t\t\tBufferView: %u\n", model->accessors[i].sparse.indices.bufferView);
            printf("\t\t\tByteOffset: %lu\n", model->accessors[i].sparse.indices.byteOffset);
            printf("\t\t\tComponentType: %u\n", model->accessors[i].sparse.indices.componentType);
            printf("\t\tValues:\n");
            printf("\t\t\tBufferView: %u\n", model->accessors[i].sparse.values.bufferView);
            printf("\t\t\tByteOffset: %lu\n", model->accessors[i].sparse.values.byteOffset);
        }
    }

    // Buffer Views
    printf("----------------------------------------\n");
    for(u32 i = 0; i < model->bufferViewsCount; i++){
        printf("Buffer View %u:\n", i);
        printf("\tBuffer:      %u\n", model->bufferViews[i].buffer);
        printf("\tByte Offset: %lu\n", model->bufferViews[i].byteOffset);
        printf("\tByte Length: %lu\n", model->bufferViews[i].byteLength);
        printf("\tByte Stride: %hhu\n", model->bufferViews[i].byteStride);
        if(model->bufferViews[i].hasTarget) printf("\tTarget:      %u\n", model->bufferViews[i].target);
        if(model->bufferViews[i].nameLength > 0) printf("\tName:        %s\n", model->bufferViews[i].name);
    }

    // Buffers
    printf("----------------------------------------\n");

    for(u32 i = 0; i < model->buffersCount; i++){
        printf("Buffer %u:\n", i);
        printf("\tByteLength: %lu\n", model->buffers[i].byteLength);
        if(model->buffers[i].nameLength > 0) printf("\tName:       %s", model->buffers[i].name);
        if(model->buffers[i].uriLength > 0) printf("\tUri:        %s", model->buffers[i].uri);       
    }

    printf("========================================\n");
}

