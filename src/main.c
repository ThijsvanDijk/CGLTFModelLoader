#include <stdlib.h>
#include <stdio.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <internal/camera.h>
#include <internal/shader.h>
#include <internal/model.h>

#include <assimp/cimport.h>


const GLuint WIDTH = 800, HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "[glad] GL with GLFW", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

    gladLoadGL(glfwGetProcAddress);

    glViewport(0, 0, WIDTH, HEIGHT);

    camera cam;
    engine_camera_default_values(&cam);
    mat4 view;
    engine_camera_view_matrix(&cam, view);
    GLuint program = engine_shader_make_program_from_files("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl");

    Model* model = engine_model_make("resources/models/crow/scene.gltf");

    // -------------------------------------------------------
    // float vertices[] = {
    //      0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    //     -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
    //      0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
    // };

    // GLuint VBO, VAO;
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    // glBindVertexArray(VAO);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    // glEnableVertexAttribArray(1);

    // -------------------------------------------------------

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.1f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glUseProgram(program);
        // glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        engine_model_draw(model, program);

        glfwSwapBuffers(window);
    }
    free(model);
    glfwTerminate();

    return 0;
}