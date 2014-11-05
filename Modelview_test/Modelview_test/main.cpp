#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <vector>
#include <time.h>
#include "MathLib3D.h"

//screen size, and aspect ratio for gluPerspective
int screenSizeX = 800;
int screenSizeY = 600;
float aspect = (screenSizeX)/(screenSizeY);
point3D camPos = point3D(0, 5, 20); //Camera Position

float ang = 0; // rotation

vector<Light> LightList;

/* rotationTimer(int time)
*  increments ang and recalls function to keep incrementing.
*  increments different amounts for if isCartoony = true
*/
void rotationTimer(int time) {
	ang+=2;
	glutTimerFunc(1,rotationTimer, 0);
}

void drawCube(int size) {
	glColor3f(1,1,1);
	glutWireCube(size);
	glColor3f(1,0.5f,1);
	glutSolidCube(size);
}

/* display() - the OpenGL display function, this draws the screen
 *  it displays a spinning cube
 */
void display()
{
		glClearColor (0.5, 0.5f, 0.5f, 1.f);
        //clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set MatrixMode to Modelview for adding and updating objects.
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(camPos.x, camPos.y, camPos.z, 0, 2, 0, 0, 1, 0);

		Light light0 = {
			{0, 5, 0, 1},
			{1, 1, 1, 1},
			{1, 1, 1, 1},
			{1, 1, 1, 1}
		};
		LightList.push_back(light0);

		//Whole System
		glPushMatrix();
			
			glPushMatrix();
				glRotatef(ang, 0, 1, 0);
				glScalef(1.5f, 1.5f, 1.f);
				drawCube(1);

				glPushMatrix();
					glTranslatef(5, 0, 2);
					glRotatef(ang * 2, 0, -1, 0);
					drawCube(1);
				glPopMatrix();
			glPopMatrix();
			

			glPushMatrix();
				glTranslatef(5, 0, 0);
				glRotatef(ang * 2, -1, 0, 0);
				drawCube(1);

			glPopMatrix();


		glPopMatrix();//End System

		//swap buffers - rendering is done to the back buffer, bring it forward to display
        glutSwapBuffers();
		//force a redisplay, to keep the animation running
        glutPostRedisplay();
}

/* main(int argc, char** argv)
*   initialize all glut initization.
*/
int main(int argc, char** argv)
{
	srand(time(NULL));

    //glut initialization stuff:
    // set the window size, display mode, and create the window
    glutInit(&argc, argv);
    glutInitWindowSize(screenSizeX, screenSizeY);
	glutInitWindowPosition(450,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Modelview test");

    //enable Z buffer test, otherwise things appear in the order they're drawn
    glEnable(GL_DEPTH_TEST);

    //setup the initial view
    // change to projection matrix mode, set the extents of our viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60, aspect, 1, 100);
		
    //set background colour to grey
    glClearColor(1, 0.5, 0.5, 0);

	//lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	createLights();
	
	//enable culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glutDisplayFunc(display);

	//start rotation incrementation
	glutTimerFunc(10,rotationTimer,0);

    //start the program!
    glutMainLoop();

    return 0;
}