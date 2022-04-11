// #include <stdlib.h>
// #include <stdio.h>

// #include <glad/gl.h>
// #include <GLFW/glfw3.h>


// const GLuint WIDTH = 800, HEIGHT = 600;

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
//     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, GL_TRUE);
// }

// int main(void) {
//     glfwInit();

//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//     GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "[glad] GL with GLFW", NULL, NULL);
//     glfwMakeContextCurrent(window);

//     glfwSetKeyCallback(window, key_callback);

//     gladLoadGL(glfwGetProcAddress);

//     glViewport(0, 0, WIDTH, HEIGHT);

//     while (!glfwWindowShouldClose(window)) {
//         glfwPollEvents();

//         glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT);

//         glfwSwapBuffers(window);
//     }

//     glfwTerminate();

//     return 0;
// }

#define DEBUG_SHADER

#include <stdio.h>
#include <internal/camera.h>
#include <internal/shader.h>

int main(){
    camera cam;
    engine_camera_default_values(&cam);
    mat4 view;
    engine_camera_view_matrix(&cam, view);
    printf("Camera Position: %.2f\t%.2f\t%.2f\n", cam.front[0], cam.front[1], cam.front[2]);
}