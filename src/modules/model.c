#include <internal/model.h>

unsigned long engine_model_allocate_bytes(const struct aiScene* scene){
    unsigned long va = 0, ia = 0, ta = 0;
    
    engine_model_recursive_scene_size(scene->mRootNode, scene, &va, &ia);
    
    return (va * sizeof(Vertex) + ia * sizeof(unsigned int));       
}

void engine_model_recursive_scene_size(struct aiNode* node, struct aiScene* scene, unsigned long *va, unsigned long *ia){
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        struct aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        *va += mesh->mNumVertices;
        *ia += mesh->mNumFaces;
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++){
        engine_model_recursive_scene_size(node->mChildren[i], scene, va, ia);
    }
}

unsigned char engine_model_make(const char* path, Model* store){
    
    const struct aiScene* scene = aiImportFile(path, 
        aiProcess_CalcTangentSpace      |
        aiProcess_Triangulate           |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType
    );

    unsigned long number_of_bytes = engine_model_allocate_bytes(scene);
    printf("Number of bytes to be allocated: %lu\n", number_of_bytes);

    if(scene == NULL){
        printf("Failure loading model at: %s\n", path);
        return -1;
    }

    return 0;
}

