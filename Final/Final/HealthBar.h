/*  
Authors: Rhett Amin, Kevin Ly, David Elsonbaty
Student Numbers: 1060085, 1144604 
File: HealthBar.h 
----------------------------- 
Header class for HealthBar.cpp
Class draws the healthbar
*/

#ifndef __Assignemnt3__HealthBar__
#define __Assignemnt3__HealthBar__

#include <iostream>
#include "MathLib3D.h"

class HealthBar{
private:
    float maxHealth;
    float depth;
    point3D location;
public:
    float *health;
    HealthBar(point3D loc, float* healthIN);
    void drawHealthBar(bool IsleftAligned);
};


#endif /* defined(__Assignemnt3__HealthBar__) */
