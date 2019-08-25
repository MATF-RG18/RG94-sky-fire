#ifndef DATA_STRUCTURES_H_INCLUDED
#define DATA_STRUCTURES_H_INCLUDED


typedef struct _vec3f
{
    float x, y, z;
} vec3f;


typedef struct _vec2f
{
    float x, y;
} vec2f;

typedef struct _mat4f
{
    float data[4][4];
} mat4f;


typedef struct _mesh_t
{
	int vao;
	int vertex_count;
	int elements;
} mesh_t;


typedef struct _camera_t
{
    vec3f pos;
    float yaw, pitch, roll;
} camera_t;


typedef struct _entity
{
    mesh_t mesh;
    vec3f position;
    vec3f rotation;
    float scale;
    mat4f model_m;
} entity_t;

#endif // DATA_STRUCTURES_H_INCLUDED
