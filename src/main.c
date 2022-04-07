#include <stdio.h>
#include <vec3.h>
#include <GLFW/glfw3.h>

int main(){
    glfwInit();    
    vec3 a = {6.22, 7.21, 0.84};
    vec3 b = {4.76, 9.19, 5.38};
    vec3 result;

    vec3_add_store(a, b, result);

    printf("Result = {%.2f, %.2f, %.2f}\n", result[0], result[1], result[2]);
    return 0;
}