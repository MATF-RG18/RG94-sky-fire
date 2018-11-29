#include<stdio.h>
#include<stdlib.h>
#include <GL\glut.h>
#include <SOIL\SOIL.h>

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
	list_node *new_node = malloc(sizeof())
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

void load_wavefont_mesh(const char *filepath)
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
		
		int total = (3 * 2 + 2) * vertices.count;
		float *float_buffer = malloc(total * sizeof(float));
		
		
		
		
		
		free(float_buffer);
	}
	
	
	
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