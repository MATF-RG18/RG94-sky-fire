#ifndef MODELS_H_INCLUDED
#define MODELS_H_INCLUDED

typedef struct _mesh
{
	int vao;
	int vertex_count;


}mesh;

void load_wavefont_mesh(const char *filepath, mesh *m);

#endif // MODELS_H_INCLUDED
