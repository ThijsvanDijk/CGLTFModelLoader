#include <zzzjson/zzzjson.h>
#include <cglm/cglm.h>
#include <glad/glad.h>
#ifndef _WIN32
    #include <unistd.h>
#endif
#include <fcntl.h>

#define GLB_IDENTIFICATION_CODE 0x46546C67
#define GLB_CHUNK_STRUCTURED_JSON 0x4E4F534A
#define GLB_CHUNK_BINARY_BUFFER 0x004E4942
#define GLB_CHUNK_BYTE_ALIGNMENT 4
#define GLTF_VERSION 2


/**
 * @brief Vertices are made of:
 * position vectors (float[3]),
 * normal vectors (float[3]) 
 */
typedef struct {
    vec3 position;
    vec3 normal;
    // vec2 tex_coords;
    // vec3 tangent;
    // vec3 bitangent;
} Vertex;

/**
 * @brief Meshes are used to store vertex and index data loaded from 3d models 
 */
typedef struct {
    Vertex * vertices;
    unsigned int * indices;
} Mesh;

/**
 * @brief Struct that helps convert GLTF type codes to OpenGL internal codes 
 */
typedef struct {
    unsigned int glType;
    unsigned short gltfType;
    unsigned short amountOfBits;
} ComponentType;

/**
 * @brief Every GLTF file has a 12 bit header with some basic info:
 * an identifier code, the version number and the length of the file (including header) 
 */
typedef struct {
    unsigned int magic, version, length;
} GLTFHeader;

/**
 * @brief Two chunks can exist in a GLB file, the JSON and the buffer. Both sections have
 * a short header which specifies which kind this section is and what the length of the section is. 
 */
typedef struct {
    unsigned int chunkLength, chunkType;
} GLTFChunkHeader;

/**
 * @brief Is used to store information about a GLTF File. This is done so the reading of the data can be done
 * as uninterrupted as possible. 
 */
typedef struct {
    Allocator * allocator;
    char bin_buffer_uri[256];
    const char * json_data;
    unsigned int json_data_size;
    unsigned int vertex_count, vertex_position_data_offset, vertex_normal_data_offset, vertex_texcoord_data_offset;
    unsigned int index_count, index_data_offset, index_type;
    unsigned int vertex_position_type, vertex_normal_type, vertex_texcoord_type;
    unsigned int glb_size;
} BufferInfo;

#ifndef _WIN32
/**
 * @brief Convert a GLTF file into a optimised GLB format where data is stored exactly as OpenGL works with it,
 * allowing possible future extension with DirectStorage like technologies, drastically reducing load times.
 * 
 * @param info The Bufferinfo that contains info about the model that needs to be converted
 * @param mesh The mesh where the model data is stored
 * @param filename The created GLB file is stored here
 * @return char Returns 0 if succesful, if 0 is not returned: error
 */
char convertGLTFToGLB(BufferInfo * info, Mesh * mesh, const char * filename);
#endif

/**
 * @brief Gets info about a model before loading occurs
 * 
 * @param file File to get info from
 * @param data Info struct to fill in the model info from file
 * @return char 0 if succesful, < 0 if unsuccesful
 */
char getGLTFModelInfo(const char * file, BufferInfo * data);

/**
 * @brief Reads the vertices and indices from the buffer and stores these in result
 * 
 * @param buffer_data Info struct with info about the 3D model
 * @param result The Mesh where the data is stored
 * @return char 0 if succesful, < 0 if unsuccesful
 */
char loadGLTFBinaryData(BufferInfo * buffer_data, Mesh * result);

/**
 * @brief UNDER CONSTRUCTION! Will eventually read the optimised GLB file and store data in result
 * 
 * @param file File to read the data from
 */
void readGLB(const char * file);