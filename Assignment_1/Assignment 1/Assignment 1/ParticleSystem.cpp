#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "Mathlib2D.h"
#include <time.h>
#define ESCAPE 27
#define SPACEBAR 32

using namespace std;

	/*=============================================================================================================
	 * Global Variables and Function Blueprints
	 *=============================================================================================================
	*/
	//Blueprints
	struct Particle;
	struct Blackhole;
	void display();
	void createParticles(float x, float y);
	void genInitParts();
	colourStruc randColour();
	void kbd();
	void mouse();
	void motion();
	void reset();

	//Global Variables
	float screenSizeX = 600;	// holds window size
	float screenSizeY = 600;
	float gMouseX;				// mouse cordinates
	float gMouseY;
	float mMouseX;				// mirror mouse positions
	float mMouseY;
	float maxRange = 500;		// max range of attraction
	float maxSpeed = 10;		// max speed of movement
	int initParticleNum = 500; // initial particles on screen
	bool paused = false;		// booleans to handle certain events
	bool LMBDown = false;
	bool RMBDown = false;
	bool mirrorMode = false;
	bool isParty = false;
	bool cBlackhole = false;

	//Arrays
    vector<Particle> particleArray;
	vector<Blackhole> bhArray;


	/*=============================================================================================================
	 * Functions
	 *=============================================================================================================
	*/

/* Particle Structure */
struct Particle {
	// Particle characteristics 
	point2D position;
	colourStruc color;
	float range;
	float speed;
	int size;
	vec2D direction;
	
	//updates the particles - velocity, bouncing, warping
	void updateParticle() {
		// slow down the particle after attraction or repelling
		direction = MathLib2D::vectorMultiply(direction, 0.99);
		position = MathLib2D::movePoint(position,direction);
		
		//bouncing and warping
		if (position.y < 0) {
			direction.y = direction.y * -1; //bounce at the bottom
		}
		if (position.y > screenSizeY) {
			direction.y = direction.y * -1; //bounce at the top
		}
		if (position.x > screenSizeX) {
			position.x = 0; // warp
		}
		if (position.x < 0) {
			position.x = screenSizeX; //warp
		}
	}
};

struct Blackhole {
	point2D position;
	colourStruc color;
};

/* Generate random colour */
colourStruc randColour() {
	float r = ((float)rand()/(float)RAND_MAX) * (1 - 0);  // 1 is max value, 0 is min
	float g = ((float)rand()/(float)RAND_MAX) * (1 - 0);
	float b = ((float)rand()/(float)RAND_MAX) * (1 - 0);

	return colourStruc(r,g,b);
}

/* Displays program commands to the console */
void consoleCommands() {
	printf("CS3GC3\nAssignment 1\nRhett Amin\nOct-4-13\n\n");
	printf("Welcome to my Particle Flocker System!\nHere is a list of commands that will affect the program \n\n");
	printf("Requirements: \n\n");
	printf("%-15s %-75s\n"," 'LMB'      ---","Attract particles towards the mouse");
	printf("%-15s %-75s\n"," 'RMB'      ---","Detract particles away the mouse");
	printf("%-15s %-75s\n"," 'a'        ---","Add a particle to the screen at the mouse position");
	printf("%-15s %-75s\n"," 'd'        ---","Delete a particle closest to the mouse");
	printf("%-15s %-75s\n"," '+'/'-'    ---","Increase/Decrease range of attraction/repelling");
	printf("%-15s %-75s\n"," ']'/'['    ---","Increase/Decrease speed of attraction/repelling");
	printf("%-15s %-75s\n"," 'spacebar' ---","Pause the simulation");
	printf("%-15s %-75s\n"," 'r'        ---","Reset the simulation");
	printf("%-15s %-75s\n"," 'ESC'/'q'  ---","Exit the simulation");
	printf("\nExtra Commands \n");
	printf("%-15s %-75s\n"," 'c'        ---","Clears the screen");
	printf("%-15s %-75s\n"," 'x'        ---","Rotates the particles around the mouse depending on the");
	printf("%-15s %-75s\n","","global range. Try attracting/repelling while holding");
	printf("%-15s %-75s\n","","down the button.");
	printf("%-15s %-75s\n"," 'm'        ---","Creates a secondary cursor reflected along the x-axis");
	printf("%-15s %-75s\n"," 'n'        ---","Display the number of particles on the screen");
	printf("%-15s %-75s\n"," 'p'        ---","Randomizes particle colour when action occurs");
	printf("%-15s %-75s\n"," 'b'        ---","Creates a blackhole that absorbs any particles"); 
	printf("%-15s %-75s\n","","drawn. Try holding down the button and moving the cursor around");
}

/* Creates the initial particles*/
void genInitParts() {
	for (int i = 0; i < initParticleNum; i++) {
		createParticles(rand() % (int)screenSizeX + 1, rand() % (int)screenSizeY + 1);
	}
};

/* Create a particle*/
void createParticles(float x, float y) {
	//create particle object
	Particle particle;

	//set positiob from function inputs
	particle.position = point2D(x, y);
	
	//create random colour
	particle.color = colourStruc(0,0,1);

	//generate random size
	int randSize = rand() % 10 + 3;
	particle.size = randSize;

	//generate random range
	float randRange = ((float)rand()/(float)RAND_MAX) * (250 - 50) + 50;
	particle.range = (float)randRange;

	//Initialize speed
	int randSpeed = rand() % 1 + 6;
	particle.speed = randSpeed;
	
	//Initialize direction
	particle.direction = vec2D(0,0);

	//add particle to particleArray
	particleArray.push_back(particle);	
}

/* Delete a particle */
void deleteParticle(float mouseX, float mouseY) {
	if (!particleArray.empty()) {
		float fDelete = 100000; //float holding closest distance number (absurd number for formula)
		int pDelete; //point to delete

		for (int i = 0; i < particleArray.size(); i++) {
			float tempDist = MathLib2D::pDistance(point2D(mouseX,mouseY), point2D(particleArray.at(i).position.x,particleArray.at(i).position.y));

			//replaces closest distance and particle to delete
			if (tempDist < fDelete) {
				fDelete = tempDist;
				pDelete = i;
			}
		}
		//remove particle from array
		particleArray.erase(particleArray.begin() + pDelete);
	}
}

/* Increase global range of attraction/detraction */
void increaseRange() {
	for(int i = 0; i < particleArray.size(); i++) {
		if (particleArray.at(i).range >=  maxRange) {
			particleArray.at(i).range = maxRange;
		} else {
			particleArray.at(i).range += 10;
		}
	}
}

/* Decrease global range of attraction/detraction */
void decreaseRange() {
	for(int i = 0; i < particleArray.size(); i++) {
		if (particleArray.at(i).range <= 0) {
			particleArray.at(i).range = 0;
		} else {
			particleArray.at(i).range -= 10;
		}
	}
}

/* Increase global speed of movement */
void increaseSpeed() {
	for (int i = 0; i < particleArray.size(); i++) {
		if (particleArray.at(i).speed >  maxSpeed) {
			particleArray.at(i).speed = maxSpeed;
		} else {
			particleArray.at(i).speed++;
		}
	}
}

/* Decrease global speed of movement */
void decreaseSpeed() {
	for (int i = 0; i < particleArray.size(); i++) {
		
		if (particleArray.at(i).speed < 0 ) {
			particleArray.at(i).speed = 0;
		} else {
			particleArray.at(i).speed--;
		}
	}
}

/* Attract/Detract particles to/from mouse */
void moveParticle(float mouseX, float mouseY, bool toAttract) {
	int attractVar;
	// -1 to attract, 1 to retract
	if (toAttract == true) {
		attractVar = -1;
	}
	else if (toAttract == false) {
		attractVar = 1;
	}
	//iterate through particle array
	for (int i = 0; i < particleArray.size(); i++) {
		
		//calculate distance from particle to mouse
		float dist = MathLib2D::pDistance(point2D(mouseX,mouseY),particleArray.at(i).position);
		
		//if distance is <= particle range move particle closer to mouse
		if (dist <= particleArray.at(i).range) {
			//create vector between particle and mouse
			vec2D vecConnect = MathLib2D::createVector(point2D(mouseX,mouseY), particleArray.at(i).position); 
			// normalize vector, and multiply the vector with speed to attract or detract
			particleArray.at(i).direction = MathLib2D::vectorMultiply(MathLib2D::normalize(vecConnect), attractVar * particleArray.at(i).speed); 
			//move particle
			particleArray.at(i).position = MathLib2D::movePoint(point2D(particleArray.at(i).position.x, particleArray.at(i).position.y) , particleArray.at(i).direction); 
		}
	}
}

/* Rotate particles around the mouse */
void rotateParticle(float mouseX, float mouseY) {
	mouseY = screenSizeY - mouseY;
	for (int i = 0; i < particleArray.size(); i++) {

		float distance = MathLib2D::pDistance(point2D(mouseX, mouseY), particleArray.at(i).position);

		// check if particles are within range to rotate
		if (distance < particleArray.at(i).range) {

			//move the paticles along a circle around the mouse
			float pointX = cos(1.f) * (particleArray.at(i).position.x - mouseX) - sin(1.f) * (particleArray.at(i).position.y - mouseY) + mouseX;
			float pointY = sin(1.f) * (particleArray.at(i).position.x - mouseX) + cos(1.f) * (particleArray.at(i).position.y - mouseY) + mouseY;
		
			particleArray.at(i).position = point2D (pointX,pointY);
		}
	}
}

/* Create a black hole at the ceter of the screen*/
void createBlackhole() {
	bhArray.at(0).color = randColour();
	glColor3f(bhArray.at(0).color.r, bhArray.at(0).color.g, bhArray.at(0).color.b);
	
	//create blackhole
	glBegin(GL_POLYGON);
	glVertex2f(bhArray.at(0).position.x - 15, bhArray.at(0).position.y - 15);
	glVertex2f(bhArray.at(0).position.x + 15, bhArray.at(0).position.y - 15);
	glVertex2f(bhArray.at(0).position.x + 20, bhArray.at(0).position.y);
	glVertex2f(bhArray.at(0).position.x + 15, bhArray.at(0).position.y + 15);
	glVertex2f(bhArray.at(0).position.x - 15, bhArray.at(0).position.y + 15);
	glVertex2f(bhArray.at(0).position.x - 20, bhArray.at(0).position.y);
	glEnd();
}

void blackholeEffect(int i) {
	createBlackhole();
	// rotate particles around blackhole
	float pointX = cos(5.f) * (particleArray.at(i).position.x - bhArray.at(0).position.x) - sin(5.f) * (particleArray.at(i).position.y - bhArray.at(0).position.y) + bhArray.at(0).position.x;
	float pointY = sin(5.f) * (particleArray.at(i).position.x - bhArray.at(0).position.x) + cos(5.f) * (particleArray.at(i).position.y - bhArray.at(0).position.y) + bhArray.at(0).position.y;
	particleArray.at(i).position = point2D (pointX,pointY);
	
	// move particles closer to blackhole
	vec2D vecConnect = MathLib2D::createVector(point2D(bhArray.at(0).position.x,bhArray.at(0).position.y), particleArray.at(i).position); //create vector between point and blackhole
	particleArray.at(i).direction = MathLib2D::vectorMultiply(MathLib2D::normalize(vecConnect), -1); // attract
	particleArray.at(i).position = MathLib2D::movePoint(point2D(particleArray.at(i).position.x, particleArray.at(i).position.y) , particleArray.at(i).direction); //move closer
	
	if (particleArray.at(i).position.x <= bhArray.at(0).position.x + 0.7 && particleArray.at(i).position.x >= bhArray.at(0).position.y - 0.7) {
		deleteParticle(bhArray.at(0).position.x, bhArray.at(0).position.y);
	}
}

/* Create cursor for mirrored mouse coordinates */
void createMirrorCursor() {
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f(1, 1, 1);
	glVertex2f(mMouseX - 5, mMouseY - 5);
	glVertex2f(mMouseX - 5, mMouseY + 5);
	glVertex2f(mMouseX + 5, mMouseY + 5);
	glVertex2f(mMouseX + 5, mMouseY - 5);
	glEnd();
}

/*  display particles in the array */
void displayParticles(int i) {
	glEnable(GL_POINT_SMOOTH);
	glPointSize(particleArray.at(i).size);
	glBegin(GL_POINTS);
	if (isParty) {
		particleArray.at(i).color = randColour();
	}
	glColor3f(particleArray.at(i).color.r, particleArray.at(i).color.g, particleArray.at(i).color.b);
	glVertex2f(particleArray.at(i).position.x, particleArray.at(i).position.y);
	glEnd();
}

/* Reset the program */
void reset() {
	mirrorMode = false;
	isParty = false;
	cBlackhole = false;
	particleArray.clear();
	genInitParts();
}

/* display function - GLUT display callback function
 *		clears the screen, draws a square, and displays it
 */
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// draw the black hole
	if (cBlackhole) {
		createBlackhole();
	}
	
	for (int i = 0; i < particleArray.size(); i++) {
		
		//call the blackhole effect
		if (cBlackhole) {
			blackholeEffect(i);
		}

		//update particle - for collision with borders and slowing down
		particleArray.at(i).updateParticle();

		//drawing the mirrored cursor
		if (mirrorMode) { 
			createMirrorCursor();
		}
			
		//drawing the particles in the array
		displayParticles(i);
	}
	glutSwapBuffers();
}

/* keyboard function - GLUT keyboard callback function
 *		calls when no action is occuring (key presses, mouse clicks)
 */
void kbd(unsigned char key, int x, int y)
{
	//close the window, shut down the program
	if(key == ESCAPE || key == 'q' || key =='Q') { 
		exit(0);
	}

	//Pause program
	if(key == SPACEBAR) {
		paused = !paused;
	}

	//make sure nothing occurs when paused
	if (!paused) {

		//Create a particle at mouse location
		if(key == 'a' || key == 'A') {
			y = screenSizeY - y;
			createParticles(x, y);
		}

		//Delete a particle from the screen/array
		if (key == 'd' || key == 'D') {
			y = screenSizeY - y;
			deleteParticle(x,y);
		}

		//reset program
		if(key == 'r' || key == 'R') {
			reset();
		}

		//increase range of particles
		if(key == '=' || key == '+') {
			increaseRange();
		}

		//decrease range of particles
		if(key == '-' || key == '_') {
			decreaseRange();
		}

		//increase range of particles
		if(key == ']' || key == '}') {
			increaseSpeed();
		}

		//decrease range of particles
		if(key == '[' || key == '{') {
			decreaseSpeed();
		}
	
		/* Extra Stuff */
		//clear the screen
		if(key == 'c' || key == 'C') {
			particleArray.clear();
		}

		//rotate particles around mouse
		if(key == 'x' || key == 'X') {
			rotateParticle(x,y);
		}
	
		//MIRROR MODE
		if (key == 'm' || key == 'M') {
			mirrorMode = !mirrorMode;	
		}

		//Display array size
		if (key == 'n' || key == 'N') {
			printf("array size: %d \n", particleArray.size());
		}
	
		//PARTY LIGHTS
		if (key == 'p' || key == 'P') {
			isParty = !isParty;
		}

		//Black Hole
		if (key == 'b' || key == 'B') {
			cBlackhole = !cBlackhole;
			if (cBlackhole) {
				particleArray.clear();
			} 
			else {
				reset();
			}
		}
	}
}

/* mouse function - GLUT mouse callback function
 *		calls when no action is occuring (key presses, mouse clicks)
 */
void mouse(int btn, int state, int x, int y)
{
	if (!paused) {
		y = screenSizeY - y;
		if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			LMBDown = true;
			gMouseX = x;
			gMouseY = y;
			mMouseX = x;
			mMouseY = screenSizeY - y;
		}
		else if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			LMBDown = false;
			gMouseX = x;
			gMouseY = y;
			mMouseX = x;
			mMouseY = screenSizeY - y;
		}

		if(btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		{
			RMBDown = true;
			gMouseX = x;
			gMouseY = y;
			mMouseX = x;
			mMouseY = screenSizeY - y;
		}
		else if(btn == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		{
			RMBDown = false;
			gMouseX = x;
			gMouseY = y;
			mMouseX = x;
			mMouseY = screenSizeY - y;
		}
	}
}
 
/* idle function - GLUT idle callback function
 *		calls when no action is occuring (key presses, mouse clicks)
 */
void idle()
{
	if (mirrorMode) {
		gMouseY = screenSizeY - gMouseY;
	}
	if (LMBDown == true) {
		moveParticle(gMouseX,gMouseY,true);
	}
	if (RMBDown == true) {
		moveParticle(gMouseX,gMouseY,false);
	}
	glutPostRedisplay();
}

/* mouseMotion function - GLUT mouseMotion callback function
 *		calls when no action is occuring (key presses, mouse clicks)
 */
void mouseMotion(int x, int y) {
	mMouseX = x;
	mMouseY = y;
	y = screenSizeY - y;
	gMouseX = x;
	gMouseY = y;
	
	if (LMBDown == true) {
		moveParticle(x,y,true);
	}
	if (RMBDown == true) {
		moveParticle(x,y,false);
	}
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	srand(time(NULL));
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitWindowPosition(500, 0);
	glutInitWindowSize(screenSizeX, screenSizeY);
	glutCreateWindow("Particle System");	//creates the window
	
	glutInitDisplayMode(GLUT_DOUBLE); // Set up a basic display buffer

	//left,right,top,bottom
	gluOrtho2D(0, screenSizeX, 0, screenSizeY); // set view to orthographic

	//displays commands to console
	consoleCommands();

	//Generate initial particles
	genInitParts();
	//create blackhole and add it to an array
	Blackhole bh;
	bh.position = point2D(screenSizeX/2, screenSizeY/2);
	bhArray.push_back(bh);
	
	//glut functions
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(kbd);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);
	glutMotionFunc(mouseMotion);

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}