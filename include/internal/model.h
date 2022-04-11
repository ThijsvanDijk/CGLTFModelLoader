#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <cglm/cglm.h>
#include <stb/stb_image.h>
#include <cgltf/cgltf.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <string.h>
#include <unistd.h>

#define PATH_MAX 1024

char cwd[PATH_MAX];

typedef struct Model{
    char directory[PATH_MAX];
    unsigned char gamma_correction : 1;
    unsigned int meshes_count;
    Mesh* meshes;
} Model;

unsigned char load_model(const struct aiScene* scene, Model* model);
void process_node_model(const struct aiNode* node, const struct aiScene* scene, Model* model);
void process_mesh_model(const struct aiMesh* mesh, const struct aiScene* scene, Mesh* store_mesh, const char* dir);
void load_material_textures_model(const struct aiMaterial* mat, enum aiTextureType type, TEXTURE_TYPE custom_type, Mesh* store_mesh, const char* dir, unsigned int* loaded_tex);
unsigned int texture_from_file(const char* path, const char* directory);
void app_model_free(Model* model);

unsigned char app_make_model(const char* path, Model* store){
    getcwd(cwd, sizeof(cwd));

    char total_path[MAXLEN];
    strcpy(total_path, cwd);
    strcat(total_path, path);

    char* end_of_directory = strrchr(total_path, '/'); 

    strncpy(store->directory, total_path, strlen(total_path) - strlen(end_of_directory));

    const struct aiScene* scene = aiImportFile(total_path, 
        aiProcess_CalcTangentSpace      |
        aiProcess_Triangulate           |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType
    );

    if(scene == NULL){
        printf("Failure loading model at: %s\n", total_path);
        return 0;
    }

    store->meshes_count = scene->mNumMeshes;

    load_model(scene, store);

    return 1;
}

void app_draw_model(Model* model, unsigned int shader){
    for(unsigned int i = 0; i < model->meshes_count; i++){
        app_draw_mesh(model->meshes + i * sizeof(Mesh), shader);
    }
}

unsigned char load_model(const struct aiScene* scene, Model* store){

    store->meshes = (Mesh*) malloc(store->meshes_count * sizeof(Mesh));

    process_node_model(scene->mRootNode, scene, store);

    aiReleaseImport(scene);
    return 1;
}

void process_node_model(const struct aiNode* node, const struct aiScene* scene, Model* store){
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        process_mesh_model(mesh, scene, &store->meshes[i], store->directory);
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++){
        process_node_model(node->mChildren[i], scene, store);
    }
}

void process_mesh_model(const struct aiMesh* mesh, const struct aiScene* scene, Mesh* store_mesh, const char* dir){
    /*
        LOAD VERTICES
    */    
    store_mesh->vertices = (Vertex*)malloc(mesh->mNumVertices * sizeof(Vertex));
    store_mesh->vertex_count = mesh->mNumVertices;
    // printf("Memory allocated for vertices: %lu\n", store_mesh->vertex_count * sizeof(Vertex));

    for(unsigned int i = 0; i < mesh->mNumVertices; i++){
        // Copy Position data to mesh
        memcpy(&store_mesh->vertices[i],                                 &mesh->mVertices[i],            sizeof(vec3));
        
        // Copy Normal data to mesh
        memcpy(&store_mesh->vertices[i] + offsetof(Vertex, normal),      &mesh->mNormals[i],             sizeof(vec3));

        // Copy Tex Coords to mesh
        memcpy(&store_mesh->vertices[i] + offsetof(Vertex, tex_coords),  &mesh->mTextureCoords[0][i],    sizeof(vec2));

        // Copy Tangents to mesh
        memcpy(&store_mesh->vertices[i] + offsetof(Vertex, tangent),     &mesh->mTangents[i],            sizeof(vec3));

        // Copy Bitangents to mesh
        memcpy(&store_mesh->vertices[i] + offsetof(Vertex, bitangent),   &mesh->mBitangents[i],          sizeof(vec3));
    }

    /*
        LOAD INDICES
    */
    store_mesh->index_count = 0;
    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        store_mesh->index_count += mesh->mFaces[i].mNumIndices;
    }
    store_mesh->indices = (unsigned int*)malloc(store_mesh->index_count * sizeof(unsigned int));
    // printf("Memory allocated for indices: %lu\n", store_mesh->index_count * sizeof(unsigned int));

    unsigned int n_of_indices_written = 0; 
    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        unsigned int indices_in_face = mesh->mFaces[i].mNumIndices;
        memcpy(&store_mesh->indices[n_of_indices_written], &mesh->mFaces[i].mIndices[n_of_indices_written], mesh->mFaces[i].mNumIndices * sizeof(unsigned int));
        n_of_indices_written += indices_in_face;
    }
    for (unsigned int i = 0; i < n_of_indices_written; i+=3){
        printf("%u, %u, %u\n", store_mesh->indices[i], store_mesh->indices[i+1], store_mesh->indices[i+2]);
    }
    

    /*
        LOAD MATERIALS
    */
    struct aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
    
    store_mesh->texture_count = aiGetMaterialTextureCount(mat, aiTextureType_DIFFUSE)  + 
                                aiGetMaterialTextureCount(mat, aiTextureType_SPECULAR) + 
                                aiGetMaterialTextureCount(mat, aiTextureType_HEIGHT)   + 
                                aiGetMaterialTextureCount(mat, aiTextureType_AMBIENT)  ;

    store_mesh->textures = (Texture*) malloc(store_mesh->texture_count * sizeof(Texture));
    printf("Number of textures to be loaded: %u\n", store_mesh->texture_count);
    printf("Memory allocated for textures:   %lu\n", store_mesh->texture_count * sizeof(Texture));


    unsigned int current_texture_nr = 0;
    load_material_textures_model(mat, aiTextureType_DIFFUSE, TEXTURE_DIFFUSE, store_mesh, dir, &current_texture_nr);
    load_material_textures_model(mat, aiTextureType_SPECULAR, TEXTURE_SPECULAR, store_mesh, dir, &current_texture_nr);
    load_material_textures_model(mat, aiTextureType_HEIGHT, TEXTURE_NORMAL, store_mesh, dir, &current_texture_nr);
    load_material_textures_model(mat, aiTextureType_AMBIENT, TEXTURE_HEIGHT, store_mesh, dir, &current_texture_nr);
    printf("Textures loaded: %d\n", current_texture_nr);
}

void load_material_textures_model(const struct aiMaterial* mat, enum aiTextureType type, TEXTURE_TYPE custom_type, Mesh* store_mesh, const char* dir, unsigned int* loaded_tex){

    unsigned int i;
    for (i = *loaded_tex; i < *loaded_tex + aiGetMaterialTextureCount(mat, type); i++){
        printf("Loaded_tex value: %u\n", *loaded_tex);
        // Get Path to Texture
        struct aiString string;
        aiGetMaterialTexture(mat, type, i, &string, NULL, NULL, NULL, NULL, NULL, NULL);
        
        printf("Path to a texture: %s\n", string.data);

        // Store Texture info in memory
        unsigned int texture_id = texture_from_file(string.data, dir);
        memcpy(&store_mesh->textures[i] + offsetof(Texture, id), &texture_id, sizeof(unsigned int));
        memcpy(&store_mesh->textures[i] + offsetof(Texture, type), &custom_type, sizeof(TEXTURE_TYPE));
    }
    *loaded_tex = i;
    printf("Current Texture Nr: %u\n", *loaded_tex);
}

unsigned int texture_from_file(const char* path, const char* directory){
    
    char combined_path[strlen(path) + strlen(directory) + 1];
    memcpy(combined_path, directory, strlen(directory));
    char c = '/';
    memcpy(combined_path + strlen(directory), &c, 1);
    memcpy(combined_path + strlen(directory) + 1, path, strlen(path));

    GLuint texture_id; 
    int width, height, nrComponents;
    glGenTextures(1, &texture_id);
    unsigned char *data = stbi_load(combined_path, &width, &height, &nrComponents, 0);

    if(data){
        GLenum format;
        if(nrComponents == 1)
            format = GL_RED;
        else if(nrComponents == 3)
            format = GL_RGB;
        else if(nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else{
        printf("Failed to load texture at path: %s\n", path);
        stbi_image_free(data);
    }
    return texture_id;
}

void app_model_free(Model* model){
    // Clean Up Meshes
    for(unsigned int i = 0; i < model->meshes_count; i++){
        // Vertex Cleanup
        free(model->meshes[i].vertices);

        // Index Cleanup
        free(model->meshes[i].indices);

        // Texture Cleanup
        free(model->meshes[i].textures);
    }
    // Free Mesh Reserved Memory 
    free(model->meshes); 
}

#endif