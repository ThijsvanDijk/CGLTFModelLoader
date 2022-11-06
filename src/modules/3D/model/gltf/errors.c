#include <3D/model/gltf/errors.h>



i8 gltf_error(u16 code){
    static const char errorMessages[GLTF_NOERRORMSG][GLTF_MAXLENERRORMSG] = {
        "File not found",
        "File is not of type glTF",
        "JSON could not be parsed"
    };

    if(code >= 0 && code < GLTF_NOERRORMSG){
        printf("Error: %s\n", errorMessages[code]);
        return -1;
    } 
}