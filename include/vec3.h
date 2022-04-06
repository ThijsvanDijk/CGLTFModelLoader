typedef float vec3[3];

/**
 * @brief 
 * Adds 2 vectors with length 3 and stores
 * result in a. This function always assumes
 * that both a and b point to float array of 
 * size 3.
 * 
 * @param {float*} a - First input vector (result is stored here)
 * @param {float*} b - Second input vector
 */
void vec3_add(float* a, float* b);

/**
 * @brief 
 * Subtracts 2 vectors with length 3 and stores
 * result in a. This function always assumes
 * that both a and b point to float array of 
 * size 3.
 * 
 * @param {float*} a - First input vector (result is stored here)
 * @param {float*} b - Second input vector
 */
void vec3_sub(float* a, float* b);

/**
 * @brief 
 * Adds 2 vectors with length 3 and stores
 * result in 'result'. This function always assumes
 * that a, b and result point to float array of 
 * size 3.
 * 
 * @param {float*} a - First input vector
 * @param {float*} b - Second input vector
 * @param {float*} result - Resulting vector is stored here
 */
void vec3_add_store(float* a, float* b, float* result);

/**
 * @brief 
 * Subtracts 2 vectors with length 3 and stores
 * result in 'result'. This function always assumes
 * that a, b and result point to float array of 
 * size 3.
 * 
 * @param {float*} a - First input vector
 * @param {float*} b - Second input vector
 * @param {float*} result - Resulting vector is stored here
 */
void vec3_sub_store(float* a, float* b, float* result);

void vec3_scale(float* vector, float scalar);
void vec3_scale_to(float* vector, float scalar, float* result);