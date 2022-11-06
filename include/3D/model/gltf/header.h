#ifndef GLTF_HEADER_H
#define GLTF_HEADER_H

#include <3D/model/gltf/common.h>

#define GLB_MAGIC 0x46546C67 // glTF in ASCII, used to determine if file is a gltf file
#define GLB_CHUNK_STRUCTURED_JSON 0x4E4F534A
#define GLB_CHUNK_BINARY_BUFFER 0x004E4942
#define GLB_CHUNK_BYTE_ALIGNMENT 4

/**
 * @brief Every GLTF file has a 12 bit header with some basic info:
 * an identifier code, the version number and the length of the file (including header) 
 */
typedef struct {
    u32 magic;
    u32 version;
    u32 length;
} GLTFHeader;

/**
 * @brief Two chunks can exist in a GLB file, the JSON and the buffer. Both sections have
 * a short header which specifies which chunk this section is and what the length of the section is. 
 */
typedef struct {
    u32 length;
    u32 type;
} GLTFChunkHeader;

#endif