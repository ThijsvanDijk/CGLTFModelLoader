#include <3D/model/gltfLoader.h>

#define NO_FREAD

static const componentType types[] = {
    {GL_BYTE, 5120},
    {GL_UNSIGNED_BYTE, 5121},
    {GL_SHORT, 5122},
    {GL_UNSIGNED_SHORT, 5123},
    {GL_UNSIGNED_INT, 5125},
    {GL_FLOAT, 5126}
};

unsigned int getGLComponentTypeFromGLTFType(unsigned int type){
    for(unsigned int i = 0; i < sizeof(types)/sizeof(componentType); i++){
        if(types[i].gltfType == type) return types[i].glType;
    }
    return 0;
}

unsigned char getGLTFModelInfo(const char * filename, BufferInfo * data){
    //---------- READING GLTF FILE ----------//
    FILE *file = fopen(filename, "rb");
    if(file == NULL){
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return -1;
    }
    fseek(file, 0L, SEEK_END);
    unsigned long file_size = ftell(file);
    rewind(file);

    char gltf_json[file_size + 1];
    gltf_json[file_size] = 0;
    fread(gltf_json, file_size, 1, file);

    fclose(file);

    //---------- PARSING JSON ----------//
    Allocator *A = NewAllocator();
    Value *parsed_json = NewValue(A);
    unsigned char ret = ParseFast(parsed_json, gltf_json);
    if(ret != 1){
        fprintf(stderr, "ParseFast Fail!\n");
        ReleaseAllocator(A);
        return -1;
    }

    //---------- GET ACCESSORS ----------//
    Value * meshes = ObjGet(parsed_json, "meshes");
    if(meshes == 0){
        fprintf(stderr, "No mesh data found\n");
        ReleaseAllocator(A);
        return -1;
    }
    Value * mesh_first_primitives = ArrayGet(ObjGet(ArrayGet(meshes, 0), "primitives"), 0);
    const int * index_accessor_index = GetInt(ObjGet(mesh_first_primitives, "indices"));
    if(index_accessor_index == 0){
        fprintf(stderr, "No index data found\n");
        ReleaseAllocator(A);
        return -1;
    }
    const int * vertex_position_accessor_index = GetInt(ObjGet(ObjGet(mesh_first_primitives, "attributes"), "POSITION"));
    if(vertex_position_accessor_index == 0){
        fprintf(stderr, "No position data found for vertices\n");
        ReleaseAllocator(A);
        return -1;
    }

    const int * vertex_normal_accessor_index = GetInt(ObjGet(ObjGet(mesh_first_primitives, "attributes"), "NORMAL"));
    if(vertex_normal_accessor_index == 0){
        fprintf(stderr, "No normal data found for vertices\n");
        ReleaseAllocator(A);
        return -1;
    }      

    //---------- GET BUFFERVIEWS AND COUNTS ----------//
    Value * accessors = ObjGet(parsed_json, "accessors");
    
    Value * index_accessor = ArrayGet(accessors, *index_accessor_index);
    Value * vertex_position_accessor = ArrayGet(accessors, *vertex_position_accessor_index);
    Value * vertex_normal_accessor = ArrayGet(accessors, *vertex_normal_accessor_index);
    
    data->index_count = *GetInt(ObjGet(index_accessor, "count"));    
    data->vertex_count = *GetInt(ObjGet(vertex_position_accessor, "count"));
    
    const int * index_bufferview_index = GetInt(ObjGet(index_accessor, "bufferView"));
    const int * vertex_position_bufferview_index = GetInt(ObjGet(vertex_position_accessor, "bufferView"));
    const int * vertex_normal_bufferview_index = GetInt(ObjGet(vertex_normal_accessor, "bufferView"));
    
    data->index_type = getGLComponentTypeFromGLTFType(*GetInt(ObjGet(index_accessor, "componentType")));
    data->vertex_position_type = getGLComponentTypeFromGLTFType(*GetInt(ObjGet(vertex_position_accessor, "componentType")));
    data->vertex_normal_type = getGLComponentTypeFromGLTFType(*GetInt(ObjGet(vertex_normal_accessor, "componentType")));

    //---------- GET BUFFERS AND OFFSETS ----------//
    Value * bufferviews = ObjGet(parsed_json, "bufferViews");
    
    Value * index_bufferview = ArrayGet(bufferviews, *index_bufferview_index);
    Value * vertex_position_bufferview = ArrayGet(bufferviews, *vertex_position_bufferview_index);
    Value * vertex_normal_bufferview = ArrayGet(bufferviews, *vertex_normal_bufferview_index);
    
    data->index_data_offset = *GetInt(ObjGet(index_bufferview, "byteOffset"));
    data->vertex_position_data_offset = *GetInt(ObjGet(vertex_position_bufferview, "byteOffset"));
    data->vertex_normal_data_offset = *GetInt(ObjGet(vertex_normal_bufferview, "byteOffset"));

    const int * buffer_index = GetInt(ObjGet(index_bufferview, "buffer"));

    //---------- GET BUFFERS URI ----------//
    Value * buffer = ArrayGet(ObjGet(parsed_json, "buffers"), *buffer_index);
    const char * uri = GetStr(ObjGet(buffer, "uri"));
    const char * last_directory = strrchr(filename, '/');
    memset(data->bin_buffer_uri, 0, 256);
    strncpy(data->bin_buffer_uri, filename, (last_directory - filename) + 1);
    strncpy((last_directory - filename) + data->bin_buffer_uri + 1, uri, 256 - (last_directory - filename) - 1);

    ReleaseAllocator(A);
    return 0;
}

#ifdef NO_FREAD
char loadGLTFBinaryData(BufferInfo * buffer_data, Mesh * result){
    int file_d = open(buffer_data->bin_buffer_uri, (unsigned char)0);
    if(file_d == -1){
        fprintf(stderr, "Failed to open file: %s\n", buffer_data->bin_buffer_uri);
        return -1;
    }
    unsigned char loaded_data[2 * sizeof(vec3) * buffer_data->vertex_count];

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
#else 
char loadGLTFBinaryData(BufferInfo * buffer_data, Mesh * result){
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
