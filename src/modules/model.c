#include <internal/model.h>

//==============================================================================//

unsigned long engine_model_allocate_bytes(const struct aiScene* scene){
    unsigned long va = 0, ia = 0, ta = 0, ma = 0;
    
    engine_model_recursive_scene_size(scene->mRootNode, scene, &va, &ia, &ma); 
    
    return (    (va * sizeof(Vertex)) + 
                (ia * sizeof(unsigned int)) + 
                ((ma * sizeof(Mesh)) - (ma * 3 * sizeof(Vertex*))) + 
                sizeof(Model) - sizeof(unsigned int)
            );       
}

//==============================================================================//

void engine_model_recursive_scene_size(struct aiNode* node, struct aiScene* scene, unsigned long *va, unsigned long *ia, unsigned long* ma){
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        *va += mesh->mNumVertices;
        *ia += mesh->mNumFaces * 3;
        *ma += 1;
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++){
        engine_model_recursive_scene_size(node->mChildren[i], scene, va, ia, ma);
    }
}

//==============================================================================//

Model* engine_model_make(const char* path){
    
    const struct aiScene* scene = aiImportFile(path, 
        aiProcess_CalcTangentSpace      |
        aiProcess_Triangulate           | // Guarantees that amount of indices per face is 3
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType
    );
    if(scene == NULL){
        printf("Failure loading model at: %s\n", path);
        return -1;
    }

    unsigned long number_of_bytes = engine_model_allocate_bytes(scene);
    Model* model = malloc(number_of_bytes);

    engine_model_fill_data_from_scene(scene, model);   

    aiReleaseImport(scene); 

    return model;
}

//==============================================================================//

void engine_model_fill_data_from_scene(struct aiScene* scene, Model* model){
    model->meshes_count = scene->mNumMeshes; // Verified
    void *current_write_position = &model->meshes; // Start writing where Mesh pointer starts

    engine_model_fill_from_node(scene, scene->mRootNode, current_write_position);
}

//==============================================================================//

void engine_model_fill_from_node(struct aiScene* scene, struct aiNode* node, void *write){
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        engine_model_fill_mesh(mesh, scene, write);
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++){
        engine_model_fill_from_node(scene, node->mChildren[i], write);
    }
}

//==============================================================================//
// This is where the magic happens
void engine_model_fill_mesh(struct aiMesh* mesh, struct aiScene* scene, void* write_position){
    // Set amount of elements
    memcpy(write_position + offsetof(Mesh, vertex_count), &mesh->mNumVertices, sizeof(unsigned int));
    
    unsigned int index_amount = mesh->mNumFaces * 3;
    printf("Amount of indices: %u\n", index_amount);

    memcpy(write_position + offsetof(Mesh, index_count), &index_amount, sizeof(unsigned int));

    // memcpy(write_position + offsetof(Mesh, texture_count), mesh->mNumVertices, sizeof(unsigned int));


    /*/--------------------------------------------------------------------------///
    |   Getting Vertices    VERIFIED                                               |
    ///--------------------------------------------------------------------------/*/
    write_position += offsetof(Mesh, vertices); // Now writing at vertex data start position 
    unsigned int counter = 0;
    for(unsigned int i = 0; i < mesh->mNumVertices; i++){
        // Copy Position data to mesh
        void *current_vertex_ptr = write_position + i * sizeof(Vertex);
        memcpy(current_vertex_ptr + offsetof(Vertex, position),     &mesh->mVertices[i],            sizeof(vec3));
        
        // Copy Normal data to mesh
        memcpy(current_vertex_ptr + offsetof(Vertex, normal),       &mesh->mNormals[i],             sizeof(vec3));

        // Copy Tex Coords to mesh
        memcpy(current_vertex_ptr + offsetof(Vertex, tex_coords),   &mesh->mTextureCoords[0][i],    sizeof(vec2));

        // Copy Tangents to mesh
        memcpy(current_vertex_ptr + offsetof(Vertex, tangent),      &mesh->mTangents[i],            sizeof(vec3));

        // Copy Bitangents to mesh
        memcpy(current_vertex_ptr + offsetof(Vertex, bitangent),    &mesh->mBitangents[i],          sizeof(vec3));
    
        counter += sizeof(Vertex);
    }

    /*/--------------------------------------------------------------------------///
    |   Getting Indices                                                            |
    ///--------------------------------------------------------------------------/*/ 
    write_position += mesh->mNumVertices * sizeof(Vertex); // Now writing at index data start position VERIFIED
    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        memcpy(write_position + (i * 3 * sizeof(unsigned int)), mesh->mFaces[i].mIndices, 3 * sizeof(unsigned int));
    }

    printf("%lu\n", sizeof(Model));

    return;
}   