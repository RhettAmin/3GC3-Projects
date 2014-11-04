/*   
Authors: Kevin Ly, Rhett Amin, David Elsonbaty 
Student Numbers: 1144604, 1130789,   
File: Box.h 
-----------------------------  
 Header File for the Box Class, Contains all variables, methods and constructors. 
*/
#include "MathLib3D.h" 
  
class Box 
{ 
public: 
    //Variables 
    float width, height; 
    bool *faceRight; 
    point3D *pos; 
    point3D *dirVec; 
  
    //Constructor 
    Box(); 
    Box(float w, float h, point3D* posC, bool* faceRight); 
    Box(float w, float h, point3D* posC); 
  
  
    //Methods 
    void draw(); 
    void drawWire(); 
    void drawNoAngle(); 
    void drawWireNoAngle(); 
    void drawBullet(colourStruc* c); 
    void move(float x, float y, float z); 
}; 