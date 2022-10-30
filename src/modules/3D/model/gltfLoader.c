#include <3D/model/gltfLoader.h>

uint8_t accessorTypeElementCounts[7] = {1, 2, 3, 4, 4, 9, 16};
uint8_t accessorComponentTypeByteCounts[] = {1, 1, 2, 2, 0, 4, 4};

GLTFAccessorType accessorTypeStrToInt(const char* type){
    if(!strcmp(type, "SCALAR")) return GLTF_ACCESSOR_TYPE_SCALAR;
    else if(!strcmp(type, "VEC2")) return GLTF_ACCESSOR_TYPE_VEC2;
    else if(!strcmp(type, "VEC3")) return GLTF_ACCESSOR_TYPE_VEC3;
    else if(!strcmp(type, "VEC4")) return GLTF_ACCESSOR_TYPE_VEC4;
    else if(!strcmp(type, "MAT2")) return GLTF_ACCESSOR_TYPE_MAT2;
    else if(!strcmp(type, "MAT3")) return GLTF_ACCESSOR_TYPE_MAT3;
    else if(!strcmp(type, "MAT4")) return GLTF_ACCESSOR_TYPE_MAT4;
    return 0;
}

void releaseModelInfo(GLBModel* model){
    free(model);
}

char prepareGLBRead(const char* file_name, GLBModel* model, char* error_msg){
    //========== START ==========//
    uint64_t bufferLengthAccumulator = 0;

    printf("Cool1\n");

    //---------- READING JSON FROM FILE ----------//
    FILE* file = fopen(file_name, "rb");
    if (!file){
        fprintf(stderr, "Failed to open file: %s\n", file_name);
        return -1;
    }

    printf("Cool2\n");

    GLBHeader header;
    fread(&header, sizeof(GLBHeader), 1, file);
    if(header.magic != GLB_IDENTIFICATION_CODE){
        fprintf(stderr, "File is not GLTF");
        return -2;
    }  

    printf("Cool3\n");

    GLBChunkHeader jsonHeader;
    fread(&jsonHeader, sizeof(GLBChunkHeader), 1, file);
    
    char json_data[jsonHeader.chunkLength + 1];
    fread(json_data, jsonHeader.chunkLength, 1, file);
    json_data[jsonHeader.chunkLength] = '\0';
    
    //---------- PARSING JSON ----------//
    model->allocator = zj_NewAllocator();
    zj_Value* parsed_json = NewValue(model->allocator);
    if(zj_ParseFast(parsed_json, json_data) != 1) return -1;

    printf("Cool\n");

    //========== START READING MODEL ==========//
    //---------- Asset ----------//
    zj_Value* asset = zj_ObjGet(parsed_json, "asset");
    // version
    const char* asset_version = zj_GetStr(zj_ObjGet(asset, "version"));
    sscanf(asset_version, "%hhu.%hhu", &model->asset.versionMajor, &model->asset.versionMinor);

    // minVersion
    model->asset.hasMinVersion = false;
    zj_Value* minVersion = zj_ObjGet(asset, "minVersion");
    if(minVersion != 0){
        const char* minVersionStr = zj_GetStr(minVersion);
        sscanf(minVersionStr, "%hhu.%hhu", &model->asset.minVersionMajor, &model->asset.minVersionMinor);
        model->asset.hasMinVersion = true;
    }

    printf("Cool\n");

    // generator
    model->asset.generatorLength = 0;
    zj_Value* generator = zj_ObjGet(asset, "generator");
    if(generator != 0){
        model->asset.generator = zj_GetStr(generator);
        model->asset.generatorLength = strlen(model->asset.generator) + 1;
        bufferLengthAccumulator += model->asset.generatorLength;
    }

    printf("Cool\n");

    // copyright
    model->asset.copyrightLength = 0;
    zj_Value* copyright = zj_ObjGet(asset, "copyright");
    if(copyright != 0){
        model->asset.copyright = zj_GetStr(copyright);
        model->asset.copyrightLength = strlen(model->asset.copyright) + 1;
        bufferLengthAccumulator += model->asset.copyrightLength;
    }

    //========== END READING MODEL ==========//
    printf("Cool\n");
    //========== START FILLING MODEL BUFFER ==========//
    uint64_t bufferPointer = 0;

    if(bufferLengthAccumulator != 0){
        GLBModel* temp = model;
        model = malloc(sizeof(GLBModel) + bufferLengthAccumulator);
        memcpy(model, temp, sizeof(GLBModel));
    }    
    // Asset
    if(model->asset.generatorLength > 0){
        strncpy(&model->data[bufferPointer], model->asset.generator, model->asset.generatorLength);
        model->asset.generator = &model->data[bufferPointer];
        bufferPointer += model->asset.generatorLength;
        
    }

    if(model->asset.copyrightLength > 0){
        strncpy(&model->data[bufferPointer], model->asset.copyright, model->asset.copyrightLength);
        model->asset.copyright = &model->data[bufferPointer];
        bufferPointer += model->asset.copyrightLength;
    }

    // Delete zj_Allocator
    zj_ReleaseAllocator(model->allocator);
    

}



#ifdef NO_FREAD
#ifndef _WIN32
char loadGLTFBinaryData(GLTFModelInfo * buffer_data, Mesh * result){
    int file_d = open(buffer_data->bin_buffer_uri, (unsigned char)0);
    if(file_d == -1){
        fprintf(stderr, "Failed to open file: %s\n", buffer_data->bin_buffer_uri);
        return -1;
    }
    unsigned char loaded_data[sizeof(Vertex) * buffer_data->vertex_count];

    lseek(file_d, buffer_data->vertex_position_data_offset, SEEK_SET);
    read(file_d, loaded_data, sizeof(vec3) * buffer_data->vertex_count);

    lseek(file_d, buffer_data->vertex_normal_data_offset, SEEK_SET);
    read(file_d, loaded_data + buffer_data->vertex_normal_data_offset, sizeof(vec3) * buffer_data->vertex_count);

    lseek(file_d, buffer_data->index_data_offset, SEEK_SET);
    read(file_d, result->indices, sizeof(buffer_data->index_type) * buffer_data->index_count);

    unsigned int i;
    for(i = 0; i < buffer_data->vertex_count; i++){
        memcpy(&result->vertices[i].position, &loaded_data[i * sizeof(vec3) + buffer_data->vertex_position_data_offset], sizeof(vec3));
        memcpy(&result->vertices[i].normal, &loaded_data[i * sizeof(vec3) + buffer_data->vertex_normal_data_offset], sizeof(vec3));
    }

    return 0;
}
#endif
#else 
char loadGLTFBinaryData(GLTFModelInfo * buffer_data, Mesh * result){
    FILE * file = fopen(buffer_data->bin_buffer_uri, "rb");
    if(file == NULL){
        fprintf(stderr, "Failed to open file: %s\n", buffer_data->bin_buffer_uri);
        return -1;
    }

    fseek(file, buffer_data->vertex_position_data_offset, SEEK_SET);
    for(unsigned int i = 0; i < buffer_data->vertex_count; i++){
        fread(result->vertices[i].position, sizeof(vec3), 1, file);
    }

    fseek(file, buffer_data->vertex_normal_data_offset, SEEK_SET);
    for(unsigned int i = 0; i < buffer_data->vertex_count; i++){
        fread(result->vertices[i].normal, sizeof(vec3), 1, file);
    }

    fseek(file, buffer_data->index_data_offset, SEEK_SET);
    fread(result->indices, sizeof(unsigned short), buffer_data->index_count, file);

    return 0;
}
#endif

/**
 * @brief Get's a GLTF Model's info before doing the read. This is done so the reading can be as optimised as possible.
 * 
 * @param model_info a struct with model info where the info about the model will be stored
 * @return 0 if succes, <0 if not
 */
// char prepareGLBRead(const char* file_name, GLBModel* info, char* error_msg){
//     //---------- READING JSON FROM FILE ----------//
//     FILE* file = fopen(file_name, "rb");
//     if (!file){
//         fprintf(stderr, "Failed to open file: %s\n", file_name);
//         return -1;
//     }
//     fread(&info->header, sizeof(GLBHeader), 1, file);
//     if(info->header.magic != GLB_IDENTIFICATION_CODE){
//         fprintf(stderr, "File is not GLTF");
//         return -2;
//     }  
//     fread(&info->jsonChunkHeader, sizeof(GLBChunkHeader), 1, file);
//     char json_data[info->jsonChunkHeader.chunkLength + 1];
//     fread(json_data, info->jsonChunkHeader.chunkLength, 1, file);
//     json_data[info->jsonChunkHeader.chunkLength] = '\0';
    
//     //---------- PARSING JSON ----------//
//     info->allocator = zj_NewAllocator();
//     zj_Value* parsed_json = NewValue(info->allocator);
//     if(zj_ParseFast(parsed_json, json_data) != 1) return -1;

//     //---------- GETTING ASSET INFO ----------//
//     zj_Value* asset = zj_ObjGet(parsed_json, "asset");
    
//     // version
//     const char* asset_version = zj_GetStr(zj_ObjGet(asset, "version"));
//     sscanf(asset_version, "%hhu.%hhu", &info->asset.versionMajor, &info->asset.versionMinor);

//     // minVersion
//     info->asset.hasMinVersion = false;
//     zj_Value* minVersion = zj_ObjGet(asset, "minVersion");
//     if(minVersion != 0){
//         const char* minVersionStr = zj_GetStr(minVersion);
//         sscanf(minVersionStr, "%hhu.%hhu", &info->asset.minVersionMajor, &info->asset.minVersionMinor);
//         info->asset.hasMinVersion = true;
//     }

//     // generator
//     info->asset.hasGenerator = false;
//     zj_Value* generator = zj_ObjGet(asset, "generator");
//     if(generator != 0){
//         info->asset.generator = zj_GetStr(generator);
//         info->asset.hasGenerator = true;
//     }

//     // copyright
//     info->asset.hasCopyright = false;
//     zj_Value* copyright = zj_ObjGet(asset, "copyright");
//     if(generator != 0){
//         info->asset.generator = zj_GetStr(generator);
//         info->asset.hasGenerator = true;
//     }

//     //---------- GETTING ACCESSOR INFO ----------//
//     zj_Value* accessors = zj_ObjGet(parsed_json, "accessors");
//     info->accessorCount = 0;
//     while(zj_ArrayGet(accessors, info->accessorCount) != 0) info->accessorCount++;
//     info->accessors = malloc(sizeof(GLTFAccessor) * info->accessorCount);
//     for(uint32_t i = 0; i < info->accessorCount; i++){
//         zj_Value* accessor = zj_ArrayGet(accessors, i);

//         // bufferView
//         info->accessors[i].hasBufferView = 0;
//         zj_Value* bufferView = zj_ObjGet(accessor, "bufferView");
//         if(bufferView != 0){
//             info->accessors[i].hasBufferView = true;
//             info->accessors[i].bufferView = *zj_GetInt(bufferView);
//         }

//         // byteOffset
//         info->accessors[i].byteOffset = 0;
//         zj_Value* byteOffset = zj_ObjGet(accessor, "byteOffset");
//         if(byteOffset != 0) info->accessors[i].byteOffset = *zj_GetInt(byteOffset);

//         // componentType
//         info->accessors[i].componentType = *zj_GetInt(zj_ObjGet(accessor, "componentType")) - 5120;

//         // normalized
//         info->accessors[i].normalized = false;
//         zj_Value* normalized = zj_ObjGet(accessor, "normalized");
//         if(normalized != 0) info->accessors[i].normalized = *zj_GetBool(normalized);

//         // count
//         info->accessors[i].count = *zj_GetInt(zj_ObjGet(accessor, "count"));

//         // type
//         const char* type = zj_GetStr(zj_ObjGet(accessor, "type"));
//         info->accessors[i].type = accessorTypeStrToInt(type);

//         // max
//         info->accessors[i].hasMax = 0;
//         zj_Value* max = zj_ObjGet(accessor, "max");
//         if(max != 0){
//             // info->accessors[i].max = malloc(100);
//         }
//     }


//     //---------- GETTING SCENES ----------//
//     zj_Value* scenes_value = zj_ObjGet(parsed_json, "scenes");
//     uint32_t scene_counter = 0;
//     while (zj_ArrayGet(scenes_value, scene_counter) != 0)
//     {
//         scene_counter++;
//     }
// }
