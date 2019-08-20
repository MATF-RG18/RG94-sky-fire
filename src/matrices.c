#include <string.h>
#include <math.h>

#include <matrices.h>

void create_perspective_projection_matrix(mat4f *result, float fov, float aspect, float near_plane, float far_plane)
{

    float frustum_depth = far_plane - near_plane;
    float one_over_depth = 1 / frustum_depth;

    memset(result -> data, 0, 4 * 4 * sizeof(float));

    result -> data[1][1] = 1 / tanf(0.5f * fov * (M_PI / 180.0f));
    result -> data[0][0] = result -> data[1][1] / aspect;
    result -> data[2][2] = far_plane * one_over_depth;
    result -> data[3][2] = (-far_plane * near_plane) * one_over_depth;
    result -> data[2][3] = 1;
    result -> data[3][3] = 0;
}
