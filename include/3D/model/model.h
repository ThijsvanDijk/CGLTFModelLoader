#include <3D/model/gltf/types.h>
#include <glad/glad.h>

typedef struct{
    u32 VAO;
    u32 drawMode;
    u32 elementCount;
    u32 elementType;
} IndexedMesh;

typedef struct{
    IndexedMesh* meshes;
    u32 meshCount;
} Scene;

/**
 * @brief Uses GLTF Data to construct the necessary buffers in OpenGL for drawing the scene
 * 
 * @param model The GLTF Model that needs to be loaded into OpenGL
 * @return A pointer to a new Scene object with the loaded meshes, or NULL if it failed
 */
Scene makeSceneFromGLTF(GLTFModel* model);

/**
 * @brief Draws all meshes, is not optimised so does one draw call per mesh
 * 
 * @param scene The scene to draw
 */
void drawScene(Scene* scene);