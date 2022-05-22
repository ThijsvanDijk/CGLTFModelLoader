#include <zzzjson/zzzjson.h>
#include <cglm/cglm.h>
#include <glad/glad.h>
#include <unistd.h>

typedef struct {
    vec3 position;
    vec3 normal;
    // vec2 tex_coords;
    // vec3 tangent;
    // vec3 bitangent;
} Vertex;

typedef struct {
    Vertex * vertices;
    unsigned int * indices;
} Mesh;

typedef struct {
    unsigned int glType;
    unsigned int gltfType;
} componentType;

typedef struct {
    char bin_buffer_uri[256];
    unsigned int vertex_count, vertex_position_data_offset, vertex_normal_data_offset, vertex_texcoord_data_offset;
    unsigned int index_count, index_data_offset;
    unsigned int vertex_position_type, vertex_normal_type, vertex_texcoord_type, index_type;
} BufferInfo;

unsigned char getGLTFModelInfo(const char * file, BufferInfo * data);
char loadGLTFBinaryData(BufferInfo * buffer_data, Mesh * result);