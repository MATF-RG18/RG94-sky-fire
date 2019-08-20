#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <helpers.h>

int file_size(const char *filepath)
{
    int size = 0;
    FILE *f = fopen(filepath, "rt");

    fseek(f, 0L, SEEK_END);

    size = ftell(f);

    fclose(f);

    return size;
}

char* read_file_content(const char *filepath)
{
    int fsize = file_size(filepath);
    FILE *f = fopen(filepath, "rt");

    fseek(f, 0, SEEK_SET);

    char *content = calloc(sizeof(char), fsize + 10);          /* This must later be freed */

    fread(content, 1, fsize, f);

    fclose(f);

    *(content + fsize) = 0;
    return content;
}

GLuint create_program_from_source(const char *vertex_source, const char *fragment_source)
{
    GLuint vert, frag, program;
    int success;
    char info_log[512];

    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertex_source, NULL);
    glCompileShader(vert);

    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vert, 512, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILE_FAILED\n%s\n", info_log);
    }

    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragment_source, NULL);
    glCompileShader(frag);

    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(frag, 512, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILE_FAILED\n%s\n", info_log);
    }


    program = glCreateProgram();

    glAttachShader(program, vert);
    glAttachShader(program, frag);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", info_log);
    }


    glDeleteShader(vert);
    glDeleteShader(frag);

    return program;
}

GLuint create_program(const char *vertex_source_filepath, const char *fragment_source_filepath)
{
    GLuint program;

    char *vert_source = read_file_content(vertex_source_filepath);
    char *frag_source = read_file_content(fragment_source_filepath);


    program = create_program_from_source(vert_source, frag_source);

    free(vert_source);
    free(frag_source);

    return program;
}

inline float to_radians(float degrees)
{
    return degrees * M_PI / 180.0f;
}


void follow_target(vec3f target_pos, vec3f target_rot, float distance, float pitch, float angle, vec3f *r_position, float *r_yaw, float *r_pitch)
{
    *r_pitch = pitch;
    float theta = target_rot.y + angle;
    float hdistance = distance * cosf(to_radians(pitch));
    float vdistance = distance * sinf(to_radians(pitch));
    float offset_x = hdistance * sinf(to_radians(theta));
    float offset_z = hdistance * cosf(to_radians(theta));

    r_position -> x = target_pos.x - offset_x;
    r_position -> y = target_pos.y + vdistance;
    r_position -> z = target_pos.z - offset_z;

    *r_yaw = 180.0f - theta;

}


void camera_follow_target(vec3f target_pos, vec3f target_rot, float distance, float pitch, float angle, camera_t *cam)
{
    follow_target(target_pos, target_rot, distance, pitch, angle, &(cam -> pos), &(cam -> yaw), &(cam -> pitch));
    cam -> roll = 0.0f;
}
