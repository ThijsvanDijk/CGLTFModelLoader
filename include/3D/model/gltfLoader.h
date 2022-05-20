#include <zzzjson/zzzjson.h>
#include <cglm/cglm.h>

typedef struct {
    vec3 position;
    // vec3 normal;
    // vec2 tex_coords;
    // vec3 tangent;
    // vec3 bitangent;
} Vertex;

typedef struct {
    Vertex * vertices;
    unsigned int * indices;
} Mesh;

typedef struct {
    Mesh ** meshes;
} Model;

typedef struct {
    char bin_buffer_uri[256];
    unsigned int vertex_position_count;
    unsigned int vertex_data_offset;
    unsigned int index_count;
    unsigned int index_data_offset;
} BufferInfo;

unsigned char getGLTFModelSize(const char * file, BufferInfo * data);
unsigned char loadGLTFBinaryData(BufferInfo * buffer_data, Mesh * result);