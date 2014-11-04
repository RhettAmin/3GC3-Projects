/*  
Authors: Rhett Amin, Kevin Ly, David Elsonbaty
Student Numbers: 1060085, 1144604 
File: Main.cpp 
----------------------------- 
This a simple fighting 2D fighting game akin to Super Smash Bros.
This is a 2-player game where each player controls there own fighter.
Each player can run, jump, punch, kick, shoot, and block their way to victory.
*/

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <vector>
#include <time.h>
#include <string>
#include <iostream>
#include "MathLib3D.h"
#include "Stage.h"
#include "GameState.h"
#include "HealthBar.h"
#include "Fighter.h"

#define ESCAPE 27
#define SPACE 32

using namespace std;

//screen size, and aspect ratio for gluPerspective
float screenSizeX = 800;
float screenSizeY = 600;
float aspect = (screenSizeX)/(screenSizeY);

//Camera Position
point3D cameraEyePosition = point3D(0,2,30);

//Raycasting
GLint view[4];
GLdouble proj[16];
GLdouble modelview[16];
point3D GetPosition(int x, int y);

//booleans
bool isCamSet = false;
bool animateCam = false;

//Fighter shield states - chekc if they are blocking
int *shieldstatep1 = new int(0);
int *shieldstatep2 = new int(0);

//keypress
int keys[256] = {0};

//Pausing
bool isPaused;

//CountDown
bool didStartGame = false;
int countDownStage = 0;
int numOfTimesLeft = 500;

//Init class objects
colourStruc* t1;
colourStruc* t2;
GameState gState = GameState(screenSizeX, screenSizeY);
Stage stage = Stage(shieldstatep1, shieldstatep2);

//Light struc
struct Light {
	float pos[4];
	float amb[4];
	float diff[4];
	float spec[4];
};
//Vectors
vector<Light> lArray;
vector<colourStruc*> colourVec;

/* addLights()
	Adds lights to the scene and draws spheres around them.
*/
void addLights() {
    glLightfv(GL_LIGHT0, GL_POSITION, lArray.at(0).pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  lArray.at(0).amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lArray.at(0).diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lArray.at(0).spec);
    
    glLightfv(GL_LIGHT1, GL_POSITION, lArray.at(1).pos);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  lArray.at(1).amb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  lArray.at(1).diff);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lArray.at(1).spec);
    
    for (int l = 0; l < 2; l++) {
        
        glPushMatrix();
			glEnable(GL_COLOR_MATERIAL);
        
				glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
				glColor4f(1, 1, 0, 1);
				float mat_specular[] = {0, 0, 0, 1.0};
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
        
				glTranslatef(lArray.at(l).pos[0], lArray.at(l).pos[1], lArray.at(l).pos[2]);
				glutSolidSphere(0.2, 5, 5);
        
				glColor4f(1.0, 1.0, 1.0, 1.0);
			glDisable(GL_COLOR_MATERIAL);
        glPopMatrix();
        
    }
}

/* createLights()
	Creates lights to be drawn in te scene
*/
void createLights() {
	Light light0 = {
		{0, 20, 5, 1},
		{1, 1, 1, 1},
		{1, 1, 1, 1},
		{1, 1, 1, 1}
	};
    
	Light light1 = {
		{5, 15, 35, 1},
		{1, 1, 1, 1},
		{1, 1, 1, 1},
		{1, 1, 1, 1}
	};
    
	lArray.push_back(light0);
	lArray.push_back(light1);
}

/* introAnimation()
	Animates the camera towards the platform
*/
void introAnimation(){
    if (cameraEyePosition.z > 35) {
        cameraEyePosition.z -= 0.1;
    }else{
        if (cameraEyePosition.y > 9) {
            cameraEyePosition.y -= 0.1;
        }else{
			gState.setIsIntroAnimation(); // set animation done(false)
        }
    }
}

/* drawStrokeText()
	Draws text to the screen spcified at the x and y locations
	Modified version given by Teather, extra bits from:
	http://stackoverflow.com/questions/9430852/glutbitmapcharacter-positions-text-wrong
*/
void drawStrokeText(char* text, int x, int y) {
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix() ;
	glLoadIdentity();
	gluOrtho2D(0, screenSizeX, 0, screenSizeY);
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

/* GetPosition(int x, int y)
 *	IDEA FROM http://nehe.gamedev.net/article/using_gluunproject/16013/
 *   Gets the position of the mouse click onto the screen
 */
point3D GetPosition(int x, int y){
	glGetIntegerv(GL_VIEWPORT, view);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
    
	//ScreenToClient(hWnd, mouse);
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;
	winX = (float) x;
	winY = (float)view[3]- y;
	glReadPixels(winX, winY, 1,1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	
	gluUnProject(winX, winY, winZ, modelview, proj, view, &posX, &posY, &posZ);
	return point3D(posX, posY, posZ);
}

/* display() - the OpenGL display function, this draws the screen
 *	
 */
void display()
{
    //clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Set MatrixMode to Modelview for adding and updating objects.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraEyePosition.x, cameraEyePosition.y, cameraEyePosition.z, 0, 2, 0, 0, 1, 0);
    
    //Whole System
    glPushMatrix();
		addLights();
    
		//If players have picked their characters do intro zooming animation
		if (gState.getIsIntroAnimationOn() == true && gState.getIsToonPicking() == false) {
			if (!isCamSet) {
				cameraEyePosition = point3D(0, 10, 100);
				stage.setFighterColour(gState.getFighterColour(0),gState.getFighterColour(1));
				isCamSet = true;
			}
			introAnimation();
		}
		
		//Main displaying
		if (gState.getIsToonPicking()) {
			gState.drawPickingToons(); //Picking Character Screen
		}else{
			//Main Scene
			stage.drawStage();
			gState.drawStatusToons(); //draw little fighters next to healthbars

			stage.drawFighters();
			stage.drawBullet(); // draw bullets if created(handled by function)

			//Display pause text
			if (isPaused) {
				drawStrokeText("Paused", screenSizeX/2.25, screenSizeY/2);
			}

			//Countdown before fight
			if (!didStartGame) {
				glEnable(GL_COLOR_MATERIAL);
					glColor4f(1, 1, 1, 1);
					switch (countDownStage) {
						case 0:
							drawStrokeText("3", screenSizeX/2, screenSizeY/2);
							break;
						case 1:
							drawStrokeText("2", screenSizeX/2, screenSizeY/2);
							break;
						case 2:
							drawStrokeText("1", screenSizeX/2, screenSizeY/2);
							break;
						case 3:
							drawStrokeText("FIGHT!", screenSizeX/2.25, screenSizeY/2);
							break;
						case 4:
							didStartGame = true;
							break;
						default:
							break;
					}
				glDisable(GL_COLOR_MATERIAL);
				
				//Handles when to change countdown text
				numOfTimesLeft --;
				if (numOfTimesLeft == 0) {
					countDownStage ++;
					numOfTimesLeft = 500;
				}
			}

			//Displays 'Victory' text
			if (stage.getVicStatus()){
				glPushMatrix();
				glEnable(GL_COLOR_MATERIAL);
					glColor4f(1, 1, 1, 1);
					glScalef(2, 2, 2);
					drawStrokeText("VICTORY!", screenSizeX/2.5, screenSizeY- 50);
				glDisable(GL_COLOR_MATERIAL);
				glPopMatrix();
			}
		}

    glPopMatrix();//End System
    
    //swap buffers - rendering is done to the back buffer, bring it forward to display
    glutSwapBuffers();
    //force a redisplay, to keep the animation running
    glutPostRedisplay();
}

/* kbd -- the GLUT keyboard function
 *  key -- the key pressed
 *  x and y - mouse x and y coordinates at the time the function is called
 */
void kbd(unsigned char key, int x, int y)
{
	if (key == SPACE) {
		if (didStartGame){ // check if countdown is occuring (don't pause)
			isPaused = !isPaused;
		}
	}
	if (key == ESCAPE) {
		exit(0);
	}

	if (!isPaused){
		if (didStartGame) {
			//Don't want shield to flicker on and off
			if (keys[key] == 0) {
				keys[key] = 1;
				if ( key == 's') { //don't shield with other animations
					keys[key] = 0;
				}
				if ( key == '5') { //don't shield with other animations
					keys[key] = 0;
				}
			}
		
			switch(key) {
		
				case 's':
					if(*shieldstatep1 == 0){
						*shieldstatep1 = 1;
					}else {
						*shieldstatep1 = 0;
					}
				break;
		
				case '5':
					if(*shieldstatep2 == 0){
						*shieldstatep2 = 1;
					}else {
						*shieldstatep2 = 0;
					}
					break;
			}
		}
	}
}

/*kbd -- the GLUT keyboard function
	check if keys are not being pressed
*/
void kbdUP (unsigned char key, int x, int y) {
	if (!isPaused){
		keys[key] = 0;
	}
}

/* Special keyboard function */
void specialKeysFunction(int key, int x, int y) {
	if (didStartGame) { 
		switch(key) {
			case GLUT_KEY_UP :
				//Increase Camera Eye Position
				cameraEyePosition.y++;
				break;
			case GLUT_KEY_DOWN :
				//Decrease Camera Eye Position
				cameraEyePosition.y--;
				break;
            case GLUT_KEY_LEFT:
				//Decrease Camera Eye Position
				cameraEyePosition.x--;
				break;
            case GLUT_KEY_RIGHT:
				//Increase Camera Eye Position
				cameraEyePosition.x++;
				break;	
		}
	}
}

/* mouse(int btn, int state, int x, int y)
 *	Mouse function for selecting and deleting
 */
void mouse(int btn, int state, int x, int y){
	/* Selects item using the left button */
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		point3D mouseHit = GetPosition(x, y);
		gState.checkHit(mouseHit);
	}
}

/* timer(int msec)
 *	handles rate of animation of fighters
 */
void timer(int msec){
	if (!isPaused) {
		stage.animFighters();
	}
	glutTimerFunc(33, timer, 0);
}

/* idle()
 *	handles events to occur if certain keys are pressed
 */
void idle() {

	if (!isPaused) {
		// Player 1
		if (keys['w']) {
			stage.actFighter1(0);
		}
		if (keys['f']) {
			stage.actFighter1(1);
		}
		if (keys['g']) {
			stage.actFighter1(2);
		}
		if (keys['h']) {
			stage.actFighter1(3);
			stage.activateBullet(0);
		}
		if (keys['d']) {
			stage.setFighterFace(0, true);
			stage.moveFighter1(0.025);
		}
		if(keys['a']) {
			stage.setFighterFace(0, false);
			stage.moveFighter1(-0.025);
		}

		//player 2
		if (keys['8']) {
			stage.actFighter2(0);
		}
		if (keys['[']) {
			stage.actFighter2(1);
		}
		if (keys[']']) {
			stage.actFighter2(2);
		}
		if (keys['p']) {
			stage.actFighter2(3);
			stage.activateBullet(1);
		}
		if (keys['4']) {
			stage.setFighterFace(1, false);
			stage.moveFighter2(-0.025);
		}
		if (keys['6']) {
			stage.setFighterFace(1, true);
			stage.moveFighter2(0.025);
		}

	}else {//Sets all keys to not pressed when paused
		for(int i = 0; i < 256; i++) {
			keys[i] = 0;
		}
	}
}

/* displayPrompt()
	Function to display commands to console
*/
void displayPrompt(){ 
    cout << "Welcome to the Final Boxination \n This is a 3D platforming fighting game using openGL, and FreeGlut \n CS3GC3 - SEPT TERM 2013 ; KEVIN LY; RHETT AMIN; DAVID E; \n\n"; 
    cout << "Objective:\nReduce enemey hitpoints\nKnock them off stage\nWIN.\n\n"; 
	cout << "Choose your fighter with the mouse \n\n";
    cout << "Controls:\nPLAYER1\nW - JUMP\nA - LEFT\nD - RIGHT\nS - TOGGLE SHIELD\n\nF - PUNCH\nG - KICK\nH - SHOOT\n\n" ; 
    cout << "\nPLAYER 2 - NUMPAD MUST BE ENABLED\n8 - JUMP\n2 - LEFT\n4 - RIGHT\n3 - TOGGLE SHIELD\n\n[ - PUNCH\n] - KICK\nP - SHOOT\n\nSYSTEM CONTROLS\n\nSPACEBAR - PAUSE\nESC - QUITS\n"; 
  
} 

/* main(int argc, char** argv)
 *   initialize all glut initization.
 */
int main(int argc, char** argv)
{
    srand(time(NULL));
    
    //Display commands to command window
    //displayReadMe();
    
    //glut initialization stuff:
    // set the window size, display mode, and create the window
    glutInit(&argc, argv);
    glutInitWindowSize(screenSizeX, screenSizeY);
    glutInitWindowPosition(450,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Final Game");
    
    //enable Z buffer test, otherwise things appear in the order they're drawn
    glEnable(GL_DEPTH_TEST);
    
    //setup the initial view
    // change to projection matrix mode, set the extents of our viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, aspect, 1, 300);
    
    //set background colour to grey
    glClearColor(0, 0, 0, 0);
    
    //lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	createLights();
    
    //enable culling
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    //register glut callbacks for keyboard and display function
    glutKeyboardFunc(kbd);
	glutKeyboardUpFunc(kbdUP);
    glutSpecialFunc(specialKeysFunction);
    glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

	glutIgnoreKeyRepeat(true); //Set keys don't repeat if held down
	
    gState.createPickingSquares(); // create boxes to be selected
    
	//create stage
	stage.createStage();
	stage.callFighterUpdate();

	//call animation timer
	glutTimerFunc(33, timer, 0);

	displayPrompt();

    //start the program!
    glutMainLoop();
    
    return 0;


}