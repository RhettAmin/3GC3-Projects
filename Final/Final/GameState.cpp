/*  
Authors: Rhett Amin, Kevin Ly, David Elsonbaty
Student Numbers: 1060085, 1144604 
File: GameState.cpp 
----------------------------- 
This class handles selection of characters
*/

#include <vector>
#include <GL/glut.h>
#include <GL/gl.h>
#include "GameState.h"
#include "Fighter.h"

using namespace std;

struct BB {
	point3D BTR; //Back Top Right
	point3D FBL; //Front Bottom Left
};

//Vectors
//bbArrays -> for displaying the characters
vector<BB> bbArray;
vector<Fighter*> bbFighterArray;

//statusArrays-> for shifting the fighters to the bottom of the screen
vector<BB> statusArray;
vector<Fighter*> statusFighterArray;

//used to transfer colour over to Stage.cpp
colourStruc* fighter1Colour;
colourStruc* fighter2Colour;

/*Constructor
	Takes screen size for drawing text
*/
GameState::GameState(float screenSizeX, float screenSizeY){
    
    setNum = 0;
	isIntroAnimationOn = false;
	isToonPicking = true;

	screenX = screenSizeX;
	screenY = screenSizeY;
	textDrawn = false;
}

/*createPickingSquares()
	creates the boxes and fighters and sets their location
*/
void GameState::createPickingSquares() {
	
    BB ps1, ps2, ps3, ps4;
    
	ps1.FBL = point3D(-7, 1, 20);
	ps1.BTR = point3D(-5, 3, 18);
    
	ps2.FBL = point3D(-3, 1, 20);
	ps2.BTR = point3D(-1, 3, 18);
    
	ps3.FBL = point3D(1, 1, 20);
	ps3.BTR = point3D(3, 3, 18);
    
	ps4.FBL = point3D(5, 1, 20);
	ps4.BTR = point3D(7, 3, 18);
	
	bbArray.push_back(ps1);
	bbArray.push_back(ps2);
	bbArray.push_back(ps3);
	bbArray.push_back(ps4);
    
	Fighter *fighter1 = new Fighter(new point3D(-15, 2, 48));
    fighter1->color = new colourStruc(1,0,0);
    Fighter *fighter2 = new Fighter(new point3D(-5, 2, 48));
    fighter2->color = new colourStruc(0,1,0);
    Fighter *fighter3 = new Fighter(new point3D(5, 2, 48));
    fighter3->color = new colourStruc(0,0,1);
    Fighter *fighter4 = new Fighter(new point3D(15, 2, 48));
    fighter4->color = new colourStruc(1,1,0);
    
	bbFighterArray.push_back(fighter1);
	bbFighterArray.push_back(fighter2);
	bbFighterArray.push_back(fighter3);
    bbFighterArray.push_back(fighter4);
}

/*drawPickingToons()
	Displays text telling user what to do
	Draws the fighters to be picked
*/
void GameState::drawPickingToons() {
	if (setNum == 0) {
		drawStrokeText("Choose your character Player 1", screenX/6, screenY - 50);
	}else{
		drawStrokeText("Choose your character Player 2", screenX/6, screenY - 50);
	}
	
    for (int i = 0; i < bbArray.size(); i++) {
        glPushMatrix();
			//Draw Fighters
			glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
					glColor4f(bbFighterArray.at(i)->color->r,bbFighterArray.at(i)->color->g,bbFighterArray.at(i)->color->b, 0.5);
					glScalef(0.4, 0.4, 0.4);
					bbFighterArray.at(i)->drawBoxNoAngle();
				glDisable(GL_COLOR_MATERIAL);
			glPopMatrix();
		glPopMatrix();
	}
}

/*createStatusToons()
	creates the boxes and fighters that are drawn below the stage.
*/
void GameState::drawStatusToons() {
	
    for (int i = 0; i < bbArray.size(); i++) {
		
        glPushMatrix();
			glEnable(GL_COLOR_MATERIAL);
        
				glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
				glColor4f(0.2, 0.15, .6, 1);
				float mat_specular[] = {0, 0, 0, 1.0};
				float mat_emission[] = {0.2, 0.2, 0.4, 1.0};
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
				glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, mat_emission);
        
				switch (i) {
					case 0:
						glTranslatef(bbArray.at(i).FBL.x - 2, bbArray.at(i).FBL.y, bbArray.at(i).FBL.z);
						break;
					case 1:
						glTranslatef(bbArray.at(i).FBL.x + 2, bbArray.at(i).FBL.y, bbArray.at(i).FBL.z);
						break;
					default:
						break;
				}
        
				glutWireCube(3);

				//Draw Fighters
				glPushMatrix();
					glColor4f(bbFighterArray.at(i)->color->r, bbFighterArray.at(i)->color->g, bbFighterArray.at(i)->color->b, 0.5);
					glTranslatef(0,-0.4, 0);
					glScalef(0.5, 0.5, 0.5);
					bbFighterArray.at(i)->drawBoxNoAngle();
				glPopMatrix();

			glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();
	}
    
}

/* drawStrokeText()
	Draws text to the screen spcified at the x and y locations
	Modified version given by Teather, extra bits from:
	http://stackoverflow.com/questions/9430852/glutbitmapcharacter-positions-text-wrong
*/
void GameState::drawStrokeText(char* text, int x, int y) {
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix() ;
	glLoadIdentity();
	gluOrtho2D(0, screenX, 0, screenY);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix() ;
	glLoadIdentity();

	glDisable( GL_DEPTH_TEST ) ;

	glTranslatef(x, y, 0);
	glScalef(0.25, 0.25, 0.25);
	
	char* c;  //character iterator (ptr)
	for(c = text; *c != '\0'; c++) //stop when we hit null character
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c); //print one char
	}

	glEnable( GL_DEPTH_TEST ) ;
	glMatrixMode( GL_PROJECTION ) ;
	glPopMatrix() ; // revert back to the matrix I had before.
	glMatrixMode( GL_MODELVIEW ) ;
	glPopMatrix() ;
}

/*setSelection()
	Handles selection of characters and transfers fighters from arrays
*/
void GameState::setSelection(int index) {
	
    if (setNum == 0) {
		//Move bounding box
        bbArray.at(index).FBL = point3D (-18, -12, 0);
		bbArray.at(index).BTR = point3D (-16, -10, -2);
        
		//Move Fighter
        bbFighterArray.at(index)->changeLocation(new point3D(-42,-30,0));

		//set Colour of fighter 1
		fighter1Colour = bbFighterArray.at(index)->color;
		
		//Add bounding box and fighter to new array
		statusArray.push_back(bbArray.at(index));
        statusFighterArray.push_back(bbFighterArray.at(index));

		setNum++;
        
	} else if (setNum == 1) {
		//Move bounding box
		bbArray.at(index).FBL = point3D (18, -12, 0);
		bbArray.at(index).BTR = point3D (20, -10, -2);
       
		//Move Fighter
		bbFighterArray.at(index)->changeLocation(new point3D(42,-30,0));

		//set Colour of fighter 2
		fighter2Colour = bbFighterArray.at(index)->color;
        
		//Add bounding box and fighter to new array
		statusArray.push_back(bbArray.at(index));
        statusFighterArray.push_back(bbFighterArray.at(index));
		
		//clear array
		bbArray.clear();
		bbFighterArray.clear();
        
		//Move status fighters to under the platform
		for (int i = 0; i < statusArray.size(); i++) {
            statusFighterArray.at(i)->changeLocation(new point3D(0,-2,0));

			bbArray.push_back(statusArray.at(i));
            bbFighterArray.push_back(statusFighterArray.at(i));
		}

        statusArray.clear();
        statusFighterArray.clear();

		//used to transfer to introAnimation() in Main.cpp
		isIntroAnimationOn = true;
		isToonPicking = false;
	}
}

/*checkHit()
	Check if mouse click hits a box
*/
void GameState::checkHit(point3D mouseHit) {
	for (int i = 0; i < bbArray.size(); i++) {
		if ( ((bbArray.at(i).FBL.x < mouseHit.x) && (mouseHit.x < bbArray.at(i).BTR.x)) &&
            ((bbArray.at(i).FBL.y < mouseHit.y) && (mouseHit.y < bbArray.at(i).BTR.y))){
			setSelection(i);
		}
	}
}

/*setIsIntroAniamtion()
	set isIntroAnimation to false, so it will not animate
*/
void GameState::setIsIntroAnimation() {
	isIntroAnimationOn = false;
}


/*getSetNum()
	get number of selection
*/
int GameState::getSetNum() {
	return setNum;
}

/*getisIntroAnimation()
	get bool checking if isIntroAniamtion is true
*/
bool GameState::getIsIntroAnimationOn() {
	return isIntroAnimationOn;
}

/*getIsToonPicking()
	get bool checking if isToonPicking is true
*/
bool GameState::getIsToonPicking() {
	return isToonPicking;
}

/*getgetFighterColour()
	returns colours of fighters picked
*/
colourStruc* GameState::getFighterColour(int pos) {
	if (pos == 0) {
		return fighter1Colour;
	}else{
		return fighter2Colour;
	}
}
