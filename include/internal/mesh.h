#ifndef MESH_H
#define MESH_H

#include <glad/gl.h>
#include <cglm/cglm.h>

#include <stdio.h>

//==============================================================================//
/**
 * @brief Defines the type of Texture. Four Options:
 * TEXTURE_DIFFUSE,
 * TEXTURE_SPECULAR,
 * TEXTURE_NORMAL,
 * TEXTURE_HEIGHT, 
 */
typedef enum TEXTURE_TYPE{
    TEXTURE_DIFFUSE,
    TEXTURE_SPECULAR,
    TEXTURE_NORMAL,
    TEXTURE_HEIGHT
} TEXTURE_TYPE;

//==============================================================================//

typedef struct Vertex{
    vec3 position;
    vec3 normal;
    vec2 tex_coords;
    vec3 tangent;
    vec3 bitangent;
} Vertex;

//==============================================================================//

typedef struct texture{
    unsigned int id;
    enum TEXTURE_TYPE type;
    char uniform_variable_name[20];
} Texture;

//==============================================================================//

typedef struct Mesh{
    Vertex * vertices;
    unsigned int  * indices;
    Texture* textures;
    unsigned int VAO, VBO, EBO, vertex_count, index_count, texture_count;
} Mesh;

//==============================================================================//

void engine_mesh_make(
    Vertex        *vertices, 
    unsigned int  *indices, 
    Texture       *textures, 
    unsigned int vertex_count, unsigned int index_count, unsigned int texture_count,
    Mesh          *result_store
);

//==============================================================================//

void engine_mesh_draw(Mesh* mesh, GLuint shader_id);

//==============================================================================//

void engine_mesh_setup(Mesh* mesh);

//==============================================================================//

#endif