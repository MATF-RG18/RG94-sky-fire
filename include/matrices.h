#ifndef MATRICES_H_INCLUDED
#define MATRICES_H_INCLUDED

#include <data_structures.h>

void mat4f_show(mat4f m);

void create_perspective_projection_matrix(mat4f *result, float fov, float aspect, float near_plane, float far_plane);
void create_translation_matrix(mat4f *result, float dx, float dy, float dz);
void create_rotation_matrix(mat4f *result, float rot_x, float rot_y, float rot_z);
void create_scale_matrix(mat4f *result, float sx, float sy, float sz);
void create_view_matrix(mat4f *result, float x, float y, float z, float yaw, float pitch, float roll);
void create_camera_view_matrix(mat4f *result, camera_t c);

void matrix_set_to_identity(mat4f *result);

void create_model_matrix(mat4f *result, float dx, float dy, float dz, float rot_x, float rot_y, float rot_z, float scale);
void create_model_matrix_entity(mat4f *result, const void const *entity);


mat4f matrix_multiply(mat4f a, mat4f b);
mat4f *matrix_multiply_pointers(mat4f *result, mat4f *a, mat4f *b);


vec3f matrix_vector_multiply(mat4f mat, vec3f vec);
#endif // MATRICES_H_INCLUDED
