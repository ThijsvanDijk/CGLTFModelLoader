#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>
#include <stdio.h>
#include <string.h>


#ifdef DEBUG_SHADER
static void show_info_log(
    GLuint object,
    PFNGLGETSHADERIVPROC glGet__iv,
    PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
)
{
    GLint log_length;
    char *log;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    log = (char*)malloc(log_length);
    glGet__InfoLog(object, log_length, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
}
#endif

static GLuint app_link_shader_program(GLuint vertex_shader, GLuint fragment_shader){
    GLint program_ok;

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

#ifdef DEBUG_SHADER
    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if(!program_ok){
        fprintf(stderr, "Failed to link shader program!\n");
        show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        return 0;
    }
#endif

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return program;
}

static GLuint app_load_shader_from_file(const char* filename, GLint type){

    FILE *file = fopen(filename, "rb");                         // Open File
    if(file == NULL){                                           // Check if it failed
        fprintf(stdout, "Failed to open file: %s\n", filename); // Failure :(
        return -1;
    }

    fseek(file, 0L, SEEK_END);                              // Find the end of the file
    unsigned long file_size = ftell(file);                  // Get file size
    rewind(file);                                           // Rewind to start of file

    char shader_source[file_size + 1];                      // Allocate space for buffer
    const GLchar * src_ptr = (const GLchar *)&shader_source; // Get pointer to the source 
    
    unsigned char c = 0;                                    // Store null termination char
    memcpy(shader_source + file_size, &c, 1);               // Copy c to back of string
    
    fread(shader_source, file_size, 1, file);               // Copy file into buffer

#ifdef DEBUG_SHADER
    printf("Shader Source:\n%s\n\n", shader_source);        // Print source code to terminal
#endif

    GLuint shader; GLint shader_ok;                         // Declare variables
    shader = glCreateShader(type);                          // Create shader
    glShaderSource(shader, 1, &src_ptr, NULL);              // Add shader source code
    glCompileShader(shader);                                // Compile shader

#ifdef DEBUG_SHADER
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);   // Error Logging
    if(!shader_ok){
        fprintf(stderr, "Failed to compile shader!\n");
        show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        return 0;
    }
#endif
    return shader;                                          // Return the shader
}

static GLuint app_make_shader_program(const char* vertex_file, const char* fragment_file){
    GLuint vertex = app_load_shader_from_file(vertex_file, GL_VERTEX_SHADER);
    GLuint fragment = app_load_shader_from_file(fragment_file, GL_FRAGMENT_SHADER);

    if(vertex == 0 || fragment == 0) return 0;

    GLuint program = app_link_shader_program(vertex, fragment);

    if(!program) return 0;

    return program;
}


#endif