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

/**
 * @brief Models are built out of Meshes. All Mesh data is located directly after
 * Model members, so that all the data is in contiguous memory. Model houses the
 * amount of meshes, an array containing each Mesh's size in bytes, and the pointers
 * to each individual Mesh's data. Both meshes and meshes_size have a length of meshes_count. 
 */
typedef struct Model{
    unsigned int meshes_count;
} Model;

//==============================================================================//

void engine_model_draw(
    Model* model, 
    unsigned int shader
);

//==============================================================================//

/**
 * @brief Calculates the amount of bytes necessary to store all model and mesh
 * data of a given scene.
 * 
 * @param scene The Assimp scene to be stored in memory
 * @return Amount of bytes necessary to store model in memory 
 */
unsigned long engine_model_allocate_bytes(const struct aiScene* scene);

//==============================================================================//

void engine_model_recursive_scene_size(const struct aiNode* node, const struct aiScene* scene, unsigned long *va, unsigned long *ia, unsigned long* ma);

//==============================================================================//

Model* engine_model_make(const char* path);

//==============================================================================//

void engine_model_fill_data_from_scene(const struct aiScene* scene, Model* model);

//==============================================================================//

void engine_model_fill_from_node(const struct aiScene* scene, const struct aiNode* node, void *write, Model* model, unsigned int *amount_of_meshes_written);

//==============================================================================//

void engine_model_fill_mesh(const struct aiMesh* mesh, void* write_position);

//==============================================================================//

#endif