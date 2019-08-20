#ifndef HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED

#include <glad\glad.h>

static const char * DEBUG_VERT_SHADER_SRC = "\
#version 410 core\n\
layout (location = 0) in vec3 pos;\n\
\n\
out float nesto;\n\
void main()\n\
{\n\
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n\
    nesto = pos.y + 0.5;\n\
}\n";

static const char * DEBUG_FRAG_SHADER_SRC = "\
#version 410 core\n\
out vec4 final_colour;\n\
in float nesto;\n\
uniform float vreme;\n\
\n\
void main()\n\
{\n\
    final_colour = vec4(1.0, 0.5, 0.2, 1.0) * nesto * vreme;\n\
}\n";


GLuint create_program(const char *vertex_source_filepath, const char *fragment_source_filepath);
GLuint create_program_from_source(const char *vertex_source, const char *fragment_source);
char* read_file_content(const char *filepath);
int file_size(const char *filepath);


#endif // HELPERS_H_INCLUDED
