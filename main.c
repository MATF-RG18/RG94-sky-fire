#include<stdio.h>
#include<stdlib.h>
#include <GL\glut.h>
#include <SOIL\SOIL.h>

static void on_display(void);
static void keyboard_callback(unsigned char key, int x, int y);
static void mouse_callback(int key, int state, int x, int y);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Igra");

	glutMouseFunc(mouse_callback);
	glutKeyboardFunc(keyboard_callback);
	glutDisplayFunc(on_display);
	
	int w, h;
	unsigned char* image = SOIL_load_image("aircraft.jpeg", &w, &h, 0,
	SOIL_LOAD_RGB);
	

	glClearColor(0.75f, 0.0f, 0.75f, 0.0f);
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

static void mouse_callback(int key, int state, int x, int y)
{
	// Provera koji je key pritisnut i gde
	printf("%d %d %d %d\n", key, state, x, y);
}

static void on_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    	glColor3f(1.0f, 0.0f, 0.0f);
    	glVertex3f(0, 0, 0);
    	glVertex3f(0.5f, 0, 0);
    	glVertex3f(0, 0.5f, 0);
    
    	glColor3f(1.0f, 0.0f, 0.0f);
    	glVertex3f(250, 200, 0);
    	glVertex3f(200, 250, 0);
    	glVertex3f(250, 250, 0);
    glEnd();
    
    glutSwapBuffers();
}
