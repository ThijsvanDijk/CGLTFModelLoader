#include <vec3.h>

void vec3_add(float* a, float* b){
    a[0] += b[0];
    a[1] += b[1];
    a[2] += b[2];
    return;
}

void vec3_sub(float* a, float* b){
    a[0] -= b[0];
    a[1] -= b[1];
    a[2] -= b[2];
    return;
}