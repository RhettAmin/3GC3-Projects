#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <vector>
#include <time.h>
#include "MathLib3D.h"

#define ESCAPE 27
#define SPACE 32

using namespace std;

//Blueprints - for the vectors to function
struct Floor;
struct Particle;

float ang = 0;			//angle for rotating cubes
float sceneRotX = 0;	//rotate the scene
float sceneRotY = 0;

//Camera Position
point3D camPos = point3D(0, 5, 20);

//screen size, and aspect ratio for gluPerspective
int screenSizeX = 800;
int screenSizeY = 600;
float aspect = (screenSizeX)/(screenSizeY);

//Physics Forces
float gravity = 0.01;
float wind = 0.001;

//Amount of Collision floors to draw
int layers = 7;

//Booleans for various functions
bool floorsDrawn = false;
bool isFriction = true;
bool isPaused = false;
bool isWindy = false;
bool isCartoony = false;

//Vectors to stores particles and Collision Floors
vector<Particle> pArray;
vector<Floor> floorArray;


//Struct to hold each Collision Floor layer
struct Floor {
	//The floor is a square
	//	x1 - neg. edge for x and z axes
	//	x2 - pos. edge for x and z axes
	//	height - 7 layers of varied heights
	float x1;
	float x2;
	float height;
};

/*Struct for each particle
* Also updates the particles attributes through the
* update function part of the particle struct.
*/
struct Particle {
	point3D position;
	vec3D direction;
	float speed;
	float rotAngleX;   // axes rotation
	float rotAngleY;
	float rotAngleZ;
	float size;
	float colour[3];
	float age;
	bool passedTop;    

	//Updates the particles attributes
	void update(int i){

		//If the particle passes the top of the pyramid
		//	so the particle doesn't collide right away.
		if (position.y >= 5 ) {
			passedTop = true;
		}

		//decrement the age and kill/delete if it dies
		age--;
		if (age <= 0) {
			pArray.erase(pArray.begin() + i);
		}

		//Updates the position of the particle
		position.x = position.x + direction.x * speed;
		position.y = position.y + direction.y * speed;
		position.z = position.z + direction.z * speed;

		//Updates direction with gravity and wind(if enabled)
		direction.y = direction.y - gravity;
		if (isWindy){
			direction.x = direction.x + wind;
			direction.z = direction.z + wind;
		}

		//Check if the particle collides with each floor
		if (passedTop == true) {
			for (int i = 0; i < layers; i++) {
				if ((position.x > floorArray.at(i).x1 && position.x < floorArray.at(i).x2) && (position.z > floorArray.at(i).x1 && position.z < floorArray.at(i).x2)) {
					if(position.y <= floorArray.at(i).height) {
						if(isFriction) {
							direction.y = direction.y * -0.75f; //lose momentum
						}
						else {
							direction.y = direction.y * -1;     // keep momentum
						}
					}
				}
			}
		}		
	}

	//Draws the particle
	void drawParticle() {
		glPushMatrix();
		
		//Move Particle with update position, and rotate around random axes.
		glTranslatef(position.x, position.y, position.z);
		
		glRotatef(rotAngleX * ang, 1, 0, 0);  //multiple by ang, for different rotation speeds
		glRotatef(rotAngleY * ang, 0, 1, 0);
		glRotatef(rotAngleZ * ang, 0, 0, 1);
		

		//draws an outline for a cell shading effect
		if (isCartoony) {
			glLineWidth(3);
			glColor3f(0,0,0);
			glutWireCube(size);
		}
		glColor3f(colour[0], colour[1], colour[2]);
		glutSolidCube(size);

		glPopMatrix();
	
	}
};

/* createParticle(int time)
* This function initilizes each propety of the particle
* and then adds them to the particle array(pArray).
* It also recalls with glutTimerFunc itself 
* so particles are created forever.
*/
void createParticle(int time) {
	Particle part;

	//Particle origin
	part.position = point3D(0, 0, 0);

	//Give Particles random x and z directions
	float randDirX = MathLib3D::randFloat(0.08, 0.08);
	float randDirZ = MathLib3D::randFloat(0.08, 0.08);
	part.direction = vec3D(randDirX, 1, randDirZ);
	
	part.colour[0] = MathLib3D::randFloat(0,1); //red
	part.colour[1] = MathLib3D::randFloat(0,1); //green
	part.colour[2] = MathLib3D::randFloat(0,1); //blue

	//random angles
	part.rotAngleX = MathLib3D::randInt(360);
	part.rotAngleY = MathLib3D::randInt(360);
	part.rotAngleZ = MathLib3D::randInt(360);

	part.speed = MathLib3D::randFloat(-0.1, 0.2);
	part.size =  MathLib3D::randFloat(-0.3, 0.5);

	part.age = MathLib3D::randFloat(-1000, 1500);
	
	part.passedTop = false;

	pArray.push_back(part);

	//Recall this function for an infinite loop, 
	//if not paused
	if (!isPaused) {
		glutTimerFunc(100, createParticle, 0);
	}
}

/* drawPyramid()
*  Draws the main floor, with extra floors on top in the shape of a pyramid.
*  Each pyramid floor is made up of multiple glutSolidCubes.
*  Also creates collision floors for each flor of the pyramid.
*/
void drawPyramid() {

	//Array storing the colours of each floor
	//Offset is to expand the collision floor to match each draw floor(floor is scaled).
	float colour[7][3] = {{1, 0, 0}, {1, 0.5f, 0}, {1, 1, 0.2f}, {0, 1, 0}, {0.1f, 0.1f, 1}, {0.4f, 0.3f, 0.9f}, {0.8f, 0, 0.8f}};
	float layerOffset[7] = {2.4f, 2.0f, 1.8f, 1.6f, 1.4f, 1.2f, 1.f};

	//Variables used to help draw the pyramid
	//initX - initial x pos. for each floor layer
	//initZ - initial z pos. for each floor layer
	//levelX - x pos. of where to draw each cube
	//levelZ - z pos. of where to draw each cube
	//levelY - y pos. of what height to draw each cube
	float initX = -3;
	float initZ = -3;
	float levelX = initX;
	float levelY = 0;
	float levelZ = initZ;

	//Numbers of rows and columns for each floor.
	float itr = 7;

	glPushMatrix(); //Begin Pyramid

		//Algotrithmn to draw each layer of the pyramid and to create each collision floor.
		//Outer loop  - is for each layer, increments height, and where to draw the the layer in the xz-plane.
		//Second loop - increases cube pos. along z plane.
		//inner loop  - increases cube pos. along x plane.
		for (int y = 0; y < layers; y++) {
			for (int i = 0; i < itr; i++) {
				for (int j = 0; j < itr; j++) {
					//Draw Cube and it's outline
					glPushMatrix();
						glScalef(1.5f,0.5f,1.5f);
						glTranslatef(levelX, levelY, levelZ);
						glLineWidth(3);
						glColor3f(0,0,0);
						glutWireCube(1.f);
						glColor3fv(colour[y]);
						glutSolidCube(1.f);
						levelX++;
					glPopMatrix();
				}
				//Increment Z pos. and reset X pos. to initX.
				levelZ++;
				levelX = initX;		
			}
			//Draw collision floor for each layer of the pyramid.
			if (!floorsDrawn) {
				Floor floor;
				floor.x1 = initX - layerOffset[y]; //Offset is to match the scaled cubes
				floor.x2 = -initX + layerOffset[y];
				floor.height = (levelY * 0.5f) + 0.5f; 
				floorArray.push_back(floor);
			}
			//updates values to draw next layer
			initX += 0.5;
			initZ += 0.5;
			levelX = initX;
			levelZ = initZ;
			levelY += 1;
			itr--;
		}

		//Stop creating collision floors after intial 7 are created
		if (!floorsDrawn) {
			floorsDrawn = true;
		}

	glPopMatrix();	// end pyramid
}

/* rotationTimer(int time)
*  increments ang and recalls function to keep incrementing.
*  increments different amounts for if isCartoony = true
*/
void rotationTimer(int time) {
	if (!isPaused) {
		if (isCartoony) {
			ang+=0.5;
			glutTimerFunc(100,rotationTimer, 0);
		} else {
			ang+=0.02;
			glutTimerFunc(10,rotationTimer, 0);
		}
	}
}

/* display() - the OpenGL display function, this draws the screen
 *  it displays a spinning cube
 */
void display()
{
        //clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set MatrixMode to Modelview for adding and updating objects.
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(camPos.x, camPos.y, camPos.z, 0, 2, 0, 0, 1, 0);

		//Whole System
		glPushMatrix();
		
			//rotation of the scene about x and y axes.
			glRotatef(sceneRotY, 0, 1, 0);
			glRotatef(sceneRotX, 1, 0, 0);

			//call function to draw pyramid
			drawPyramid();

			//Update particle 
			for (int i = 0; i < pArray.size(); i ++) {
				
				//Update particle position, direction, and rotation
				//then draw it
				if (!isPaused) {
					pArray.at(i).update(i);
				}
				pArray.at(i).drawParticle();
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

	switch(key) {

		//Close program
		case 'q':
		case 'Q':
		case ESCAPE:
			 exit(0);
			break;

		//Set Friction mode on/off
		case 'f':
		case 'F':
			isFriction = !isFriction;
			break;
	
		//Pause/unpause creation of particles
		case SPACE:
			isPaused = !isPaused;
			if (!isPaused) {
				glutTimerFunc(100,createParticle,0);
			}
			break;

		//Clear particle array , reseting simulation.
		case 'r':
		case 'R':
			pArray.clear();
			break;
	
		//Turn if Wind is on/off
		case 'w':
		case 'W':
			isWindy = !isWindy;
			break;

		//Draw one less layer of the pyramid
		case 'n':
		case 'N':
			if (layers <= 1) {
				layers = 1;
			} else {
				layers--;
			}
			break;

		//draw one more layer of the pyramid
		//Move all particles on lower layer up
		case 'm':
		case 'M':
			if (layers >= 7) {
				layers = 7;
			} else {
				layers++;
				for (int i = 0; i < pArray.size(); i++) {
					if ((pArray.at(i).position.x > floorArray.at(layers - 1).x1 && pArray.at(i).position.x < floorArray.at(layers - 1).x2) && (pArray.at(i).position.z > floorArray.at(layers - 1).x1 && pArray.at(i).position.z < floorArray.at(layers - 1).x2)) {
						if (pArray.at(i).position.y < floorArray.at(layers).height && pArray.at(i).position.y >= floorArray.at(layers).height - 2) {
							pArray.at(i).position.y+=2;
							pArray.at(i).direction.y = pArray.at(i).direction.y * - 1.5f;
						}
					}
				}
			}
			break;

		//Turn on/off a cartoon like effect for the particles.
		case 'c':
		case 'C':
			isCartoony = !isCartoony;
			break;
		}
}

/* special -- the GLUT special function
 *  key -- the key pressed
 *  x and y - mouse x and y coordinates at the time the function is called
 *  handles arrow key presses for scene rotation.
 */
void special(int key, int x, int y)
{
	/* arrow key presses move the camera */
	switch(key)
	{
		//Rotate left along y axis
		case GLUT_KEY_LEFT:	
			sceneRotY += 2;
			break;

		//Rotate right along y axis
		case GLUT_KEY_RIGHT:
			sceneRotY -= 2;
			break;

		//Rotate left along x axis
		case GLUT_KEY_UP:
			sceneRotX += 2;
			break;

		//Rotate right along x axis
		case GLUT_KEY_DOWN:
			sceneRotX -= 2;
			break;
	}
}

void displayReadMe() {
	printf("Particle System Commands \n");
	printf("Commands \n");
	printf("%-15s %-75s\n"," 'f'          ---","Turn on/off friction mode");
	printf("%-15s %-75s\n"," 'r'          ---","Reset simulation, clear particles in array");
	printf("%-15s %-75s\n"," 'space'      ---","Pauses simulation");
	printf("%-15s %-75s\n"," 'arrow keys' ---","Rotate the scene");
	printf("%-15s %-75s\n"," 'w'          ---","Turn on/off the wind");
	printf("%-15s %-75s\n"," 'c'          ---","Turn on/off cartoon mode");
	printf("%-15s %-75s\n"," 'n'          ---","Decrease layers on the pyramid");
	printf("%-15s %-75s\n"," 'm'          ---","Increase layers on the pyramid");
}

/* main(int argc, char** argv)
*   initialize all glut initization.
*/
int main(int argc, char** argv)
{
	srand(time(NULL));

	//Display commands to command window
	displayReadMe();

    //glut initialization stuff:
    // set the window size, display mode, and create the window
    glutInit(&argc, argv);
    glutInitWindowSize(screenSizeX, screenSizeY);
	glutInitWindowPosition(450,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Particle System 3D");

    //enable Z buffer test, otherwise things appear in the order they're drawn
    glEnable(GL_DEPTH_TEST);

    //setup the initial view
    // change to projection matrix mode, set the extents of our viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60, aspect, 1, 100);
		
    //set background colour to grey
    glClearColor(0.5, 0.5, 0.5, 0);
	
	//enable culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

    //register glut callbacks for keyboard and display function
    glutKeyboardFunc(kbd);
    glutDisplayFunc(display);
	glutSpecialFunc(special);

	//start creating particles
	glutTimerFunc(100, createParticle, 0);
	//start rotation incrementation
	glutTimerFunc(10,rotationTimer,0);

    //start the program!
    glutMainLoop();

    return 0;
}