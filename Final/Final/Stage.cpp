/*  
Authors: Rhett Amin, Kevin Ly, David Elsonbaty
Student Numbers: 1060085, 1144604 
File: Stage.cpp
----------------------------- 
This class does almost everything during the the time the main scene is being drawn/run
This class does:
	-Draws the background and stage
	-Draws fighters and bullets
	-Sets Materials
	-Draws healthbars
*/

#include "Stage.h"
#include "HealthBar.h"
#include <iostream>

using namespace std;

Stage::Stage(void){}
//Constructor
Stage::Stage(int *fightshieldstatep1, int *fightshieldstatep2)
{
	F1Shield = fightshieldstatep1;
	F2Shield = fightshieldstatep2;
}

//Intialization
colourStruc* color1;
colourStruc* color2;

//booleans - fighter facing direction
bool* isFighter1FacingRight = new bool(true);
bool* isFighter2FacingRight = new bool(false);

//Fighters
point3D *spawn1 = new point3D(0, 1.5, -15);
point3D *spawn2 = new point3D(0, 1.5, 15);
Fighter *fighter1 = new Fighter(spawn1, isFighter1FacingRight);
Fighter *fighter2 = new Fighter(spawn2, isFighter2FacingRight);
float *shieldsizep1 = new float(4);
float *shieldsizep2 = new float(4);
float fighter1Scale = 0.8;
float fighter2Scale = 0.8;

//Healthbars
HealthBar *leftPlayerHealth = new HealthBar(point3D(-13, -12, 0), fighter1->hitPoints);
HealthBar *rightPlayerHealth = new HealthBar(point3D(13, -12, 0), fighter2->hitPoints);

/*makePlatforms()
	creates the platform
*/
void Stage::makePlatform() {
	plat.createPlatform(40, point3D(0, 0, 0));
}

/*LoadPPM()
	Code provided by Dr.Teather
	Modified to not print out anything
*/
GLubyte* LoadPPM(const char* file, int* width, int* height, int* max)
{
	GLubyte* img;
	FILE *fd;
	int n, m;
	int  k, size;
	char c;
	int i;
	char b[100];
	float s;
	int red, green, blue;
	
	//open the file in read mode
	fd = fopen(file, "r");

	//scan everything up to newline
 	fscanf(fd,"%[^\n] ",b);

	//check if the first two characters are not P3, if not, it's not an ASCII PPM file
	if(b[0]!='P'|| b[1] != '3')
	{
		printf("%s is not a PPM file!\n",file); 
		exit(0);
	}

	//read past the file comments: scan for lines that begin 
	//  with #, and keep going until you find no more
	fscanf(fd, "%c",&c);
	while(c == '#') 
	{
		fscanf(fd, "%[^\n] ", b);
		fscanf(fd, "%c",&c);
	}

	//rewind the read pointer one character, or we'll lose the size
	ungetc(c,fd); 

	//read the rows, columns and max colour values
	fscanf(fd, "%d %d %d", &n, &m, &k);

	//number of pixels is rows * columns
	size = n*m;

	//allocate memory to store 3 GLuints for every pixel
	img =  (GLubyte *)malloc(3*sizeof(GLuint)*size);

	//scale the colour in case maxCol is not 255
	s=255.0/k;

	//start reading pixel colour data
	for(i=0;i<size;i++) 
	{
		fscanf(fd,"%d %d %d",&red, &green, &blue );
		img[3*size-3*i-3]=red*s;
		img[3*size-3*i-2]=green*s;
		img[3*size-3*i-1]=blue*s;
	}

	*width = n;
	*height = m;
	*max = k;

	return img;
}

/*loadTextures()
	Code provided by Dr.Teather
	Loads in all textures 
*/
void Stage::loadTextures() {
	glEnable(GL_TEXTURE_2D);
		glGenTextures(2, textures);
		//The background.ppm image was taken from wallbase.cc
		//The paltform texture was made by Rhett
		image[0] = LoadPPM("Images\\platform.ppm", &texWidth[0], &texHeight[0], &texMax[0]);
		image[1] = LoadPPM("Images\\wp.ppm", &texWidth[1], &texHeight[1], &texMax[1]);
	
		for (int i = 0; i < 2; i++) {
			glBindTexture(GL_TEXTURE_2D, textures[i]);
    
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth[i], texHeight[i], 0, GL_RGB, GL_UNSIGNED_BYTE, image[i]);
		}
    glDisable(GL_TEXTURE_2D);
}

/* drawBackground()
	craws the background texture square
*/
void Stage::drawBackground() {
	glEnable(GL_TEXTURE_2D); 
		glEnable(GL_COLOR_MATERIAL); 
	
			//Draw background
			glBindTexture(GL_TEXTURE_2D, textures[1]);
			
			glBegin(GL_POLYGON);	
				glNormal3d(0,0,1);
				glTexCoord2f(1,0); glVertex3f(-150, -150, -100);
				glTexCoord2f(0,0); glVertex3f( 150, -150, -100);
				glTexCoord2f(0,1); glVertex3f( 150,  150, -100);
				glTexCoord2f(1,1); glVertex3f(-150,  150, -100);
			glEnd();

			glColor4f(1, 1, 1, 1); 
          
        glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
}

/* drawStage()
	Function to call other functions to draw the scene
*/
void Stage::drawStage(){
    //draw background
    drawBackground();

    //Draw Platform
    plat.drawPlatform(textures[0]); // give the platform the texture to use
}

/* setMat()
	Function to set the shield material
 */
void setMat() {
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor4f(0.1, 0.1, 0.1, 0.5);
	float mat_specular[] = {0, 0, 0, 1.0};
	float mat_emission[] = {0.2, 0.2, 0.4, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, mat_emission);
}

/* setFighterMat()
	Function to set Fighter material
 */
void setFighterMat(Fighter* f) {
	//set their colours
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor4f(f->color->r, f->color->g, f->color->b, 1);
	float mat_specular[] = {0, 0, 0, 1.0};
	float mat_emission[] = {0.2, 0.2, 0.4, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, mat_emission);
}

/* drawShield1()
	Draws fighter1's shield
 */
void drawShield1() {
	glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
			//set alpha blending on see through bubble
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
				setMat();
				glTranslatef(fighter1->posFighter->x, fighter1->posFighter->y + 2, fighter1->posFighter->z);
				glutSolidSphere(*shieldsizep1, 10, 10);
				glColor4f(1, 1, 1, 1);
			glDisable(GL_BLEND);
		glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
}

/* drawShield2()
	Draws fighter2's shield
 */
void drawShield2() {
	glPushMatrix();
		glEnable(GL_COLOR_MATERIAL);
			//set alpha blending on see through bubble
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
				setMat();
				glTranslatef(fighter2->posFighter->x, fighter2->posFighter->y + 2, fighter2->posFighter->z);
				glutSolidSphere(*shieldsizep2, 10, 10);
				glColor4f(1, 1, 1, 1);
			glDisable(GL_BLEND);
		glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
}

/*drawBullet()
	Function to draw the bullets
*/
void Stage::drawBullet() {
	fighter1->displaybullet();
	fighter2->displaybullet();
}

//drawFighters() 
//for each fighter draw fighter, with respect to scale 
//rotate fighter 90 degrees on the y axis they z + is going to the right of the screen 
//draw black wire cube 
//draw coloured solid cub 
//also draw shield 
void Stage::drawFighters() { 
    glPushMatrix(); 
        glEnable(GL_COLOR_MATERIAL); 
              
            glScalef(fighter1Scale, fighter1Scale, fighter1Scale); 
              
            //Two different drawing methods, one for victory one for while fighting 
            if(!*fighter1->Victory){ 
                glRotatef(90, 0, 1, 0); 
                  
                glLineWidth(1.5); 
                glColor4f(0, 0, 0, 1); 
                fighter1->drawWire(); 
  
                glLineWidth(1); 
                setFighterMat(fighter1); 
                fighter1->drawBox(); 
            } else{ 
                glLineWidth(1.5); 
                glColor4f(0, 0, 0, 1); 
                fighter1->drawWireNoAngle(); 
                glLineWidth(1); 
                setFighterMat(fighter1); 
                fighter1->drawBoxNoAngle(); 
            } 
  
            //Check shield State 
            if(*F1Shield == 1) { 
                drawShield1(); 
            } 
        glDisable(GL_COLOR_MATERIAL); 
    glPopMatrix(); 
  
    glPushMatrix(); 
        glEnable(GL_COLOR_MATERIAL); 
  
            glScalef(fighter2Scale, fighter2Scale, fighter2Scale); 
            if(!*fighter2->Victory){ 
                glRotatef(90, 0, 1, 0); 
  
                glLineWidth(1.5); 
                glColor4f(0, 0, 0, 1); 
                fighter2->drawWire(); 
  
                glLineWidth(1); 
                setFighterMat(fighter2); 
                fighter2->drawBox(); 
            } else{ 
                glLineWidth(1.5); 
                glColor4f(0, 0, 0, 1); 
                fighter2->drawWireNoAngle(); 
                glLineWidth(1); 
                setFighterMat(fighter2); 
                fighter2->drawBoxNoAngle(); 
            } 
          
            if(*F2Shield == 1) { 
                drawShield2(); 
            } 
        glDisable(GL_COLOR_MATERIAL); 
    glPopMatrix(); 
  
    //Draw healthbars 
    leftPlayerHealth->drawHealthBar(true); 
    rightPlayerHealth->drawHealthBar(false); 
} 
  
//aniFighters() 
//executes all the fighter object animation functions 
//this includes 
//animate, collisionCheck, aniBullet 
//does this for both fighters 
void Stage::animFighters() { 
    fighter1->animate(); 
    fighter1->collisionCheck(); 
    fighter1->aniBullet(); 
    fighter2->animate(); 
    fighter2->collisionCheck(); 
    fighter2->aniBullet(); 
  
    //Victory Check if hit points of fighter is < = 0 
    //move winning fighter up to z @ 15 
    //scales winning fighter to 1.5 
    //set winning fighter boolean victory state = true; 
    if(*fighter1->hitPoints <= 0){ 
        while(fighter2->posFighter->z < 15) { 
			fighter2->moveFighter(0,0,1); 
        } 
		while(fighter2->posFighter->z > 15) {
			fighter2->moveFighter(0,0,-1); 
		}
        *fighter1->hitPoints = 0; 
        fighter1Scale = 0; 
        fighter2Scale = 1.5; 
        *fighter2->Victory = true; 
    } 
    if(*fighter2->hitPoints <= 0){ 
        while(fighter1->posFighter->z < 15) { 
            fighter1->moveFighter(0,0,1); 
        } 
		while(fighter1->posFighter->z > 15) {
			fighter1->moveFighter(0,0,-1); 
		}
        *fighter2->hitPoints = 0; 
        fighter1Scale = 1.5; 
        fighter2Scale = 0; 
        *fighter1->Victory = true; 
    } 
    //If both fighters fall below -30 y delete fighter 
    if(fighter1->posFighter->y < -30) { 
        *fighter1->hitPoints = 0; 
    } 
    if(fighter2->posFighter->y < -30) { 
        *fighter2->hitPoints = 0; 
    } 
} 
  
//moveFighter1 
//Mainly used for victory scene, Move fighter1 with an increment of z 
void Stage::moveFighter1(float z) { 
    fighter1->moveFighter(0, 0, z); 
} 
  
//moveFighter2 
//Mainly used for victory scene, Move fighter2 with an increment of z 
void Stage::moveFighter2(float z) { 
    fighter2->moveFighter(0, 0, z); 
} 
  
//actfighter1 
//If shield for fighter 1 not active execute the buffered keyboard input for action -> which will then make an animation object 
void Stage::actFighter1(int num) { 
    if (*F1Shield == 0){ 
        fighter1->action(num); 
    } 
} 
  
//actfighter2 
//If shield for fighter 2 not active execute the buffered keyboard input for action -> which will then make an animation object 
void Stage::actFighter2(int num) { 
    if (*F2Shield == 0){ 
        fighter2->action(num); 
    } 
} 
  
//createStage 
//Utilizes the PPM loader to load the texture map, 
//makes the platform 
void Stage::createStage() { 
    loadTextures(); 
    makePlatform(); 
} 
  
//setFighterColour 
//set a fighter object colour to equal a colouStruc object 
void Stage::setFighterColour(int player, colourStruc* colour){ 
    if (player == 0) { 
        fighter1->color->r = colour->r; 
        fighter1->color->g = colour->g; 
        fighter1->color->b = colour->b; 
    } 
    else if (player == 1) { 
        fighter2->color->r = colour->r; 
        fighter2->color->g = colour->g; 
        fighter2->color->b = colour->b; 
    } 
} 
  
//setFighterFace 
//Utilize this function for fighter facing direction 
//if they are moving right, face right, else they are moving left, then face left. 
void Stage::setFighterFace(int player, bool isRight) { 
    if (player == 0) { 
        *isFighter1FacingRight = isRight; 
    } 
    if (player == 1) { 
        *isFighter2FacingRight = isRight; 
    } 
} 
  
//setFighterColour 
//get colours from main(gamestate) and set to the fighters to be used on stage 
void Stage::setFighterColour(colourStruc* c1, colourStruc* c2) { 
    fighter1->color = c1; 
    fighter2->color = c2; 
} 
  
//callFigherUpdate 
//update the fighter objects to have interactions with their shields and other fighters 
void Stage::callFighterUpdate() { 
    fighter1->updateInfo(fighter2, F1Shield, shieldsizep1); 
    fighter2->updateInfo(fighter1, F2Shield, shieldsizep2); 
} 
//activateBullet(int Player) 
//Activatie bullets for player 1 and player2 based on inputs 
void Stage::activateBullet(int player){ 
    if (player == 0) { 
        fighter1->activateBullet(); 
    }else { 
        fighter2->activateBullet(); 
    } 
} 
  
//getVicStatus() 
//Checks victory status between fighter1 and fighter2 
//if either of them is true, return true, else false 
bool Stage::getVicStatus() { 
    if (*fighter1->Victory || *fighter2->Victory ){ 
        return true; 
    } else { 
        return false; 
    } 
}