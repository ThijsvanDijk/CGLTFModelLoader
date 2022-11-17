#include <3D/model/model.h>

static const u8 accessorComponentTypeSizes[] = {
    1, 1, 2, 2, 0, 4, 4
};

static const u8 accessorTypeSizes[] = {
    1, 2, 3, 4, 4, 9, 16
};

u32 recursiveMeshCount(GLTFModel* model, u32 node){
    u32 meshCounter = 0;
    if(model->nodes[node].hasMesh) meshCounter++;
    for(u32 i = 0; i < model->nodes[node].childrenLength; i++){
        meshCounter += recursiveMeshCount(model, model->nodes[node].children[i]);
    }
    return meshCounter;
}

u32 getMeshCountFromModel(GLTFModel* model, u32 rootScene){
    u32 meshCounter = 0;
    for(u32 i = 0; i < model->scenes[rootScene].nodesLength; i++){
        meshCounter += recursiveMeshCount(model, model->scenes[rootScene].nodes[i]);
    }
    return meshCounter;    
}

u32 recursiveMeshMake(GLTFModel* model, u32 node, u32 currentMesh, Scene* scene){
    if(model->nodes[node].hasMesh && (currentMesh < scene->meshCount)){
        GLTFMesh* mesh = &model->meshes[model->nodes[node].mesh];        
        GLTFAccessor* positionAccessor = &model->accessors[mesh->primitives[0].attributes.position];
        GLTFAccessor* normalAccessor = &model->accessors[mesh->primitives[0].attributes.normal];
        GLTFAccessor* indicesAccessor = &model->accessors[mesh->primitives[0].indices];
        GLTFBufferView* vertexBufferView = &model->bufferViews[positionAccessor->bufferView];
        GLTFBufferView* indicesBufferView = &model->bufferViews[indicesAccessor->bufferView];
        GLTFBuffer* vertexBuffer = &model->buffers[vertexBufferView->buffer];
        GLTFBuffer* indicesBuffer = &model->buffers[indicesBufferView->buffer];

        if(positionAccessor->bufferView != normalAccessor->bufferView){
            printf("Position and indices incompatible with loader\n");
            return 0;
        }

        u32 VertexBufferObject, ElementBufferObject;
        glGenVertexArrays(1, &scene->meshes[currentMesh].VAO);
        glGenBuffers(1, &VertexBufferObject);
        glGenBuffers(1, &ElementBufferObject);
        glBindVertexArray(scene->meshes[currentMesh].VAO);

        glBindBuffer(vertexBufferView->target, VertexBufferObject);
        glBufferData(vertexBufferView->target, vertexBufferView->byteLength, (void*)(model->binDataStart + vertexBufferView->byteOffset), GL_STATIC_DRAW);

        glBindBuffer(indicesBufferView->target, ElementBufferObject);
        glBufferData(indicesBufferView->target, indicesBufferView->byteLength, (void*)(model->binDataStart + indicesBufferView->byteOffset), GL_STATIC_DRAW);
        
        glVertexAttribPointer(
            0, 
            accessorTypeSizes[positionAccessor->type], 
            positionAccessor->componentType + 5120, 
            positionAccessor->normalized, 
            vertexBufferView->byteStride, 
            (void*)positionAccessor->byteOffset
        );
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            1, 
            accessorTypeSizes[normalAccessor->type], 
            normalAccessor->componentType + 5120, 
            normalAccessor->normalized, 
            vertexBufferView->byteStride, 
            (void*)normalAccessor->byteOffset
        );
        glEnableVertexAttribArray(1);

        scene->meshes[currentMesh].drawMode = mesh->primitives[0].mode;
        scene->meshes[currentMesh].elementCount = indicesAccessor->count;
        scene->meshes[currentMesh].elementType = indicesAccessor->componentType + 5120;

        currentMesh++;
    }

    for(u32 i = 0; i < model->nodes[node].childrenLength; i++){
        currentMesh = recursiveMeshMake(model, model->nodes[node].children[i], currentMesh, scene);
    }

    return currentMesh;
}

void fillScene(GLTFModel* model, u32 rootScene, Scene* scene){
    unsigned int currentMesh = 0;
    for(u32 i = 0; i < model->scenes[rootScene].nodesLength; i++){
        currentMesh = recursiveMeshMake(model, model->scenes[rootScene].nodes[i], currentMesh, scene);
    }
    glBindVertexArray(0);
}

Scene makeSceneFromGLTF(GLTFModel* model){
    u32 rootScene = 0;
    u32 meshesToDraw = 0;
    if(model->hasScene && model->scenesCount > 0) rootScene = model->scene;
    else{
        Scene s = {.meshCount = 0, .meshes = NULL};
        return s;
    }

    meshesToDraw = getMeshCountFromModel(model, rootScene);

    Scene scene= {
        .meshCount = meshesToDraw,
        .meshes = malloc(meshesToDraw * sizeof(IndexedMesh))
    };

    fillScene(model, rootScene, &scene);
    
    return scene;
}

void drawScene(Scene* scene){
    for(u32 i = 0; i < scene->meshCount; i++){
        glBindVertexArray(scene->meshes[i].VAO);
        glDrawElements(scene->meshes[i].drawMode, scene->meshes[i].elementCount, scene->meshes[i].elementType, (void *)0);
    }
    glBindVertexArray(0);
}