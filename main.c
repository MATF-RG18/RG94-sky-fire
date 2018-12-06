#include<stdio.h>
#include<stdlib.h>
#include <GL\glut.h>

//FLOAT LIST

typedef struct _list_node
{
	float x, y, z;
	struct _list_node *next;
}list_node;

typedef struct _linked_list
{
	list_node *head, *tail;
	int count;
}linked_list;


list_node* add_node(list_node *tail, float x, float y, float z)
{
	list_node *new_node = malloc(sizeof(list_node));
	new_node -> x = x;
	new_node -> y = y;
	new_node -> z = z;
	new_node -> next = NULL;
	
	if(!tail) return new_node;
	
	tail -> next = new_node;
	return new_node;	
}

void list_init(linked_list *list)
{
	list -> count = 0;
	list -> head = NULL;
	list -> tail = NULL;
}

void list_add(linked_list *list, float x, float y, float z)
{
	list -> tail = add_node(list -> tail, x, y, z);
	if(list -> head == NULL) list -> head = list -> tail;
	list -> count ++;
}

list_node* list_get(linked_list *list, int index)
{
	list_node *i;
	for( i = list -> head; i != NULL; i = i -> next)
	{
		if(!index)
		{
			return i;
		}
		index--;
	}
	return NULL;
}

void list_node_free(list_node *node)
{
	if(!node) return;
	list_node_free(node -> next);
	free(node);
}

void list_free(linked_list *list)
{
	if(!list) return;
	list_node_free(list -> head);
	
}

// INT LIST
typedef struct _int_list_node
{
	int x;
	struct _int_list_node *next;
}int_list_node;

typedef struct _int_linked_list
{
	int_list_node *head, *tail;
	int count;
}int_linked_list;


int_list_node* int_add_node(int_list_node *tail, int x)
{
	int_list_node *new_node = malloc(sizeof(int_list_node));
	new_node -> x = x;
	new_node -> next = NULL;
	
	if(!tail) return new_node;
	
	tail -> next = new_node;
	return new_node;	
}

void int_list_init(int_linked_list *list)
{
	list -> count = 0;
	list -> head = NULL;
	list -> tail = NULL;
}

void int_list_add(int_linked_list *list, int x)
{
	list -> tail = int_add_node(list -> tail, x);
	if(list -> head == NULL) list -> head = list -> tail;
	list -> count ++;
}

int_list_node* int_list_get(int_linked_list *list, int index)
{
	int_list_node *i;
	for( i = list -> head; i != NULL; i = i -> next)
	{
		if(!index)
		{
			return i;
		}
		index--;
	}
	return NULL;
}

void int_list_node_free(int_list_node *node)
{
	if(!node) return;
	int_list_node_free(node -> next);
	free(node);
}

void int_list_free(int_linked_list *list)
{
	if(!list) return;
	int_list_node_free(list -> head);
	
}

typedef struct _mesh
{
	int data_buffer;
	int index_buffer;
	
}mesh;


void load_wavefont_mesh(const char *filepath, mesh *m)
{
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
			list_add(&vertices, x, y, z);
		}
		else if (line[0] == 'v' && line[1] == 't')
		{
			sscanf(line + 3, "%f%f", &x, &y);
			list_add(&uv_coordinates, x, y, 0.0f);
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			sscanf(line + 3, "%f%f%f", &x, &y, &z);
			list_add(&normals, x, y, z);
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
		
		
		float_buffer[8 * i] = vertex_data -> x;
		float_buffer[8 * i + 1] = vertex_data -> y;
		float_buffer[8 * i + 2] = vertex_data -> z;
		
		float_buffer[8 * i + 3] = uv_data -> x;
		float_buffer[8 * i + 4] = uv_data -> y;
		
		float_buffer[8 * i + 5] = normal_data -> x;
		float_buffer[8 * i + 6] = normal_data -> y;
		float_buffer[8 * i + 7] = normal_data -> z;
		
		
		
	}
	// GL BUFFER DATA
	
	/*
	int data_buffer;
	glGenBuffers(1, &data_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, data_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_indices.count * 8 * sizeof(float), float_buffer, GL_STATIC_DRAW);
	
	glDeleteBuffers(1, &data_buffer);
	
	*/
	
	
	free(float_buffer);
	list_free(&vertices);
	list_free(&uv_coordinates);
	list_free(&normals);
	
	int_list_free(&vertex_indices);
	int_list_free(&uv_indices);
	int_list_free(&normal_indices);
	
	fclose(f);
}


static void on_display(void);
static void keyboard_callback(unsigned char key, int x, int y);
static void mouse_callback(int key, int state, int x, int y);
void reshape(int w, int h);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);

	glutInitWindowSize(1024, 512);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Igra");

	glutMouseFunc(mouse_callback);
	glutKeyboardFunc(keyboard_callback);
	glutDisplayFunc(on_display);
	glutIdleFunc (on_display);
	glutReshapeFunc (reshape);
	
	
	load_wavefont_mesh("res\\kocka.obj");

	//glClearColor(0.75f, 0.0f, 0.75f, 0.0f);
	printf("Starting the game!\n");
	
	
	glutMainLoop();
	return 0;
}


static void keyboard_callback(unsigned char key, int x, int y)
{
	if(key == 27)
	{
		// Escape clicked
		printf("Shutting down!\nGoodbye...\n");
		exit(0);
	}
	
}

void reshape(int w, int h) 
{
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode (GL_MODELVIEW);
}

static void mouse_callback(int key, int state, int x, int y)
{
	// Provera koji je key pritisnut i gde
	printf("%d %d %d %d\n", key, state, x, y);
}

static void on_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    // background render
    
    
    glutSwapBuffers();
}
