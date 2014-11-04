/*  
Authors: Rhett Amin, Kevin Ly, David Elsonbaty
Student Numbers: 1060085, 1144604 
File: Platform.cpp 
----------------------------- 
This class draws the platform which the fighters fight on.
*/

#include "Platform.h"
#include <iostream>
#include <fstream>
#include <sstream>

//Constructor
Platform::Platform()
{}

/* createPlatform()
	creates the platform
*/
void Platform::createPlatform(float widthIn, point3D locIn) {
	width = widthIn;
	location = locIn;
	depth = 10;
}

/* drawPlatform()
	Draws the platform to the scene
*/
void Platform::drawPlatform(GLuint tex){
    glPushMatrix();
		drawFaces(location, width, depth, tex);
	glPopMatrix();
}

/* drawFaces()
	Creates and draws the platform faces and sets their texture
*/
void Platform::drawFaces(point3D loc, float w, float d, GLuint tex) {
	float vertices[8][3] = { {loc.x - w/2, loc.y - 0.7, loc.z + depth/2}, {loc.x - w/2, loc.y + 0.7, loc.z + depth/2}, {loc.x + w/2, loc.y + 0.7, loc.z + depth/2}, {loc.x + w/2, loc.y - 0.7, loc.z + depth/2}, {loc.x - w/2, loc.y - 0.7, loc.z - depth/2}, {loc.x - w/2, loc.y + 0.7, loc.z - depth/2}, {loc.x + w/2, loc.y + 0.7, loc.z - depth/2}, {loc.x + w/2, loc.y - 0.5, loc.z - depth/2} };
	//                     Top           left          Back          Right         Bottom        Front
	int faces[6][4] = { {1, 2, 6, 5}, {1, 5, 4, 0}, {5, 6, 7, 4}, {7, 6, 2, 3}, {3, 0, 4, 7}, {1, 0, 3, 2} };

	glEnable(GL_TEXTURE_2D); 
		glEnable(GL_COLOR_MATERIAL); 
	
			//Draw each platform
			glBindTexture(GL_TEXTURE_2D, tex);
			
			//Top
			glBegin(GL_POLYGON);	
				glNormal3d(0,1,0);
				glTexCoord2f(1,0); glVertex3fv(vertices[1]);
				glTexCoord2f(0,0); glVertex3fv(vertices[2]);
				glTexCoord2f(0,1); glVertex3fv(vertices[6]);
				glTexCoord2f(1,1); glVertex3fv(vertices[5]);
			glEnd();

			//Left
			glBegin(GL_POLYGON);
				glNormal3d(-1,0,0);
				glTexCoord2f(1,0); glVertex3fv(vertices[1]);
				glTexCoord2f(0,0); glVertex3fv(vertices[5]);
				glTexCoord2f(0,1); glVertex3fv(vertices[4]);
				glTexCoord2f(1,1); glVertex3fv(vertices[0]);
			glEnd();

			//Back
			glBegin(GL_POLYGON);	
				glNormal3d(0,0,-1);
				glTexCoord2f(1,0); glVertex3fv(vertices[5]);
				glTexCoord2f(0,0); glVertex3fv(vertices[6]);
				glTexCoord2f(0,1); glVertex3fv(vertices[7]);
				glTexCoord2f(1,1); glVertex3fv(vertices[4]);
			glEnd();

			//Right
			glBegin(GL_POLYGON);
				glNormal3d(1,0,0);
				glTexCoord2f(1,0); glVertex3fv(vertices[7]);
				glTexCoord2f(0,0); glVertex3fv(vertices[6]);
				glTexCoord2f(0,1); glVertex3fv(vertices[2]);
				glTexCoord2f(1,1); glVertex3fv(vertices[3]);
			glEnd();

			//Bottom
			glBegin(GL_POLYGON);	
				glNormal3d(0,-1,0);
				glTexCoord2f(1,0); glVertex3fv(vertices[3]);
				glTexCoord2f(0,0); glVertex3fv(vertices[0]);
				glTexCoord2f(0,1); glVertex3fv(vertices[4]);
				glTexCoord2f(1,1); glVertex3fv(vertices[7]);
			glEnd();

			//Front
			glBegin(GL_POLYGON);	
				glNormal3d(0,0,1);
				glTexCoord2f(1,0); glVertex3fv(vertices[1]);
				glTexCoord2f(0,0); glVertex3fv(vertices[0]);
				glTexCoord2f(0,1); glVertex3fv(vertices[3]);
				glTexCoord2f(1,1); glVertex3fv(vertices[2]);
			glEnd();
		
			glColor4f(1, 1, 1, 1); 
          
        glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
}

