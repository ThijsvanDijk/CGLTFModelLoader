#include <3D/model/gltf/accessor.h>

static GLTFAccessorType accessorTypeStrToInt(const char* type){
    if(!strcmp(type, "SCALAR")) return GLTF_ACCESSOR_TYPE_SCALAR;
    else if(!strcmp(type, "VEC2")) return GLTF_ACCESSOR_TYPE_VEC2;
    else if(!strcmp(type, "VEC3")) return GLTF_ACCESSOR_TYPE_VEC3;
    else if(!strcmp(type, "VEC4")) return GLTF_ACCESSOR_TYPE_VEC4;
    else if(!strcmp(type, "MAT2")) return GLTF_ACCESSOR_TYPE_MAT2;
    else if(!strcmp(type, "MAT3")) return GLTF_ACCESSOR_TYPE_MAT3;
    else if(!strcmp(type, "MAT4")) return GLTF_ACCESSOR_TYPE_MAT4;
    return -1;
}

static const u8 accessorComponentTypeSizes[] = {
    1, 1, 2, 2, 0, 4, 4
};

static const u8 accessorTypeSizes[] = {
    1, 2, 3, 4, 4, 9, 16
};

u64 gltf_getAccessorsSize(zj_Value* accessors_json, GLTFModel* model){
    u64 bufferLengthAccumulator = 0;

    u32 accessorsCount = 0;
    zj_Value* currentAccessor;
    while(currentAccessor = zj_ArrayGet(accessors_json, accessorsCount)){
        accessorsCount++;

        zj_Value* name = zj_ObjGet(currentAccessor, "name");
        if(name != 0) bufferLengthAccumulator += strlen(zj_GetStr(name)) + 1;
    }
    model->accessorsCount = accessorsCount;
    bufferLengthAccumulator += sizeof(GLTFAccessor) * accessorsCount;

    return bufferLengthAccumulator;
}

i8 gltf_fillAccessorsBuffer(zj_Value* accessors_json, GLTFModel* model, void* bufferPointer){
    model->accessors = (GLTFAccessor*)bufferPointer;
    bufferPointer += model->accessorsCount * sizeof(GLTFAccessor);

    zj_Value* currentAccessor;
    for(uint32_t i = 0; i < model->accessorsCount; i++){
        currentAccessor = zj_ArrayGet(accessors_json, i);

        // componentType
        model->accessors[i].componentType = *zj_GetInt(zj_ObjGet(currentAccessor, "componentType")) - 5120;

        // count
        model->accessors[i].count = *zj_GetInt(zj_ObjGet(currentAccessor, "count"));

        // type
        model->accessors[i].type = accessorTypeStrToInt(zj_GetStr(zj_ObjGet(currentAccessor, "type")));

        // bufferView
        model->accessors[i].hasBufferView = false;
        zj_Value* bufferView = zj_ObjGet(currentAccessor, "bufferView");
        if(bufferView){
            model->accessors[i].bufferView = *zj_GetInt(bufferView);
            model->accessors[i].hasBufferView = true;
        }

        // byteOffset
        model->accessors[i].byteOffset = 0;
        zj_Value* byteOffset = zj_ObjGet(currentAccessor, "byteOffset");
        if(byteOffset) model->accessors[i].byteOffset = *zj_GetInt(byteOffset);

        // normalized
        model->accessors[i].normalized = false;
        zj_Value* normalized = zj_ObjGet(currentAccessor, "normalized");
        if(normalized) model->accessors[i].normalized = true;

        // Sparse
        model->accessors[i].hasSparse = false;
        zj_Value* sparse = zj_ObjGet(currentAccessor, "sparse");
        if(sparse){
            // Count
            model->accessors[i].sparse.count = *zj_GetInt(zj_ObjGet(sparse, "count"));

            // Indices
            zj_Value* indices = zj_ObjGet(sparse, "indices");
            model->accessors[i].sparse.indices.bufferView = *zj_GetInt(zj_ObjGet(indices, "bufferView"));

            model->accessors[i].sparse.indices.byteOffset = 0;
            zj_Value* indicesByteOffset = zj_ObjGet(indices, "byteOffset");
            if(indicesByteOffset) model->accessors[i].sparse.indices.byteOffset = (u64)*zj_GetInt(zj_ObjGet(indices, "byteOffset"));

            model->accessors[i].sparse.indices.componentType = *zj_GetInt(zj_ObjGet(indices, "componentType")) - 5120;

            // Values
            zj_Value* values = zj_ObjGet(sparse, "values");
            
            model->accessors[i].sparse.values.bufferView = (u32)*zj_GetInt(zj_ObjGet(values, "bufferView"));

            model->accessors[i].sparse.values.byteOffset = 0;
            zj_Value* valuesByteOffset = zj_ObjGet(values, "byteOffset");
            if(valuesByteOffset) model->accessors[i].sparse.values.byteOffset = (u64)*zj_GetInt(zj_ObjGet(values, "byteOffset"));

        }

        // name
        model->accessors[i].nameLength = 0;
        zj_Value* name = zj_ObjGet(currentAccessor, "name");
        if(name){
            const char * name_str = zj_GetStr(name);
            u32 name_str_length = strlen(zj_GetStr(name)) + 1;
            strncpy(bufferPointer, name_str, name_str_length);
            model->accessors[i].nameLength = name_str_length;
            model->accessors[i].name = bufferPointer;
            bufferPointer += name_str_length;
        }
    }

    return 0;
}