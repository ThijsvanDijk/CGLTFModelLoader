#ifndef MESH_H
#define MESH_H

#include <glad/gl.h>
#include <cglm/cglm.h>

#include <stdio.h>

typedef enum TEXTURE_TYPE{
    TEXTURE_DIFFUSE,
    TEXTURE_SPECULAR,
    TEXTURE_NORMAL,
    TEXTURE_HEIGHT
} TEXTURE_TYPE;

typedef struct Vertex{
    vec3 position;
    vec3 normal;
    vec2 tex_coords;
    vec3 tangent;
    vec3 bitangent;
} Vertex;

typedef struct Texture{
    unsigned int id;
    enum TEXTURE_TYPE type;
} Texture;

typedef struct Mesh{
    Vertex * vertices;
    unsigned int  * indices;
    Texture* textures;
    unsigned int VAO, VBO, EBO, vertex_count, index_count, texture_count;
} Mesh;

void app_setup_mesh(Mesh* mesh);

void app_make_mesh(
    Vertex        *vertices, 
    unsigned int  *indices, 
    Texture       *textures, 
    unsigned int vertex_count, unsigned int index_count, unsigned int texture_count,
    Mesh* result_store
){
    result_store->vertices = vertices;
    result_store->indices = indices;
    result_store->textures = textures;
    result_store->vertex_count = vertex_count;
    result_store->index_count = index_count;
    result_store->texture_count = texture_count;
    app_setup_mesh(result_store);
}

void app_draw_mesh(Mesh* mesh, GLuint shader_id){
    // Texture Uniform naming should be moved to separate function to improve performance
    // No need to do this every frame, just store its uniform shader name in the texture struct
    unsigned int diffuseNr = 1, specularNr = 1, normalNr = 1, heightNr = 1;
    for(unsigned int i = 0; i < mesh->texture_count; i++){
        char variable_name[21] = "texture_";
        glActiveTexture(GL_TEXTURE0 + i);
        TEXTURE_TYPE type = mesh->textures[i].type; 
        
        if(type == TEXTURE_DIFFUSE && diffuseNr < 10000)
            sprintf(&variable_name[8], "diffuse%d", diffuseNr++);
        else if(type == TEXTURE_SPECULAR && specularNr < 10000)
            sprintf(&variable_name[8], "specular%d", specularNr++);
        else if(type == TEXTURE_NORMAL && normalNr < 10000)
            sprintf(&variable_name[8], "normal%d", normalNr++);
        else if(type == TEXTURE_HEIGHT && heightNr < 10000)
            sprintf(&variable_name[8], "height%d", heightNr++);

        glUniform1i(glGetUniformLocation(shader_id, variable_name), i);
        glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
    }

    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0); 
}



void app_setup_mesh(Mesh* mesh){
    // Buffer Generation
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    // Setup Buffers
    glBindVertexArray(mesh->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertex_count * sizeof(Vertex), mesh->vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->index_count * sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);

    // Set Vertex Attribute Pointers
    //      Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); 
    
    //      Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    //      Tex Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    //      Tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    //      Bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));    

    // Unbind VAO
    glBindVertexArray(0);
}

#endif