#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include <3D/camera/camera.h>
#include <3D/model/gltfLoader.h>
#include <3D/shader/shader.h>


#define WIDTH 1920
#define HEIGHT 1080

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
    clock_t t;
    double cpu_time;
    
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
    printf("Cool -1");
    //----------OpenGL Setup----------
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glViewport(0, 0, WIDTH, HEIGHT);

    camera cam;
    engine_camera_default_values(&cam);
    mat4 view;
    engine_camera_view_matrix(&cam, view);
    main_cam = &cam;
    printf("Cool");
    GLuint program = engine_shader_make_program_from_files("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl");
    printf("Cool 1");
    //----------Get GLTF Model----------
    BufferInfo buffer_data;   
    char code = getGLTFModelInfo("resources/models/crow/scene.gltf", &buffer_data);
    if(code < 0){
        fprintf(stderr, "Model failed to load, quitting...\n");
        return -1;
    }
    printf("Cool2");

    Vertex vertices[buffer_data.vertex_count];
    unsigned int indices[buffer_data.index_count];
    Mesh mesh = {vertices, indices};

    t = clock(); 
    code = loadGLTFBinaryData(&buffer_data, &mesh);
    t = clock() - t;
    cpu_time = ((double)t) / CLOCKS_PER_SEC; 
    printf("Loading GLTF bin data took: %f ms\n", cpu_time * 1000);
    if(code < 0){
        fprintf(stderr, "Binary data loading failed, quitting...\n");
        return -1;
    }
    printf("Cool 4");

    //----------Vertex Array Setup----------
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    printf("Cool 5");

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, buffer_data.vertex_position_type, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, buffer_data.vertex_normal_type, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    printf("Cool 6");

    vec3 scale = {1.0f, 1.0f, 1.0f};

    unsigned int mvpLoc = glGetUniformLocation(program, "mvp");
    unsigned int viewPosLoc = glGetUniformLocation(program, "viewPos");

    // -------------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_input(window);

        glClearColor(0.2f, 0.1f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program);

        engine_camera_update_vectors(main_cam);
        mat4 projection;
        glm_perspective(glm_rad(main_cam->zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f, projection);
        mat4 view = GLM_MAT4_IDENTITY_INIT;
        engine_camera_view_matrix(main_cam, view);

        mat4 model = GLM_MAT4_IDENTITY_INIT;
        vec3 translate = GLM_VEC3_ZERO_INIT;

        glm_translate(model, translate);
        glm_scale(model, scale);
        
        mat4 mvp = GLM_MAT4_IDENTITY_INIT; 

        glm_mat4_mul(projection, view, mvp);
        glm_mat4_mul(mvp, model, mvp);

        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (float*)mvp);
        glUniform3fv(viewPosLoc, 1, main_cam->position);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, buffer_data.index_count, buffer_data.index_type, (void *)0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    glfwTerminate();

    return 0;
}