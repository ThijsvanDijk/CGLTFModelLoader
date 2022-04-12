#ifndef MODEL_H
#define MODEL_H

#include <glad/gl.h>

#include <cglm/cglm.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb/stb_image.h>

#include <internal/mesh.h>
#include <string.h>
#include <unistd.h>

//==============================================================================//

// #define MAX_LEN 1024
// char cwd[MAX_LEN];

//==============================================================================//

typedef struct Model{
    unsigned int meshes_count;
    Mesh meshes[];
} Model;

//==============================================================================//

unsigned long engine_model_allocate_bytes(const struct aiScene* scene);

//==============================================================================//

unsigned char engine_model_make(
    const char* path, 
    Model* store
);

//==============================================================================//

void engine_model_draw(
    Model* model, 
    unsigned int shader
);

//==============================================================================//

unsigned char engine_model_load(
    const struct aiScene* scene, 
    Model* store
);

//==============================================================================//

void engine_model_process_node(
    const struct aiNode* node, 
    const struct aiScene* scene, 
    Model* store
);

//==============================================================================//

void engine_model_process_mesh(
    const struct aiMesh* mesh, 
    const struct aiScene* scene, 
    Mesh* store_mesh, 
    const char* dir
);

//==============================================================================//

void engine_model_load_material(
    const struct aiMaterial* mat, 
    enum aiTextureType type, 
    TEXTURE_TYPE custom_type, 
    Mesh* store_mesh, 
    const char* dir, 
    unsigned int* loaded_tex
);

//==============================================================================//

unsigned int engine_texture_from_file(
    const char* path, 
    const char* directory
);

//==============================================================================//

/**
 * @brief Deallocates all heap allocated memory from a model
 * 
 * @param[in] model Model that needs to be deallocated 
 */
void engine_model_free(Model* model);

//==============================================================================//

#endif