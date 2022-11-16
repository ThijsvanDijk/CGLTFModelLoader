// Standard Libs
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

// 3rd Party Libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

// Custom Libs
#include <3D/camera/camera.h>
#include <3D/model/gltf/gltf.h>
#include <3D/shader/shader.h>

#define WIDTH 1920
#define HEIGHT 1080

#define NOFITERATIONS 1

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 0.5f * WIDTH, lastY = 0.5f * HEIGHT;
int firstMouse = 1;
int jump = 0, scaler = 0;
float jumpStartTime, jumpEndTime, scalerStartTime, scalerEndTime;
camera * main_cam;

static const u8 accessorComponentTypeSizes[] = {
    1, 1, 2, 2, 0, 4, 4
};

static const u8 accessorTypeSizes[] = {
    1, 2, 3, 4, 4, 9, 16
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = 0;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    engine_camera_process_mouse_movement(main_cam, xoffset, yoffset, 1);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    engine_camera_process_mouse_scroll(main_cam, yoffset);
}

void process_input(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        engine_camera_process_keyboard(main_cam, FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        engine_camera_process_keyboard(main_cam, BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        engine_camera_process_keyboard(main_cam, LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        engine_camera_process_keyboard(main_cam, RIGHT, deltaTime);
}

void print_model(GLTFModel* model);

int main(void) {
    clock_t start_t, end_t;
    double total_t;
    
    //----------GLFW Setup----------    
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "CGLTFModelLoader", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //----------OpenGL Setup----------
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glViewport(0, 0, WIDTH, HEIGHT);

    camera cam;
    engine_camera_default_values(&cam);
    mat4 view;
    engine_camera_view_matrix(&cam, view);
    main_cam = &cam;
    GLuint program = engine_shader_make_program_from_files("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl");

    //----------Get GLTF Model----------//
    start_t = clock();

    for(uint64_t i = 0; i < NOFITERATIONS; i++){
        GLTFModel model;
        char c = gltf_modelLoad("resources/models/box/box.glb", &model);
        free(model.data);
    }
    end_t = clock();

    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC; 
    printf("Loading GLB Model (%u iterations) info took on average %f microseconds per iteration\n", NOFITERATIONS, (total_t / (double)NOFITERATIONS) * 1000000);
    
    GLTFModel model;
    char c = gltf_modelLoad("resources/models/box/box.glb", &model);

    print_model(&model);

    //----------Getting a test mesh to show----------
    GLTFMesh mesh = model.meshes[0];
    GLTFAccessor positionAccessor = model.accessors[mesh.primitives[0].attributes.position];
    GLTFAccessor normalAccessor = model.accessors[mesh.primitives[0].attributes.normal];
    GLTFAccessor indicesAccessor = model.accessors[mesh.primitives[0].indices];
    GLTFBufferView vertexBufferView = model.bufferViews[positionAccessor.bufferView];
    GLTFBufferView indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
    GLTFBuffer buffer = model.buffers[0];

    vec3* vertices = (vec3*)model.binDataStart;
    for(u16 i = 0; i < positionAccessor.count; i++){
        printf("Vec3 %hu:  [%.2f, %.2f, %.2f]\n", i, vertices[i][0], vertices[i][1], vertices[i][2]);
    }

    u16* indices = (u16*)(model.binDataStart + indicesBufferView.byteOffset);
    printf("Indices bufferView ByteOffset: %lu\n", indicesBufferView.byteOffset);
    for(u16 i = 0; i < indicesAccessor.count; i++){
        printf("Index %hu: %hu\n", i, indices[i]);
    }

    // //----------Vertex Array Setup----------
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(vertexBufferView.target, VBO);
    glBufferData(vertexBufferView.target, vertexBufferView.byteLength, (void*)(model.binDataStart + vertexBufferView.byteOffset), GL_STATIC_DRAW);

    glBindBuffer(indicesBufferView.target, EBO);
    glBufferData(indicesBufferView.target, indicesBufferView.byteLength, (void*)(model.binDataStart + indicesBufferView.byteOffset), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, accessorTypeSizes[positionAccessor.type], positionAccessor.componentType + 5120, positionAccessor.normalized, vertexBufferView.byteStride, (void*)positionAccessor.byteOffset);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, accessorTypeSizes[normalAccessor.type], normalAccessor.componentType + 5120, normalAccessor.normalized, vertexBufferView.byteStride, (void*)normalAccessor.byteOffset);
    glEnableVertexAttribArray(1);

    vec3 scale = {1.0f, 1.0f, 1.0f};

    unsigned int mvpLoc = glGetUniformLocation(program, "mvp");
    unsigned int viewPosLoc = glGetUniformLocation(program, "viewPos");

    // // -------------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_input(window);

        glClearColor(0.1f, 0.6f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program);

        engine_camera_update_vectors(main_cam);
        mat4 projection;
        glm_perspective(glm_rad(main_cam->zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f, projection);
        mat4 view = GLM_MAT4_IDENTITY_INIT;
        engine_camera_view_matrix(main_cam, view);

        mat4 modelMat;
        memcpy(modelMat, model.nodes[0].matrix, sizeof(mat4));
        vec3 translate = GLM_VEC3_ZERO_INIT;

        glm_translate(modelMat, translate);
        glm_scale(modelMat, scale);
        
        mat4 mvp = GLM_MAT4_IDENTITY_INIT; 

        glm_mat4_mul(projection, view, mvp);
        glm_mat4_mul(mvp, modelMat, mvp);

        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (float*)mvp);
        glUniform3fv(viewPosLoc, 1, main_cam->position);

        glBindVertexArray(VAO);
        glDrawElements(mesh.primitives[0].mode, indicesAccessor.count, indicesAccessor.componentType + 5120, (void *)0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    free(model.data);

    return 0;
}

void print_model(GLTFModel* model){
    printf("========================================\nGLTF Model Info\nTotal Size in memory: %lu bytes\n", sizeof(GLTFModel) + model->dataLength);

    // Asset
    printf("----------------------------------------\n");
                                            printf("Asset:\n\tVersion:     %hhu.%hhu\n", model->asset.versionMajor, model->asset.versionMinor);
    if(model->asset.hasMinVersion)          printf("\tMin Version: %hhu.%hhu\n", model->asset.minVersionMajor, model->asset.minVersionMinor);
    if(model->asset.copyrightLength > 0)    printf("\tCopyright:   %s\n", model->asset.copyright);
    if(model->asset.generatorLength > 0)    printf("\tGenerator:   %s\n", model->asset.generator);

    // Scenes
    printf("----------------------------------------\n");

    for(u32 i = 0; i < model->scenesCount; i++){
        printf("Scene %u:\n", i);
        if(model->scenes[i].nodesLength > 0){
            printf("\tNodes: [");
            for(u32 j = 0; j < model->scenes[i].nodesLength; j++){
                printf("%u", model->scenes[i].nodes[j]);
                if(j != model->scenes[i].nodesLength - 1) printf(", ");
            }
            printf("]\n");
        }
        if(model->scenes[i].nameLength > 0) printf("\tName:  %s\n", model->scenes[i].name);        
    }

    // Nodes
    printf("----------------------------------------\n");
    for(u32 i = 0; i < model->nodesCount; i++){
        printf("Node %u:\n", i);
        
        // Children
        printf("\tChildren(%u): [", model->nodes[i].childrenLength);
        for(u32 j = 0; j < model->nodes[i].childrenLength; j++){
            printf("%u", model->nodes[i].children[j]);
            if(j != model->nodes[i].childrenLength - 1) printf(", ");
        }
        printf("]\n");

        // Name
        if(model->nodes[i].nameLength > 0) 
            printf("\tName:   %s\n", model->nodes[i].name);
        
        // Mesh
        if(model->nodes[i].hasMesh) 
            printf("\tMesh:   %u\n", model->nodes[i].mesh);

        // Camera
        if(model->nodes[i].hasCamera) 
            printf("\tCamera: %u\n", model->nodes[i].camera);
        
        // Translation
        printf("\tTranslation: [%.2f, %.2f, %.2f]\n", model->nodes[i].translation[0], model->nodes[i].translation[1], model->nodes[i].translation[2]);
        
        // Rotation
        printf("\tRotation: [%.2f, %.2f, %.2f, %.2f]\n", model->nodes[i].rotation[0], model->nodes[i].rotation[1], model->nodes[i].rotation[2], model->nodes[i].rotation[3]);
        
        // Scale
        printf("\tScale: [%.2f, %.2f, %.2f]\n", model->nodes[i].scale[0], model->nodes[i].scale[1], model->nodes[i].scale[2]);

        // Matrix
        vec4* matrix = model->nodes[i].matrix;
        printf("\tMatrix:   | %0.2f %0.2f %0.2f %0.2f |\n", matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0]);
        printf("\t          | %0.2f %0.2f %0.2f %0.2f |\n", matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1]);
        printf("\t          | %0.2f %0.2f %0.2f %0.2f |\n", matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2]);
        printf("\t          | %0.2f %0.2f %0.2f %0.2f |\n", matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]);

        // Weights
        printf("\tWeights(%u): [", model->nodes[i].weightsLength);
        for(u32 j = 0; j < model->nodes[i].weightsLength; j++){
            printf("%.2f", model->nodes[i].weights[j]);
            if(j != model->nodes[i].weightsLength - 1) printf(", ");
        }
        printf("]\n");
    }

    // Meshes
    printf("----------------------------------------\n");
    for(u32 i = 0; i < model->meshesCount; i++){
        printf("Mesh %u:\n", i);
        if(model->meshes[i].nameLength > 0) printf("\tName: %s\n", model->meshes[i].name);

        // Weights
        printf("\tWeights: [");
        for(u32 j = 0; j < model->meshes[i].weightsLength; j++){
            printf("%.2f", model->meshes[i].weights[j]);
            if(j != model->meshes[i].weightsLength - 1) printf(", ");
        }
        printf("]\n");

        // Primitives
        printf("\tPrimitives:\n");
        for(u32 j = 0; j < model->meshes[i].primitivesLength; j++){
            if(model->meshes[i].primitives[j].hasIndices) printf("\t\tIndices: %u\n", model->meshes[i].primitives[j].indices);
            if(model->meshes[i].primitives[j].hasMaterial) printf("\t\tMaterial: %u\n", model->meshes[i].primitives[j].material);
            printf("\t\tMode: %u\n", model->meshes[i].primitives[j].mode);
            printf("\t\tAttributes:\n");
            if(model->meshes[i].primitives[j].attributes.hasPosition) printf("\t\t\tPOSITION: %u\n", model->meshes[i].primitives[j].attributes.position);
            if(model->meshes[i].primitives[j].attributes.hasNormal) printf("\t\t\tNORMAL: %u\n", model->meshes[i].primitives[j].attributes.normal);
            if(model->meshes[i].primitives[j].attributes.hasTangent) printf("\t\t\tTANGENT: %u\n", model->meshes[i].primitives[j].attributes.tangent);
            if(model->meshes[i].primitives[j].attributes.hasTexcoord0) printf("\t\t\tTEXCOORD_0: %u\n", model->meshes[i].primitives[j].attributes.texcoord0);
            if(model->meshes[i].primitives[j].attributes.hasTexcoord1) printf("\t\t\tTEXCOORD_1: %u\n", model->meshes[i].primitives[j].attributes.texcoord1);
            if(model->meshes[i].primitives[j].attributes.hasColor) printf("\t\t\tCOLOR_0: %u\n", model->meshes[i].primitives[j].attributes.color);
            if(model->meshes[i].primitives[j].attributes.hasJoints) printf("\t\t\tJOINTS_0: %u\n", model->meshes[i].primitives[j].attributes.joints);
            if(model->meshes[i].primitives[j].attributes.hasWeights) printf("\t\t\tWEIGHTS_0: %u\n", model->meshes[i].primitives[j].attributes.weights);
        }
        printf("]\n");
    }

    // Accessors
    printf("----------------------------------------\n");
    for(u32 i = 0; i < model->accessorsCount; i++){
        printf("Accessor %u:\n", i);
        printf("\tComponentType: %u\n", model->accessors[i].componentType);
        printf("\tCount:         %u\n", model->accessors[i].count);
        printf("\tType:          %u\n", model->accessors[i].type);
        if(model->accessors[i].hasBufferView) printf("\tBufferView:    %u\n", model->accessors[i].bufferView);
        printf("\tByteOffset:    %lu\n", model->accessors[i].byteOffset);
        printf("\tNormalized:    %hhu\n", model->accessors[i].normalized);
        if(model->accessors[i].nameLength > 0) printf("\tName:          %s\n", model->accessors[i].name);
        if(model->accessors[i].hasSparse){
            printf("\tSparse:\n");
            printf("\t\tCount: %lu\n", model->accessors[i].sparse.count);
            printf("\t\tIndices:\n");
            printf("\t\t\tBufferView: %u\n", model->accessors[i].sparse.indices.bufferView);
            printf("\t\t\tByteOffset: %lu\n", model->accessors[i].sparse.indices.byteOffset);
            printf("\t\t\tComponentType: %u\n", model->accessors[i].sparse.indices.componentType);
            printf("\t\tValues:\n");
            printf("\t\t\tBufferView: %u\n", model->accessors[i].sparse.values.bufferView);
            printf("\t\t\tByteOffset: %lu\n", model->accessors[i].sparse.values.byteOffset);
        }
    }

    // Buffer Views
    printf("----------------------------------------\n");
    for(u32 i = 0; i < model->bufferViewsCount; i++){
        printf("Buffer View %u:\n", i);
        printf("\tBuffer:      %u\n", model->bufferViews[i].buffer);
        printf("\tByte Offset: %lu\n", model->bufferViews[i].byteOffset);
        printf("\tByte Length: %lu\n", model->bufferViews[i].byteLength);
        printf("\tByte Stride: %hhu\n", model->bufferViews[i].byteStride);
        if(model->bufferViews[i].hasTarget) printf("\tTarget:      %u\n", model->bufferViews[i].target);
        if(model->bufferViews[i].nameLength > 0) printf("\tName:        %s\n", model->bufferViews[i].name);
    }

    // Buffers
    printf("----------------------------------------\n");

    for(u32 i = 0; i < model->buffersCount; i++){
        printf("Buffer %u:\n", i);
        printf("\tByteLength: %lu\n", model->buffers[i].byteLength);
        if(model->buffers[i].nameLength > 0) printf("\tName:       %s", model->buffers[i].name);
        if(model->buffers[i].uriLength > 0) printf("\tUri:        %s", model->buffers[i].uri);       
    }

    printf("========================================\n");
}