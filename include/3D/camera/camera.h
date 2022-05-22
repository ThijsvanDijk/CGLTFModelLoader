#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include <string.h>

//==============================================================================//

/**
 * @brief Has the four directions for camera movement:
 * {FORWARD, BACKWARD, LEFT, RIGHT}
 */
typedef enum MOVEMENT_DIRECTION {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} MOVEMENT_DIRECTION;

//==============================================================================//

/*
    Default Camera Values
*/
extern const float CAMERA_DEFAULT_YAW;
extern const float CAMERA_DEFAULT_PITCH;
extern const float CAMERA_DEFAULT_SPEED;
extern const float CAMERA_DEFAULT_SENSITIVITY; 
extern const float CAMERA_DEFAULT_ZOOM;

//==============================================================================//

/**
 * @brief Camera struct that defines players viewport 
 */
typedef struct camera{
    vec3  position;
    vec3  front;
    vec3  up;
    vec3  right;
    vec3  world_up;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float zoom;
} camera;
 
//==============================================================================//

/**
 * @brief Fills a camera structure with the default values as defined in camera.h
 * 
 * @param camera The camera struct that needs to be filled 
 */
void engine_camera_default_values(camera* camera);

//==============================================================================//

/**
 * @brief After a change in camera state it is necessary to update some vectors.
 * This function does that.
 * 
 * @param camera The camera whose vectors need updating
 */
void engine_camera_update_vectors(camera* camera);

//==============================================================================//

/**
 * @brief Gets the view matrix from a camera and stores it in dest.
 * 
 * @param camera   The camera whose view matrix is needed 
 * @param dest     Where the view matrix is stored.   
 */
void engine_camera_view_matrix(camera* camera, mat4 dest);

//==============================================================================//
#define CAMERA_DEFAULT_YAW -90.0f;
#define CAMERA_DEFAULT_PITCH 0.0f;
#define CAMERA_DEFAULT_SPEED 7.5f;
#define CAMERA_DEFAULT_SENSITIVITY 0.1f; 
#define CAMERA_DEFAULT_ZOOM 45.0f;


void engine_camera_process_keyboard(camera* camera, MOVEMENT_DIRECTION direction, float deltaTime);
void engine_camera_process_mouse_movement(camera* camera, float xoffset, float yoffset, unsigned char constrainPitch);
void engine_camera_process_mouse_scroll(camera* camera, float yoffset);


#endif