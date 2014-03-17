#include "GL/glew.h"
#include "GL/freeglut.h"

#include <stdlib.h>
#include <iostream> 
#include <stdio.h>

#include "Constants.h"

#include "NFF_Reader.h"

#define MAX_DEPTH 6 
 
int RES_X, RES_Y; 


void reshape(int w, int h) { 
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	glClear(GL_COLOR_BUFFER_BIT); 
	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
 
	gluOrtho2D(0, RES_X-1, 0, RES_Y-1); 
	glMatrixMode (GL_MODELVIEW); 
	glLoadIdentity(); 
}

// Draw function by primary ray casting from the eye towards the scene's objects 
void drawScene() { 

	/*for (int y = 0; y < RES_Y; y++) { 
		for (int x = 0; x < RES_X; x++) { 
			Ray ray = scene->GetCamera()->PrimaryRay(x, y); 
			Color color=rayTracing(ray, 1, 1.0 ); //depth=1, ior=1.0 
			glBegin(GL_POINTS); 
			glColor3f(color.r(), color.g(), color.b()); 
			glVertex2f(x, y); 
			glEnd();
		} 
	} */
	glFlush();

	printf("Terminou!\n"); 
}



int main(int argc, char**argv) {

	NFF_Reader* nffReader = NFF_Reader::getInstance();
	nffReader->loadModel("model.nff");

	//RES_X = scene->GetCamera()->GetResX();
	//RES_Y = scene->GetCamera()->GetResY();
	RES_X = WIDTH;
	RES_Y = HEIGHT;
	
	printf("resx = %d resy= %d.\n", RES_X, RES_Y);

	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); 
 
	glutInitWindowSize(RES_X, RES_Y); 
	glutInitWindowPosition(100, 100); 
	glutCreateWindow("Ray Tracing"); 
	glClearColor(0, 0, 0, 1); 
	glClear(GL_COLOR_BUFFER_BIT); 
 
	glutReshapeFunc(reshape); 
	glutDisplayFunc(drawScene); 
	glDisable(GL_DEPTH_TEST); 
 
	glutMainLoop(); 
	return 0;
} 