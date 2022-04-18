#include <internal/mesh.h>

// void engine_mesh_make(
//     Vertex        *vertices, 
//     unsigned int  *indices, 
//     Texture       *textures, 
//     unsigned int vertex_count, unsigned int index_count, unsigned int texture_count,
//     Mesh* result_store
// ){
//     result_store->vertices = vertices;
//     result_store->indices = indices;
//     result_store->vertex_count = vertex_count;
//     result_store->index_count = index_count;
//     engine_mesh_setup(result_store);
// }

void engine_mesh_draw(Mesh* mesh, GLuint shader_id){
    // Texture Uniform naming should be moved to separate function to improve performance
    // No need to do this every frame, just store its uniform shader name in the texture struct
    // unsigned int diffuseNr = 1, specularNr = 1, normalNr = 1, heightNr = 1;
    // for(unsigned int i = 0; i < mesh->texture_count; i++){
    //     char variable_name[21] = "texture_";
    //     glActiveTexture(GL_TEXTURE0 + i);
    //     TEXTURE_TYPE type = mesh->textures[i].type; 
        
    //     if(type == TEXTURE_DIFFUSE && diffuseNr < 10000)
    //         sprintf(&variable_name[8], "diffuse%d", diffuseNr++);
    //     else if(type == TEXTURE_SPECULAR && specularNr < 10000)
    //         sprintf(&variable_name[8], "specular%d", specularNr++);
    //     else if(type == TEXTURE_NORMAL && normalNr < 10000)
    //         sprintf(&variable_name[8], "normal%d", normalNr++);
    //     else if(type == TEXTURE_HEIGHT && heightNr < 10000)
    //         sprintf(&variable_name[8], "height%d", heightNr++);

    //     glUniform1i(glGetUniformLocation(shader_id, variable_name), i);
    //     glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
    // }

    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
    // printf("Amount of indices drawn: %u\n", mesh->index_count);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0); 
}



void engine_mesh_setup(Mesh* mesh){
    // Buffer Generation
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    // Setup Buffers
    glBindVertexArray(mesh->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertex_count * sizeof(Vertex), mesh + sizeof(Mesh), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->index_count * sizeof(unsigned int), mesh + sizeof(Mesh) + mesh->vertex_count * sizeof(Vertex), GL_STATIC_DRAW);

    printf("Amount of vertices: %u\nAmount of indices: %u", mesh->vertex_count, mesh->index_count);

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