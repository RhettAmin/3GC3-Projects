/*   
Authors: Kevin Ly, Rhett Amin, David Elsonbaty 
Student Numbers: 1144604, 1130789,   
File: Animation.h 
-----------------------------  
Class for for Box, Creates a box object which can be drawn, takes inputs size: width, hieght, and point3d location 
can draw different type of cubes: solid and wire. 
contains rotation depending on which way the fighter is looking towards. 
*/
  
#include "Box.h" 
#include <gl/glut.h> 
  
//Constructors 
Box::Box(){ 
    point3D *fu = new point3D(0,0,0); 
    pos = fu; 
} 
  
Box::Box(float w, float h, point3D* posC, bool* faceRight){ 
    width = w; 
    height = h; 
    pos = posC;  
    this->faceRight = faceRight; 
    dirVec = new point3D(0,0,0); 
} 
  
Box::Box(float w, float h, point3D* posC){ 
    width = w; 
    height = h; 
    pos = posC;  
    dirVec = new point3D(0,0,0); 
} 
  
//Methods: 
  
//Draw method 
//Draws glutsolidcube with face indication 
void Box::draw(){ 
    glPushMatrix(); 
        glTranslatef(pos->x,pos->y,pos->z); 
        glScaled(width,height,1); 
        if(*faceRight){ 
            glRotated(-30,0,1,0); 
        } else { 
            glRotated(30,0,1,0); 
        } 
        glutSolidCube(1); 
    glPopMatrix(); 
} 
  
//drawWire method 
//Draws glutwirecube with face indication 
void Box::drawWire(){ 
    glPushMatrix(); 
        glTranslatef(pos->x,pos->y,pos->z); 
        glScaled(width,height,1); 
        if(*faceRight){ 
            glRotated(-30,0,1,0); 
        } else { 
            glRotated(30,0,1,0); 
        } 
        glutWireCube(1); 
    glPopMatrix(); 
} 
  
//drawNoAngle method 
//Draws glutsolidcube with  no face indication 
void Box::drawNoAngle(){ 
    glPushMatrix(); 
        glTranslatef(pos->x,pos->y,pos->z); 
        glScaled(width,height,1); 
        glutSolidCube(1); 
    glPopMatrix(); 
} 
//Draw method 
//Draws glutwirecube with no face indication 
void Box::drawWireNoAngle(){ 
    glPushMatrix(); 
        glTranslatef(pos->x,pos->y,pos->z); 
        glScaled(width,height,1); 
        glutWireCube(1); 
    glPopMatrix(); 
} 
//DrawBullet method 
//Draws bullet 
void Box::drawBullet(colourStruc* c){ 
    glPushMatrix(); 
        glEnable(GL_COLOR_MATERIAL); 
            glColor4f(c->r, c->g, c->b, 1); 
            glRotatef(90, 0, 1, 0); 
            glTranslatef(pos->x,pos->y,pos->z); 
            glScaled(width,height,1); 
            glutSolidCube(1); 
        glDisable(GL_COLOR_MATERIAL); 
    glPopMatrix(); 
} 
  
//Move Method: 
//Move box based on input x,y,z 
void Box::move(float x, float y , float z) 
{ 
    pos->x += x; 
    pos->y += y; 
    pos->z += z; 
}