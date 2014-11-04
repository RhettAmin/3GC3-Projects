/*  
Authors: Rhett Amin, Kevin Ly, David Elsonbaty
Student Numbers: 1060085, 1144604 
File: HealthBar.cpp 
----------------------------- 
Class handles drawing the healthbars
*/

#include "HealthBar.h"
#include <GL/glut.h>
#include <GL/gl.h>

//Constructor
HealthBar::HealthBar(point3D loc, float* healthIN){
    health = healthIN;
	maxHealth = 100;
    depth = 1;
    location = loc;
}

/* drawHealthBar()
	draws the healthbar to the screen
 */
void HealthBar::drawHealthBar(bool isLeftAligned){
    
    glDisable(GL_LIGHTING);
		
        //Health
        glPushMatrix();
            glColor3f(1,0,0);
			glTranslatef(location.x, location.y, location.z);
            glScalef(*health/10, 1, 0.25);
            glutSolidCube(1);
        glPopMatrix();
    
    
        //Max Health
        glPushMatrix();
            glColor3f(0,0,0);
            glTranslatef(location.x, location.y, location.z  - 0.26);
            glScalef(10, 1, 0.01);
            glutSolidCube(1);
        glPopMatrix();
    
    glEnable(GL_LIGHTING);

}