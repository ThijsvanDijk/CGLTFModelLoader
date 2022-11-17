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
#include <3D/model/model.h>
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

int main(void) {  
    clock_t time;  
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

    // OpenGL Setup
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, WIDTH, HEIGHT);

    // Camera setup
    camera cam;
    engine_camera_default_values(&cam);
    mat4 view;
    engine_camera_view_matrix(&cam, view);
    main_cam = &cam;
    GLuint program = engine_shader_make_program_from_files("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl");


    // Load GLTF Model  
    time = clock();  
    GLTFModel model;
    char c = gltf_modelLoad("resources/models/box/box.glb", &model);

    // Make scene from model
    Scene scene = makeSceneFromGLTF(&model);
    time = clock() - time;

    printf("Total time for loading model: %lf ms\n", (((double)time)/((double)CLOCKS_PER_SEC) * 1000.0f));

    unsigned int modelLoc = glGetUniformLocation(program, "model");
    unsigned int mvpLoc = glGetUniformLocation(program, "mvp");
    unsigned int viewPosLoc = glGetUniformLocation(program, "viewPos");
    glEnable(GL_DEPTH_TEST);

    // Main Loop
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

        mat4 modelMat = GLM_MAT4_IDENTITY_INIT;
        vec3 translate = GLM_VEC3_ZERO_INIT;
        vec3 scale = GLM_VEC3_ONE_INIT;

        glm_translate(modelMat, translate);
        glm_scale(modelMat, scale);
        
        mat4 mvp = GLM_MAT4_IDENTITY_INIT; 

        glm_mat4_mul(projection, view, mvp);
        glm_mat4_mul(mvp, modelMat, mvp);

        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (float*)mvp);
        glUniform3fv(viewPosLoc, 1, main_cam->position);

        drawScene(&scene);

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    free(scene.meshes);
    free(model.data);

    return 0;
}