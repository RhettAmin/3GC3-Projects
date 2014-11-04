/*  
Authors: Rhett Amin, Kevin Ly, David Elsonbaty
Student Numbers: 1060085, 1144604 
File: Platform.h 
----------------------------- 
Header class for Platform.cpp
*/

#include "MathLib3D.h"
#include <GL/glut.h>
#include <GL/gl.h>

#ifndef _H_PLATFORM_
#define _H_PLATFORM_

class Platform
{
private:
public:
	float width;
    float depth;
    point3D location;

    Platform();
    void createPlatform(float widthIn, point3D locIn);
    void drawPlatform(GLuint tex);
	void drawFaces(point3D loc, float w, float d, GLuint tex);
};

#endif
