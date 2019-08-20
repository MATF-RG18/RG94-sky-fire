#ifndef MATRICES_H_INCLUDED
#define MATRICES_H_INCLUDED

typedef struct _mat4f
{
    float data[4][4];
} mat4f;

void create_perspective_projection_matrix(mat4f *result, float fov, float aspect, float near_plane, float far_plane);
void create_translation_matrix(mat4f *result, float dx, float dy, float dz);
void create_rotation_matrix(mat4f *result, float rot_x, float rot_y, float rot_z);
void create_scale_matrix(mat4f *result, float sx, float sy, float sz);

void matrix_set_to_identity(mat4f *result);

void create_model_matrix(mat4f *result, float dx, float dy, float dz, float rot_x, float rot_y, float rot_z, float scale);

mat4f matrix_multiply(mat4f a, mat4f b);
mat4f *matrix_multiply_pointers(mat4f *result, mat4f *a, mat4f *b);


#endif // MATRICES_H_INCLUDED
