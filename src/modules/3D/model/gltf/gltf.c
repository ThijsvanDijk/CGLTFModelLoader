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

static __always_inline u64 gltf_getModelBufferSize(zj_Value* parsed_json, GLTFModel* model){
    u64 bufferSizeAccumulator = 0;
    // zj_Value* accessors = zj_ObjGet(parsed_json, "accessors");
    // zj_Value* animations = zj_ObjGet(parsed_json, "animations");
    zj_Value* asset = zj_ObjGet(parsed_json, "asset");
    zj_Value* buffers = zj_ObjGet(parsed_json, "buffers");
    zj_Value* bufferViews = zj_ObjGet(parsed_json, "bufferViews");
    // zj_Value* cameras = zj_ObjGet(parsed_json, "cameras");
    // zj_Value* images = zj_ObjGet(parsed_json, "images");
    // zj_Value* materials = zj_ObjGet(parsed_json, "materials");
    // zj_Value* meshes = zj_ObjGet(parsed_json, "meshes");
    zj_Value* nodes = zj_ObjGet(parsed_json, "nodes");
    // zj_Value* samplers = zj_ObjGet(parsed_json, "samplers");
    zj_Value* scenes = zj_ObjGet(parsed_json, "scenes");
    // zj_Value* skins = zj_ObjGet(parsed_json, "skins");
    // zj_Value* textures = zj_ObjGet(parsed_json, "textures");


    // if(accessors != NULL) bufferSizeAccumulator += gltf_getAccessorsSize(accessors);
    // if(animations != NULL) bufferSizeAccumulator += gltf_getAnimationsSize(animations);
    bufferSizeAccumulator += gltf_getAssetSize(asset);
    if(buffers != NULL) bufferSizeAccumulator += gltf_getBuffersSize(buffers, model);
    if(bufferViews != NULL) bufferSizeAccumulator += gltf_getBufferViewsSize(bufferViews, model);
    // if(cameras != NULL) bufferSizeAccumulator += gltf_getCamerasSize(cameras);
    // if(images != NULL) bufferSizeAccumulator += gltf_getImagesSize(images);
    // if(materials != NULL) bufferSizeAccumulator += gltf_getMaterialsSize(materials);
    // if(meshes != NULL) bufferSizeAccumulator += gltf_getMeshesSize(meshes);
    if(nodes != NULL) bufferSizeAccumulator += gltf_getNodesSize(nodes, model);
    // if(samplers != NULL) bufferSizeAccumulator += gltf_getSamplersSize(samplers);
    if(scenes != NULL) bufferSizeAccumulator += gltf_getScenesSize(scenes, model);
    // if(skins != NULL) bufferSizeAccumulator += gltf_getSkinsSize(skins);
    // if(textures != NULL) bufferSizeAccumulator += gltf_getTexturesSize(textures);
    
    return bufferSizeAccumulator;
}

static __always_inline i8 gltf_fillModelData(zj_Value* parsed_json, GLTFModel* model){
    byte* bufferPointer = model->data;
    // zj_Value* accessors = zj_ObjGet(parsed_json, "accessors");
    // zj_Value* animations = zj_ObjGet(parsed_json, "animations");
    zj_Value* asset = zj_ObjGet(parsed_json, "asset");
    zj_Value* buffers = zj_ObjGet(parsed_json, "buffers");
    zj_Value* bufferViews = zj_ObjGet(parsed_json, "bufferViews");
    // zj_Value* cameras = zj_ObjGet(parsed_json, "cameras");
    // zj_Value* images = zj_ObjGet(parsed_json, "images");
    // zj_Value* materials = zj_ObjGet(parsed_json, "materials");
    // zj_Value* meshes = zj_ObjGet(parsed_json, "meshes");
    zj_Value* nodes = zj_ObjGet(parsed_json, "nodes");
    // zj_Value* samplers = zj_ObjGet(parsed_json, "samplers");
    zj_Value* scenes = zj_ObjGet(parsed_json, "scenes");
    // zj_Value* skins = zj_ObjGet(parsed_json, "skins");
    // zj_Value* textures = zj_ObjGet(parsed_json, "textures");

    bufferPointer = gltf_fillAssetBuffer(asset, model, bufferPointer);
    if(buffers != 0) bufferPointer = gltf_fillBuffersBuffer(buffers, model, bufferPointer);
    if(bufferViews != 0) bufferPointer = gltf_fillBufferViewsBuffer(bufferViews, model, bufferPointer); 
    if(scenes != 0) bufferPointer = gltf_fillScenesBuffer(scenes, model, bufferPointer); 
    if(nodes != 0) bufferPointer = gltf_fillNodesBuffer(nodes, model, bufferPointer);
      
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
    byte fileBuffer[fileLength];
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
    gltf_fillModelData(parsed_json, model);

    free(fileBuffer);
    zj_ReleaseAllocator(allocator);
}

