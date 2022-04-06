#pragma GCC optimize("O3")
#pragma GCC target("avx2")
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

void vec3_add_store(float* a, float* b, float* result){
    result[0] = a[0] + b[0];
    result[1] = a[1] + b[1];
    result[2] = a[2] + b[2];
    return;
}