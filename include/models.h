#ifndef MODELS_H_INCLUDED
#define MODELS_H_INCLUDED

#include <vector.h>

typedef struct _mesh_t
{
	int vao;
	int vertex_count;
} mesh_t;


typedef struct _entity
{
    mesh_t mesh;
    vec3f position;
    vec3f rotation;
    float scale;
} entity_t;

void create_entity(entity_t *result, mesh_t mesh, float x, float y, float z, float rot_x, float rot_y, float rot_z, float scale);
void create_entity_vecs(entity_t *result, mesh_t mesh, vec3f position, vec3f rotation, float scale);


void load_wavefront_mesh(const char *filepath, mesh_t *m);
void load_wavefront_mesh_with_rotation(const char *filepath, mesh_t *m, vec3f rot);

#endif // MODELS_H_INCLUDED
