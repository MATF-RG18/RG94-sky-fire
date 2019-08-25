#include <stdio.h>
#include <glad\glad.h>
#include <lists.h>
#include <matrices.h>

#include <models.h>


void load_wavefront_mesh_with_rotation(const char *filepath, mesh_t *m, vec3f rot)
{
    mat4f rotm;
    create_rotation_matrix(&rotm, rot.x, rot.y, rot.z);

    linked_list vertices, uv_coordinates, normals;
	list_init(&vertices);
	list_init(&uv_coordinates);
	list_init(&normals);
	FILE *f = fopen(filepath, "rt");

	char line[256];
	float x, y, z;


	while(!feof(f))
	{
		fgets(line, 256, f);
		if(line[0] == 'v' && line[1] == ' ')
		{
			sscanf(line + 2, "%f%f%f", &x, &y, &z);
			vec3f rotated = matrix_vector_multiply(rotm, vec3f_create(x, y, z));
			list_add(&vertices, rotated.x, rotated.y, rotated.z);
		}
		else if (line[0] == 'v' && line[1] == 't')
		{
			sscanf(line + 3, "%f%f", &x, &y);
			list_add(&uv_coordinates, x, y, 0.0f);
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			sscanf(line + 3, "%f%f%f", &x, &y, &z);
			vec3f rotated = matrix_vector_multiply(rotm, vec3f_create(x, y, z));
			list_add(&normals, rotated.x, rotated.y, rotated.z);
		}
		else if (line[0] == 'f')
		{
			break;
		}

	}


	int_linked_list vertex_indices, uv_indices, normal_indices;
	int_list_init(&vertex_indices);
	int_list_init(&uv_indices);
	int_list_init(&normal_indices);
	int fake_uvs = 0;


	while(!feof(f))
	{
		while(line[0] != 'f' && !feof(f))
		{
			fgets(line, 256, f);
		}

		int v1, v2, v3;
		int n1, n2, n3;
		int t1, t2, t3;

		int matches = sscanf(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
		if(matches != 9)
		{
			matches = sscanf(line + 2, "%d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3);
			if(matches != 6)
			{
				fprintf(stderr, "File can't be read, try exporting with other options [matches = %d]", matches);
				fprintf(stderr, "error on: %s\n", line);
				return;
			}
			else
			{
				fake_uvs = 1;
				t1 = t2 = t3 = 1;
			}


		}

		int_list_add(&vertex_indices, v1);
		int_list_add(&vertex_indices, v2);
		int_list_add(&vertex_indices, v3);

		int_list_add(&uv_indices, t1);
		int_list_add(&uv_indices, t2);
		int_list_add(&uv_indices, t3);

		int_list_add(&normal_indices, n1);
		int_list_add(&normal_indices, n2);
		int_list_add(&normal_indices, n3);

		fgets(line, 256, f);


	}

	if(fake_uvs)
		list_add(&uv_coordinates, 0.0f, 0.0f, 0.0f);


	float *float_buffer = malloc(vertex_indices.count * 8 * sizeof(float));

	int i;
	for(i = 0; i < vertex_indices.count; i++)
	{

		int vert_ind = int_list_get(&vertex_indices, i) -> x;
		int uv_ind = int_list_get(&uv_indices, i) -> x;
		int norm_ind = int_list_get(&normal_indices, i) -> x;


		list_node *vertex_data = list_get(&vertices, vert_ind - 1);
		list_node *uv_data = list_get(&uv_coordinates, uv_ind - 1);
		list_node *normal_data = list_get(&normals, norm_ind - 1);


		float_buffer[8 * i + 0] = vertex_data -> x;
		float_buffer[8 * i + 1] = vertex_data -> y;
		float_buffer[8 * i + 2] = vertex_data -> z;

		float_buffer[8 * i + 3] = normal_data -> x;
		float_buffer[8 * i + 4] = normal_data -> y;
		float_buffer[8 * i + 5] = normal_data -> z;


		float_buffer[8 * i + 6] = uv_data -> x;
		float_buffer[8 * i + 7] = 1.0f - uv_data -> y;

	}
	// GL BUFFER DATA

	int vao;
	glGenVertexArrays(1, &vao);

	m -> vao = vao;
	m -> vertex_count = vertex_indices.count;
	m -> elements = 0;

	glBindVertexArray(vao);

	int data_buffer;
	glGenBuffers(1, &data_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, data_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_indices.count * 8 * sizeof(float), float_buffer, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 3 * sizeof(float));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 6 * sizeof(float));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



	free(float_buffer);
	list_free(&vertices);
	list_free(&uv_coordinates);
	list_free(&normals);

	int_list_free(&vertex_indices);
	int_list_free(&uv_indices);
	int_list_free(&normal_indices);

	fclose(f);
}



void load_wavefront_mesh(const char *filepath, mesh_t *m)
{
    load_wavefront_mesh_with_rotation(filepath, m, vec3f_create(0.0f, 0.0f, 0.0f));
}




void create_entity(entity_t *result, mesh_t mesh, float x, float y, float z, float rot_x, float rot_y, float rot_z, float scale)
{
    create_entity_vecs(result, mesh, vec3f_create(x, y, z), vec3f_create(rot_x, rot_y, rot_z), scale);
}

void create_entity_vecs(entity_t *result, mesh_t mesh, vec3f position, vec3f rotation, float scale)
{
    result -> mesh = mesh;
    result -> position = position;
    result -> rotation = rotation;
    result -> scale = scale;
}
