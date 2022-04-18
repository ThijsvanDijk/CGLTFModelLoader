#include <internal/model.h>

//==============================================================================//

void engine_model_draw(Model* model, unsigned int shader){
    for(unsigned int i = 0; i < model->meshes_count; i++){
        engine_mesh_draw((Mesh*)(model + sizeof(Model) + (i * sizeof(Mesh*))), shader);
    }
}

//==============================================================================//

unsigned long engine_model_allocate_bytes(const struct aiScene* scene){
    unsigned long va = 0, ia = 0, ma = 0;
    
    engine_model_recursive_scene_size(scene->mRootNode, scene, &va, &ia, &ma); 

    return (    sizeof(Model)       +
                ma * sizeof(Mesh*)  +
                ma * sizeof(Mesh)   +
                va * sizeof(Vertex) +
                ia * sizeof(unsigned int)
            );       
}

//==============================================================================//

void engine_model_recursive_scene_size(const struct aiNode* node, const struct aiScene* scene, unsigned long *va, unsigned long *ia, unsigned long* ma){
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
        exit(EXIT_FAILURE);
    }

    unsigned long number_of_bytes = engine_model_allocate_bytes(scene);
    Model* model = malloc(number_of_bytes);

    engine_model_fill_data_from_scene(scene, model);   

    aiReleaseImport(scene); 

    return model;
}

//==============================================================================//

void engine_model_fill_data_from_scene(const struct aiScene* scene, Model* model){
    model->meshes_count = scene->mNumMeshes; // Verified

    void *current_write_position = model + sizeof(Model) + (model->meshes_count * sizeof(Mesh*)); // Start writing where Mesh data begins

    unsigned int amount_of_meshes_written = 0;

    engine_model_fill_from_node(scene, scene->mRootNode, current_write_position, model, &amount_of_meshes_written);
}

//==============================================================================//

void engine_model_fill_from_node(const struct aiScene* scene, const struct aiNode* node, void *write, Model* model, unsigned int *amount_of_meshes_written){
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        const struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        memcpy(model + sizeof(Model) + (*amount_of_meshes_written * sizeof(Mesh*)), &write, sizeof(Mesh*));
        engine_model_fill_mesh(mesh, write);
        amount_of_meshes_written++;
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++){
        engine_model_fill_from_node(scene, node->mChildren[i], write, model, amount_of_meshes_written);
    }
}

//==============================================================================//
// This is where the magic happens
void engine_model_fill_mesh(const struct aiMesh* mesh, void* write_position){
    Mesh* start_of_mesh = write_position;
    // Set amount of elements
    memcpy(write_position + offsetof(Mesh, vertex_count), &mesh->mNumVertices, sizeof(unsigned int));
    
    unsigned int index_amount = mesh->mNumFaces * 3;

    memcpy(write_position + offsetof(Mesh, index_count), &index_amount, sizeof(unsigned int));

    /*/--------------------------------------------------------------------------///
    |   Getting Vertices    VERIFIED                                               |
    ///--------------------------------------------------------------------------/*/
    write_position += sizeof(Mesh); // Now writing at vertex data start position 
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

    // Vertex v_array[mesh->mNumVertices];
    // memcpy(v_array, write_position, mesh->mNumVertices * sizeof(Vertex));
    // for (unsigned int i = 0; i < mesh->mNumVertices; i++){
    //     printf("Vertex Position %u {x, y, z}:\t%.2f\t%.2f\t%.2f\n", i + 1, v_array[i].position[0], v_array[i].position[1], v_array[i].position[2]);
    // }
    

    /*/--------------------------------------------------------------------------///
    |   Getting Indices                                                            |
    ///--------------------------------------------------------------------------/*/ 
    write_position += mesh->mNumVertices * sizeof(Vertex); // Now writing at index data start position VERIFIED
    for(unsigned int i = 0; i < mesh->mNumFaces - 1; i++){
        memcpy(write_position + (i * 3 * sizeof(unsigned int)), mesh->mFaces[i].mIndices, 3 * sizeof(unsigned int));
    }

    // unsigned int i_array[mesh->mNumFaces * 3];
    // memcpy(i_array, write_position, mesh->mNumFaces * 3 * sizeof(unsigned int));
    // for (unsigned int i = 0; i < mesh->mNumVertices; i++){
    //     printf("Index %u: %u\n", i + 1, i_array[i]);
    // }

    // engine_mesh_setup(start_of_mesh);

    // printf("Mesh VAO: %u\n", start_of_mesh->VAO);

    write_position += index_amount * sizeof(unsigned int); // This is where the next mesh can start to write

    return;
}