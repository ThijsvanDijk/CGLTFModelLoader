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
