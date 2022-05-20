#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <stdio.h>
#include <string.h>

//==============================================================================//

/**
 * @brief Gets shader compilation log for debugging. Is enabled by defining DEBUG_SHADER
 * 
 * @param[in] object            Shader or program to get the log from
 * @param[in] glGet__iv         Function pointer to OpenGL function that gets iv (info about log)       
 * @param[in] glGet__InfoLog    Fucntion pointer to OpenGL function that gets the log data
 */
void engine_shader_info_log(
    GLuint object,
    PFNGLGETSHADERIVPROC glGet__iv,
    PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
);

//==============================================================================//

/**
 * @brief Takes a vertex and fragment shader and links them into a shader program.
 * Deletes the shaders afterwards as they are now loaded into the program
 * 
 * @param[in] vertex_shader ID of a compiled vertex shader
 * @param[in] fragment_shader ID of a compiled fragment shader
 * @return Linked shader program ID as unsigned integer
 */
GLuint engine_shader_link_program(GLuint vertex_shader, GLuint fragment_shader);

//==============================================================================//

/**
 * @brief Takes a filepath to shader source code and takes type of shader and builds
 * a shader whose ID is returned as an unsigned integer
 * @param[in] filename Location of file where shader source code is stored
 * @param[in] type The type of shader as GL_<TYPE>_SHADER where TYPE = VERTEX || FRAGMENT || GEOMETRY
 * @return Compiled shader ID as unsigned integer 
 */
GLuint engine_shader_load_from_file(const char* filename, GLint type);

//==============================================================================//

/**
 * @brief Wrapper function that allows a shader program to be built for given paths 
 * to both vertex and fragment source codes. 
 * @param[in] vertex_file String that contains the filepath to vertex shader source code
 * @param[in] fragment_file String that contains the filepath to fragment shader source code
 * @return Linked shader program ID as unsigned integer
 */
GLuint engine_shader_make_program_from_files(const char* vertex_file, const char* fragment_file);

//==============================================================================//

#endif