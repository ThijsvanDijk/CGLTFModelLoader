// #include <stdlib.h>
// #include <stdio.h>

// #include <glad/gl.h>
// #include <GLFW/glfw3.h>
#include <cglm/cglm.h>


// const GLuint WIDTH = 800, HEIGHT = 600;

// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
//     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, GL_TRUE);
// }

int main(void) {
    // glfwInit();

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "[glad] GL with GLFW", NULL, NULL);
    // glfwMakeContextCurrent(window);

    // glfwSetKeyCallback(window, key_callback);

    // gladLoadGL(glfwGetProcAddress);

    // glViewport(0, 0, WIDTH, HEIGHT);
    vec3 v1 = {0.6f, 7.2f, 9.61f};
    vec3 v2 = {8.6f, 3.5f, 0.1f};
    vec3 v3;
    glm_vec3_cross(v1, v2, v3);

    // while (!glfwWindowShouldClose(window)) {
    //     glfwPollEvents();

    //     glClearColor(0.7f, 0.9f, 0.1f, 1.0f);
    //     glClear(GL_COLOR_BUFFER_BIT);

    //     glfwSwapBuffers(window);
    // }

    // glfwTerminate();

    return 0;
}