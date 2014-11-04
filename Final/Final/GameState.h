/*  
Authors: Rhett Amin, Kevin Ly, David Elsonbaty
Student Numbers: 1060085, 1144604 
File: GameState.h 
----------------------------- 
Header class for GameState.cpp
This class handles selection of characters
*/

#ifndef __Assignemnt3__GameState__
#define __Assignemnt3__GameState__

#include <iostream>
#include "MathLib3D.h"

class GameState
{
private:
    
    int setNum;
	float screenX, screenY;
	bool textDrawn;
    bool isIntroAnimationOn;
    bool isToonPicking;
	
	void setSelection(int index);
	void drawStrokeText(char* text, int x, int y);
    
public:
	int getSetNum();
	bool getIsIntroAnimationOn();
    bool getIsToonPicking();

    GameState(float screenSizeX, float screenSizeY);
	colourStruc* getFighterColour(int pos);
    void drawStatusToons();
    void drawPickingToons();
    void createPickingSquares();
    void checkHit(point3D mouseHit);
    void setIsIntroAnimation();
};

#endif /* defined(__Assignemnt3__GameState__) */