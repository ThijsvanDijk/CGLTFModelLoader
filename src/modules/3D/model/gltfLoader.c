#include <3D/model/gltfLoader.h>

unsigned char getGLTFModelSize(const char * filename, BufferInfo * data){
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
    Value *src_v = NewValue(A);
    unsigned char ret = ParseFast(src_v, gltf_json);
    if(ret != 1){
        fprintf(stderr, "ParseFast Fail!\n");
        ReleaseAllocator(A);
        return -1;
    }

    //---------- GET ACCESSORS ----------//
    Value * v = ObjGet(src_v, "meshes");
    if(v == 0){
        fprintf(stderr, "No mesh data found\n");
        ReleaseAllocator(A);
        return -1;
    }
    v = ArrayGet(ObjGet(ArrayGet(v, 0), "primitives"), 0);
    Value * w = ObjGet(v, "indices");
    if(v == 0){
        fprintf(stderr, "No index data found\n");
        ReleaseAllocator(A);
        return -1;
    }
    v = ObjGet(ObjGet(v, "attributes"), "POSITION");
    if(v == 0){
        fprintf(stderr, "No position data found for vertices\n");
        ReleaseAllocator(A);
        return -1;
    }
    const int * vertex_position_accessor = GetInt(v);
    const int * index_data_accessor = GetInt(w);     

    //---------- GET BUFFERVIEWS AND COUNTS ----------//
    Value * z = ObjGet(src_v, "accessors");
    w = ArrayGet(z, *index_data_accessor);
    v = ArrayGet(z, *vertex_position_accessor);
    z = ObjGet(w, "count");
    data->index_count = *GetInt(z);
    z = ObjGet(v, "count");    
    data->vertex_position_count = *GetInt(z);
    z = ObjGet(w, "bufferView");
    const int * index_bufferview = GetInt(z);
    z = ObjGet(v, "bufferView");
    const int * vertex_bufferview = GetInt(z);

    //---------- GET BUFFERS AND OFFSETS ----------//
    z = ObjGet(src_v, "bufferViews");
    w = ArrayGet(z, *index_bufferview);
    v = ArrayGet(z, *vertex_bufferview);
    data->index_data_offset = *GetInt(ObjGet(w, "byteOffset"));
    const int * buffer = GetInt(ObjGet(w, "buffer"));
    data->vertex_data_offset = *GetInt(ObjGet(v, "byteOffset"));

    //---------- GET BUFFERS URI ----------//
    z = ObjGet(src_v, "buffers");
    w = ArrayGet(z, *buffer);
    const char * uri = GetStr(ObjGet(w, "uri"));
    const char * last_directory = strrchr(filename, '/');
    memset(data->bin_buffer_uri, 0, 256);
    strncpy(data->bin_buffer_uri, filename, (last_directory - filename) + 1);
    strncpy((last_directory - filename) + data->bin_buffer_uri + 1, uri, 256 - (last_directory - filename) - 1);

    ReleaseAllocator(A);
    return 0;
}

unsigned char loadGLTFBinaryData(BufferInfo * buffer_data, Mesh * result){
    FILE *file = fopen(buffer_data->bin_buffer_uri, "rb");
    if(file == NULL){
        fprintf(stderr, "Failed to open file: %s\n", buffer_data->bin_buffer_uri);
        return -1;
    }

    fseek(file, buffer_data->vertex_data_offset, SEEK_SET);
    fread(result->vertices, sizeof(vec3), buffer_data->vertex_position_count, file);
    fseek(file, buffer_data->index_data_offset, SEEK_SET);
    fread(result->indices, sizeof(unsigned short), buffer_data->index_count, file);

    return 0;
}
