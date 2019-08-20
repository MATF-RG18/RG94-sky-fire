#ifndef MATRICES_H_INCLUDED
#define MATRICES_H_INCLUDED

typedef struct _mat4f
{
    float data[4][4];
} mat4f;

void create_perspective_projection_matrix(mat4f *result, float fov, float aspect, float near_plane, float far_plane);

#endif // MATRICES_H_INCLUDED
