#include <string.h>
#include <math.h>

#include <matrices.h>
#include <helpers.h>
#include <vector.h>

vec3f matrix_vector_multiply(mat4f mat, vec3f vec)
{
    float v[4] = {vec.x, vec.y, vec.z, 1.0f};
    float r[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    int i, j;
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            r[i] += mat.data[i][j] * v[j];
        }

    }
    return (vec3f) {r[0], r[1], r[2]};
}

void create_perspective_projection_matrix(mat4f *result, float fov, float aspect, float near_plane, float far_plane)
{

    float frustum_depth = far_plane - near_plane;
    float one_over_depth = 1.0f / frustum_depth;
    float y_scale = 1.0f / tanf(to_radians(fov * 0.5f));
    float x_scale = y_scale / aspect;

    memset(result -> data, 0, 4 * 4 * sizeof(float));

    result -> data[0][0] = x_scale;
    result -> data[1][1] = y_scale;
    result -> data[2][2] = -((far_plane + near_plane) * one_over_depth);
    result -> data[3][2] = -1.0f;
    result -> data[2][3] = -((2.0f * near_plane * far_plane) * one_over_depth);
    result -> data[3][3] = 0.0f;
/*

    result -> data[1][1] = 1 / tanf(to_radians(0.5f * fov));
    result -> data[0][0] = result -> data[1][1] / aspect;
    result -> data[2][2] = far_plane * one_over_depth;
    result -> data[3][2] = (-far_plane * near_plane) * one_over_depth;
    result -> data[2][3] = 1;
    result -> data[3][3] = 0;
*/
}

void create_translation_matrix(mat4f *result, float dx, float dy, float dz)
{
    memset(result -> data, 0, 4 * 4 * sizeof(float));
    result -> data[0][0] = 1.0f;
    result -> data[1][1] = 1.0f;
    result -> data[2][2] = 1.0f;
    result -> data[3][3] = 1.0f;

    result -> data[0][3] = dx;
    result -> data[1][3] = dy;
    result -> data[2][3] = dz;
}

void create_scale_matrix(mat4f *result, float sx, float sy, float sz)
{

    memset(result -> data, 0, 4 * 4 * sizeof(float));
    result -> data[0][0] = sx;
    result -> data[1][1] = sy;
    result -> data[2][2] = sz;
    result -> data[3][3] = 1.0f;
}

static void mat_rotate_x(mat4f *result, float angle)
{
    memset(result -> data, 0, 4 * 4 * sizeof(float));
    result -> data[0][0] = 1.0f;
    result -> data[1][1] = cosf(to_radians(angle));
    result -> data[2][2] = cosf(to_radians(angle));
    result -> data[3][3] = 1.0f;
    result -> data[1][2] = -sinf(to_radians(angle));
    result -> data[2][1] = sinf(to_radians(angle));
}

static void mat_rotate_y(mat4f *result, float angle)
{
    memset(result -> data, 0, 4 * 4 * sizeof(float));
    result -> data[0][0] = cosf(to_radians(angle));
    result -> data[1][1] = 1.0f;
    result -> data[2][2] = cosf(to_radians(angle));
    result -> data[3][3] = 1.0f;
    result -> data[0][2] = sinf(to_radians(angle));
    result -> data[2][0] = -sinf(to_radians(angle));
}

static void mat_rotate_z(mat4f *result, float angle)
{
    memset(result -> data, 0, 4 * 4 * sizeof(float));
    result -> data[0][0] = cosf(to_radians(angle));
    result -> data[1][1] = cosf(to_radians(angle));
    result -> data[2][2] = 1.0f;
    result -> data[3][3] = 1.0f;
    result -> data[0][1] = -sinf(to_radians(angle));
    result -> data[1][0] = sinf(to_radians(angle));
}

void create_rotation_matrix(mat4f *result, float rot_x, float rot_y, float rot_z)
{
    mat4f mrx, mry, mrz, mryz;
    mat_rotate_x(&mrx, rot_x);
    mat_rotate_y(&mry, rot_y);
    mat_rotate_z(&mrz, rot_z);

    mryz = matrix_multiply(mry, mrz);
    *result = matrix_multiply(mrx, mryz);
}

void create_camera_view_matrix(mat4f *result, camera_t c)
{
    create_view_matrix(result, c.pos.x, c.pos.y, c.pos.z, c.yaw, c.pitch, c.roll);
}



void mat4f_show(mat4f m)
{
    int i, j;
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            printf("%4.2f ", m.data[i][j]);
        }
        printf("\n");
    }
}

void create_view_matrix(mat4f *result, float x, float y, float z, float yaw, float pitch, float roll)
{
    matrix_set_to_identity(result);
    mat4f transl, mrx, mry;

    create_translation_matrix(&transl, -x, -y, -z);

    mat_rotate_x(&mrx, pitch);
    mat_rotate_y(&mry, yaw);

    *result = matrix_multiply(mrx, mry);


    *result = matrix_multiply(*result, transl);
}

void matrix_set_to_identity(mat4f *result)
{
    memset(result -> data, 0, 4 * 4 * sizeof(float));
    result -> data[0][0] = 1.0f;
    result -> data[1][1] = 1.0f;
    result -> data[2][2] = 1.0f;
    result -> data[3][3] = 1.0f;

}

void create_model_matrix_entity(mat4f *result, const entity_t const *entity)
{
    create_model_matrix(result, entity -> position.x, entity -> position.y, entity -> position.z, entity -> rotation.x, entity -> rotation.y, entity -> rotation.z, entity -> scale);
}

void create_model_matrix(mat4f *result, float dx, float dy, float dz, float rot_x, float rot_y, float rot_z, float scale)
{
    memset(result, 0, 4 * 4 * sizeof(float));
    mat4f transl, rot, sca;
    create_translation_matrix(&transl, dx, dy, dz);
    create_scale_matrix(&sca, scale, scale, scale);
    create_rotation_matrix(&rot, rot_x, rot_y, rot_z);
    *result = matrix_multiply(transl, rot);
    *result = matrix_multiply(*result, sca);
}


mat4f matrix_multiply(mat4f a, mat4f b)
{
    mat4f result;
    int i, j, k;

    float r;

    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            r = 0.0f;
            for(k = 0; k < 4; k++)
            {
                r += a.data[i][k] * b.data[k][j];
            }
            result.data[i][j] = r;
        }
    }
    return result;
}

mat4f *matrix_multiply_pointers(mat4f *result, mat4f *a, mat4f *b)
{
    int i, j, k;

    float r;

    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            r = 0.0f;
            for(k = 0; k < 4; k++)
            {
                r += a -> data[i][k] * b -> data[k][j];
            }
            result -> data[i][j] = r;
        }
    }
    return result;
}

