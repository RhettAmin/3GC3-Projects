/*   
Authors: Kevin Ly, Rhett Amin, David Elsonbaty 
Student Numbers: 1144604, 1130789,   
File: Fighter.h 
-----------------------------  
 Header File for the Fighter Class, Contains all variables, methods and constructors. 
*/

#pragma once 
#include "Animation.h" 
  
class Fighter 
{ 
public: 
  
    //Default points away from posFighter 
    point3D* posll; 
    point3D* posrl; 
    point3D* posb; 
    point3D* posla; 
    point3D* posra; 
    point3D* posh; 
    point3D* posbullet; 
  
    //box object variables 
    Box* bullet; 
    Box* bodyParts [6]; 
    //Position and direction vector as a point3D object 
    point3D *posFighter; 
    point3D *dirVec; 
  
    //variables 
    float gravity; 
    float *shieldSize; 
    float *hitPoints; 
    //Properties 
    bool *faceRight; 
    bool *bulletState; 
    bool *coliHit; 
    bool *Victory; 
  
    //Colour 
    colourStruc* color; 
  
    //Animations 
    Animation *aniState; 
  
    //Idle states 
    int idle,idle2; 
    int *jumpCount; 
    int *shieldState; 
  
    //Constructor 
    Fighter(); 
    Fighter(point3D* d,bool* faceRight); 
    Fighter(point3D* d); 
    Fighter *enefig; 
  
    //Methods 
    void drawBox(); 
    void drawWire(); 
    void drawBoxNoAngle(); 
    void drawWireNoAngle(); 
    void animate(); 
    void action(int d); 
    void applyGrav(); 
    void updateBodyParts(); 
    void moveFighter(float x, float y, float z); 
    void changeLocation(point3D *point); 
    void updateInfo(Fighter* asdf,int* shieldstate, float* shieldsize); 
    void collisionCheck(); 
    void aniBullet(); 
    void activateBullet(); 
    void displaybullet(); 
}; 