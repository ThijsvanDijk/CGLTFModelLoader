#include <3D/model/gltf/asset.h>

u64 gltf_getAssetSize(zj_Value* asset){
    u64 bufferLengthAccumulator = 0;

    // generator
    zj_Value* generator = zj_ObjGet(asset, "generator");
    if(generator != 0) bufferLengthAccumulator += strlen(zj_GetStr(generator)) + 1;

    // copyright
    zj_Value* copyright = zj_ObjGet(asset, "copyright");
    if(copyright != 0) bufferLengthAccumulator += strlen(zj_GetStr(copyright)) + 1;

    return bufferLengthAccumulator;
}

byte* gltf_fillAssetBuffer(zj_Value* asset, GLTFModel* result, byte* bufferPointer){
    // version
    const char* asset_version = zj_GetStr(zj_ObjGet(asset, "version"));
    sscanf(asset_version, "%hhu.%hhu", &result->asset.versionMajor, &result->asset.versionMinor);

    // minVersion
    result->asset.hasMinVersion = false;
    zj_Value* minVersion = zj_ObjGet(asset, "minVersion");
    if(minVersion != 0){
        sscanf(zj_GetStr(minVersion), "%hhu.%hhu", &result->asset.minVersionMajor, &result->asset.minVersionMinor);
        result->asset.hasMinVersion = true;
    }

    // generator
    result->asset.generatorLength = 0;
    zj_Value* generator = zj_ObjGet(asset, "generator");
    if(generator != 0){
        result->asset.generator = bufferPointer;
        const char* generatorString = zj_GetStr(generator);
        size_t generatorLength = strlen(generatorString) + 1;
        strncpy(result->asset.generator, generatorString, generatorLength);
        result->asset.generatorLength = generatorLength;
        bufferPointer += generatorLength;
    }

    // copyright
    result->asset.copyrightLength = 0;
    zj_Value* copyright = zj_ObjGet(asset, "copyright");
    if(copyright != 0){
        result->asset.copyright = bufferPointer;
        const char* copyrightString = zj_GetStr(copyright);
        size_t copyrightLength = strlen(copyrightString) + 1;
        strncpy(result->asset.copyright, copyrightString, copyrightLength);
        result->asset.copyrightLength = copyrightLength;
        bufferPointer += copyrightLength;
    }

    return bufferPointer;
}