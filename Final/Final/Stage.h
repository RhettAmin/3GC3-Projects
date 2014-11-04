/*  
Authors: Rhett Amin, Kevin Ly, David Elsonbaty
Student Numbers: 1060085, 1144604 
File: Stage.h
----------------------------- 
Header Class for Stage.cpp
*/

#include <vector>
#include <GL/glut.h>
#include <GL/gl.h>
#include "Platform.h"
#include "Fighter.h"

using namespace std;

class Stage
{
private:    
    struct Material;
    
	//Textures
    GLuint textures[2];
    GLubyte* image[2];
    int texWidth[2];
    int texHeight[2];
    int texMax[2];

	int *F1Shield;
	int *F2Shield;
    void loadTextures();
	void drawBackground();
	void createFighters();
	void makePlatform();

public:
	Platform plat;
    Stage(void);
	Stage(int *shieldstatep1, int *shieldstatep2);
	bool getVicStatus();
    void createStage();
    void drawStage();
	void animFighters();
	void drawFighters();
	void drawBullet();
	void actFighter1(int num);
	void actFighter2(int num);
	void moveFighter1(float z);
	void moveFighter2(float z);
	void setFighterColour(int player, colourStruc* colour);
	void setFighterFace(int player, bool isRight);
	void callFighterUpdate();
	void activateBullet(int player);
	void setFighterColour(colourStruc* c1, colourStruc* c2);
};

