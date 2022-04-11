#include <internal/camera.h>
#include <string.h>

const float CAMERA_DEFAULT_YAW          = -90.0f;
const float CAMERA_DEFAULT_PITCH        =   0.0f;
const float CAMERA_DEFAULT_SPEED        =   2.5f;
const float CAMERA_DEFAULT_SENSITIVITY  =   0.1f; 
const float CAMERA_DEFAULT_ZOOM         =  45.0f;

void engine_camera_default_values(camera* camera){
    memset(camera, 0, offsetof(struct camera, yaw));        // Make all vector values 0
    camera->position[2] = 3.0f;
    camera->front[2] = -1.0f;
    camera->world_up[1] = 1.0f;
    camera->yaw = CAMERA_DEFAULT_YAW;
    camera->pitch = CAMERA_DEFAULT_PITCH;
    camera->speed = CAMERA_DEFAULT_SPEED;
    camera->sensitivity = CAMERA_DEFAULT_SENSITIVITY;
    camera->zoom =CAMERA_DEFAULT_ZOOM; 
    engine_camera_update_vectors(camera);
}

void engine_camera_update_vectors(camera* camera){
    vec3 temp = {
        cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch)),
        sin(glm_rad(camera->pitch)),
        sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch))
    };
    glm_normalize_to(temp, camera->front);
    glm_cross(camera->front, camera->world_up, temp);
    glm_normalize_to(temp, camera->right);
    glm_cross(camera->right, camera->front, temp);
    glm_normalize_to(temp, camera->up);
}

void engine_camera_view_matrix(camera* camera, mat4 dest){
    glm_vec3_add(camera->position, camera->front, dest[0]); // Using first column in dest as temp vector for addition
    glm_lookat(camera->position, dest[0], camera->up, dest);   
}

void engine_camera_process_keyboard(camera* camera, MOVEMENT_DIRECTION direction, float deltaTime){
    float velocity = camera->speed * deltaTime;
    vec3 result;

    if(direction == FORWARD){
        glm_vec3_scale(camera->front, velocity, result);
        glm_vec3_add(camera->position, result, camera->position);
    }

    if(direction == BACKWARD){
        glm_vec3_scale(camera->front, velocity, result);
        glm_vec3_sub(camera->position, result, camera->position);
    }

    if(direction == LEFT){
        glm_vec3_scale(camera->right, velocity, result);
        glm_vec3_sub(camera->position, result, camera->position);
    }
    
    if(direction == RIGHT){
        glm_vec3_scale(camera->right, velocity, result);
        glm_vec3_add(camera->position, result, camera->position);
    }
}

void engine_camera_process_mouse_movement(camera* camera, float xoffset, float yoffset, unsigned char constrainPitch){
    xoffset *= camera->sensitivity;
    yoffset *= camera->sensitivity;

    camera->yaw += xoffset;
    camera->pitch += yoffset;

    if(constrainPitch){
        if(camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if(camera->pitch < -89.0f)
            camera->pitch = -89.0f;
    }

    engine_camera_update_vectors(camera);
}

void engine_camera_process_mouse_scroll(camera* camera, float yoffset){
    camera->zoom -= yoffset;
    if(camera->zoom < 1.0f)
        camera->zoom = 1.0f;
    if(camera->zoom > 45.0f)
        camera->zoom = 45.0f;
}