#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>

static void on_display(void);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	
	glutDisplayFunc(on_display);
	
	glClearColor(0.75, 0.75, 0.75, 0);
	
	glutMainLoop();
	
	return 0;
}

static void on_display(void)
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
