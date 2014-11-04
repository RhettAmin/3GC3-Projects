#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <vector>
#include <time.h>
#include "mathLib.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#define ESCAPE 27
#define SPACE 32

using namespace std;

//Blueprints
struct Material;
struct Light;
struct BB;
struct Object;
void fileSave(string fileName);
void fileLoad(string fileName);

//screen size, and aspect ratio for gluPerspective
float screenSizeX = 800;
float screenSizeY = 600;
float aspect = (screenSizeX)/(screenSizeY);
point3D objectOrigin = point3D(0, 0, 0);

//Rays
GLint view[4];
GLdouble proj[16];
GLdouble modelview[16];
point3D GetPosition(int x, int y);

float sceneRotZ;
float sceneRotY;

//Lighting
int lSelected = 0;

//Material and textures
int gMat = 1;
GLuint texArray[3];
GLubyte* tex[3];
int width[3];
int height[3];
int maxTex[3];

// Selection
int objSelected;

point3D bbOffset;

bool rPressed = false;

vector<Object> objectList;
vector<Light> LightList;

struct Material {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float shininess;
};

struct Light {
	float pos[4];
	float amb[4];
	float diff[4];
	float spec[4];
};

struct BB {
	point3D FTR;
	point3D BBL;
};

struct Object {
    int type; // Cube, sphere, teapot, Torus, Isocehedron
    int oMat;
    point3D position;
    point3D translation;
    point3D scale;
	point3D rotation;
	float objRZ;
	float objRX;
	float objRY;
    BB bb;

};

GLubyte* LoadPPM(char* file, int* width, int* height, int* max)
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

	printf("%s is a PPM file\n", file);

	//read past the file comments: scan for lines that begin 
	//  with #, and keep going until you find no more
	fscanf(fd, "%c",&c);
	while(c == '#') 
	{
		fscanf(fd, "%[^\n] ", b);
		printf("%s\n",b);
		fscanf(fd, "%c",&c);
	}

	//rewind the read pointer one character, or we'll lose the size
	ungetc(c,fd); 

	//read the rows, columns and max colour values
	fscanf(fd, "%d %d %d", &n, &m, &k);

	printf("%d rows  %d columns  max value= %d\n",n,m,k);

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

void loadTextures(){
	/*
	tex[0] = LoadPPM("image.ppm", &width[0], &height[0], &maxTex[0]);
	tex[1] = LoadPPM("grass. ppm", &width[1], &height[1], &maxTex[1]);
	tex[2] = LoadPPM("random.ppm", &width[2], &height[2], &maxTex[2]);	

	for (int i = 0; i < 3; i++) {
		glBindTexture(GL_TEXTURE_2D, texArray[i]);
		
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[i], height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, tex[i]);
	}
	*/

	// Get the image file type from FreeImage.
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType("images.jpg", 0);

	// Actually load the image file.
	tex[0] = FreeImage_Load(fifmt, "images.jpg",0);

	// Now, there is no guarantee that the image file
	// loaded will be GL_RGB, so we force FreeImage to
	// convert the image to GL_RGB.
    dib = FreeImage_ConvertTo24Bits(dib);

    if( dib != NULL )
	{	
        glGenTextures( 1, textures);
		glBindTexture( GL_TEXTURE_2D, tex[0]);
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		
		// This is important to note, FreeImage loads textures in
		// BGR format. Now we could just use the GL_BGR extension
		// But, we will simply swap the B and R components ourselves.
		// Firstly, allocate the new bit data doe the image.
		BYTE *bits = new BYTE[FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib) * 3];

		// get a pointer to FreeImage's data.
		BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);

		// Iterate through the pixels, copying the data
		// from 'pixels' to 'bits' except in RGB format.
		for(int pix=0; pix<FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib); pix++)
		{
			bits[pix*3+0]=pixels[pix*3+2];
			bits[pix*3+1]=pixels[pix*3+1];
			bits[pix*3+2]=pixels[pix*3+0];

		}
	     
        // The new 'glTexImage2D' function, the prime difference
		// being that it gets the width, height and pixel information
		// from 'bits', which is the RGB pixel data..
		glTexImage2D( GL_TEXTURE_2D, 0, 3, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), 0,
				GL_RGB, GL_UNSIGNED_BYTE, bits );

		
		// Unload the image.
		// and free the bit data.
		FreeImage_Unload(dib);
		delete bits;
	}
}

void setMaterial(int num) {
    Material mat;

    if (num == 1) {
        Material redPlastic = {
                {0.3f, 0.f, 0.f, 1.f},
                {0.6f, 0.f, 0.f, 1.f},
                {0.8f, 0.6f, 0.6f, 1.f},
                27.f
            };
        mat = redPlastic;
    }
    else if (num == 2) {
        Material jade = {
                {0.135f, 0.22f, 0.157f, 0.95f},
                {0.54f, 0.89f, 0.63f, 0.95f },
                {0.31f, 0.32f, 0.32f, 0.95f },
                12.8f
            };
        mat = jade;
    }
    else if (num == 3) {
        Material yellowRubber = {
                {0.05f,0.05f,0.0f,1.0f},
                {0.5f,0.5f,0.4f,1.0f},
                {0.7f,0.7f,0.04f,1.0f},
                10.f
            };
        mat = yellowRubber;
    }
    else if (num == 4) {
        Material blueMatte = {
                {0.0f,0.05f,0.05f,1.0f},
                {0.2f,0.3f,0.8f,1.0f},
                {0.04f,0.07f,0.07f,1.0f},
                10.f
            };
        mat = blueMatte;
    }
    else if (num == 5) {
        Material pearl = {
                { 0.25f, 0.21f, 0.21f, 0.92f },
                {1.0f, 0.829f, 0.829f, 0.92f },
                {0.29f, 0.29f, 0.29f, 0.92f },
                11.3f
            };
        mat = pearl;
    }
	else if (num == 6) {
		glBindTexture(GL_TEXTURE_2D, texArray[0]);
	}
	else if (num == 7) {
		glBindTexture(GL_TEXTURE_2D, texArray[1]);
	}
	else if (num == 8) {
		glBindTexture(GL_TEXTURE_2D, texArray[2]);
	}
    else if (num == 0) {
        Material obsidian = {
                {0.054f, 0.05f, 0.066f, 0.82f},
                {0.18f, 0.17f, 0.225f, 0.82f},
                {0.33f, 0.33f, 0.35f, 0.82f},
                38.4f
            };
        mat = obsidian;
    }

	if (! (num == 6 || num == 7 || num == 8)) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat.ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat.diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,mat.specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,mat.shininess);
	}
}

void addLights() {
	for (int l = 0; l < 2; l++) {
		glLightfv(GL_LIGHT0, GL_POSITION, LightList.at(l).pos);
		glLightfv(GL_LIGHT0, GL_AMBIENT,  LightList.at(l).amb);
		glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightList.at(l).diff);
		glLightfv(GL_LIGHT0, GL_SPECULAR, LightList.at(l).spec);
			
		glPushMatrix();
					
			glTranslatef(LightList.at(l).pos[0], LightList.at(l).pos[1], LightList.at(l).pos[2]);
			glLineWidth(1);
			setMaterial(0);
			glutWireSphere(0.2, 5, 5);

		glPopMatrix();
	}
}

void createLights() {

		Light light0 = {
			{0, 5, 0, 1},
			{1, 1, 1, 1},
			{1, 1, 1, 1},
			{1, 1, 1, 1}
		};

		Light light1 = {
			{4, 2, 2, 1},
			{1, 1, 1, 1},
			{1, 1, 1, 1},
			{1, 1, 1, 1}
		};

		LightList.push_back(light0);
		LightList.push_back(light1);
}

void drawBase() {

    glPushMatrix(); 
	setMaterial(6);

    glBegin(GL_QUADS);
        glNormal3d(0,1,0);
		glTexCoord2f(1,0);
        glVertex3f(-15, -2, 15);
		glTexCoord2f(0,0);
        glVertex3f(15, -2, 15);
		glTexCoord2f(0,1);
        glVertex3f(15, -2, -15);
		glTexCoord2f(1,1);
        glVertex3f(-15, -2, -15);
    glEnd();
        
    glPopMatrix();  
}

void createObject (int num) {

        Object object;

        object.type = num;
        object.oMat = gMat;
        object.position = objectOrigin;
        
        object.translation = point3D(0, 0, 0);
        object.scale = point3D(1, 1, 1);
        object.rotation = point3D(0, 0, 0);

		//creates bounding boxes
	object.bb.FTR =  _3DMathLib::addPoints(object.position, object.scale);
	object.bb.BBL =  _3DMathLib::addPoints(object.position, _3DMathLib:: pointMultiply(object.scale, -1.0));
        objectList.push_back(object);

        objSelected = objectList.size() - 1; 
}

void drawObject(int index) {
    glPushMatrix();
         
        setMaterial(objectList.at(index).oMat);
        glTranslatef(objectList.at(index).translation.x, objectList.at(index).translation.y,        
        objectList.at(index).translation.z);
        glRotatef(objectList.at(index).rotation.x, 1, 0, 0);
        glRotatef(objectList.at(index).rotation.y, 0, 1, 0);
        glRotatef(objectList.at(index).rotation.z, 0, 0, 1);
        glScalef(objectList.at(index).scale.x, objectList.at(index).scale.y, objectList.at(index).scale.z);

        if (objectList.at(index).type == 1) {
            glutSolidCube(1);    
            bbOffset = point3D(0, 0, 0);
        }
        if (objectList.at(index).type == 2) {
			glScalef(0.6,0.6,0.6);
			bbOffset = point3D(0, 0, 0);            
			glutSolidSphere(1, 10, 10);
        }
        if (objectList.at(index).type == 3) {
			glScalef(1,1.9,0.7);
            glutSolidTeapot(1);
			bbOffset = point3D(1.75, 1.75, 0);
        }
        if (objectList.at(index).type == 4) {
            glutSolidTorus(0.5, 1, 10, 10);
            bbOffset = point3D(1.75, 1.75, 0);
        }
        if (objectList.at(index).type == 5) {
            glScalef(0.5f, 0.5f, 0.5f);
            glutSolidDodecahedron();
            bbOffset = point3D(0.5, 0.5, 0.5);
        }
	glPopMatrix();

	//updates the bb relative to the object
	objectList.at(index).bb.FTR = _3DMathLib:: addPoints(bbOffset, _3DMathLib:: pointMultiply(objectList.at(index).bb.FTR,0.5));
	objectList.at(index).bb.BBL = _3DMathLib:: addPoints(bbOffset, _3DMathLib:: pointMultiply(objectList.at(index).bb.BBL,0.5));

	//updates the bb relative to the translation
	objectList.at(index).bb.FTR = _3DMathLib:: addPoints(objectList.at(index).translation, objectList.at(index).scale);
    objectList.at(index).bb.BBL = _3DMathLib:: addPoints(objectList.at(index).translation, _3DMathLib:: pointMultiply(objectList.at(index).scale,-1.0)); 	

	glPushMatrix();
	
	//updates the wire box of the selected item
        if (index == objSelected) {
            glLineWidth(1.5f);
			glTranslatef(objectList.at(index).translation.x, objectList.at(index).translation.y, objectList.at(index).translation.z);
            glRotatef(objectList.at(index).rotation.x, 1, 0, 0);
            glRotatef(objectList.at(index).rotation.y, 0, 1, 0);
            glRotatef(objectList.at(index).rotation.z, 0, 0, 1);
            glScalef(objectList.at(index).scale.x + bbOffset.x, objectList.at(index).scale.y + bbOffset.y, objectList.at(index).scale.z + bbOffset.z);
            glEnable(GL_COLOR_MATERIAL);
			glColor3f(0,0.5,0);
			glDisable(GL_COLOR_MATERIAL);
            glutWireCube(1);
        }
	
        glPopMatrix();        
}

void mouse(int btn, int state, int x, int y){
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{	
		point3D mouseHit = GetPosition(x, y);
		/* temporary object index. Will be used to select an object. Starts as a large number since it will be changed afterwards */
		int tempIndex = 100000;
		for (int j = 0; j < objectList.size() ; j++){
			//cout << "BBL " << objectList.at(j).bb.BBL.x << ", " << objectList.at(j).bb.BBL.y << " ," << objectList.at(j).bb.BBL.z << "\n";
			//cout << "FTR " << objectList.at(j).bb.FTR.x << ", " << objectList.at(j).bb.FTR.y << " ," <<  objectList.at(j).bb.FTR.z << "\n";
			//cout << "MOUSE " << mouseHit.x << ", " << mouseHit.y << ", " << mouseHit.z << "\n";
				
			if ( objectList.at(j).bb.BBL.x <= mouseHit.x && mouseHit.x <= objectList.at(j).bb.FTR.x && 
			objectList.at(j).bb.BBL.y <= mouseHit.y && mouseHit.y  <= objectList.at(j).bb.FTR.y)
				
				if (tempIndex <= objectList.size()){
					
					if ( abs(objectList.at(j).bb.BBL.x - mouseHit.x) <= abs (objectList.at(tempIndex).bb.BBL.x - mouseHit.x) &&
					abs(objectList.at(j).bb.FTR.x - mouseHit.x) <=  abs (objectList.at(tempIndex).bb.FTR.x - mouseHit.x) &&
					abs(objectList.at(j).bb.BBL.y - mouseHit.y) <=  abs (objectList.at(tempIndex).bb.BBL.y - mouseHit.y) &&
					abs(objectList.at(j).bb.FTR.y - mouseHit.y) <=  abs (objectList.at(tempIndex).bb.FTR.y - mouseHit.y) &&
					abs(objectList.at(j).bb.BBL.z - mouseHit.z) <=  abs (objectList.at(tempIndex).bb.BBL.z - mouseHit.z) &&
					abs(objectList.at(j).bb.FTR.z - mouseHit.z) <= abs(objectList.at(tempIndex).bb.FTR.z - mouseHit.z) )
						tempIndex = j;
						
						
				}else tempIndex = j;
			
		}
		objSelected = tempIndex;
		
		glutPostRedisplay();
	}
}

/* NB IDEA FROM http://nehe.gamedev.net/article/using_gluunproject/16013/
Gets the position of the mouse click onto the screen
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
 *  it displays a spinning cube
 */
void display()
{
        //clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Set MatrixMode to Modelview for adding and updating objects.
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 5, 20, 0, 2, 0, 0, 1, 0);

		//rotation of the scene about z and y axes.
        glRotatef(sceneRotY, 0, 1, 0);
        glRotatef(sceneRotZ, 0, 0, 1);

        //Whole System
        glPushMatrix();

			addLights();
            drawBase();
			 
            for (int i = 0; i < objectList.size(); i++) {
                drawObject(i);
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
    //Object Transformations
    //Rotation
    if (glutGetModifiers() == GLUT_ACTIVE_SHIFT && key == 'Z' && objectList.size() > 0 && objSelected < objectList.size()) {
        objectList.at(objSelected).rotation.x -= 2;
    }
    else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT && key == 'X' && objectList.size() > 0 && objSelected < objectList.size()) {
        objectList.at(objSelected).rotation.z -= 2;
    }
    else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT && key == 'C' && objectList.size() > 0 && objSelected < objectList.size()) {
        objectList.at(objSelected).rotation.y -= 2;
    }

    //Translation
    else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT && key == 'B' && objectList.size() > 0 && objSelected < objectList.size()) {
        objectList.at(objSelected).translation.x -= 0.3;
    }
    else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT && key == 'N' && objectList.size() > 0 && objSelected < objectList.size()) {
        objectList.at(objSelected).translation.y -= 0.3;
    }
    else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT && key == 'M' && objectList.size() > 0 && objSelected < objectList.size()) {
        objectList.at(objSelected).translation.z -= 0.3;
    }
    
    //Scale
    else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT && key == 'F' && objectList.size() > 0 && objSelected < objectList.size()) {
        objectList.at(objSelected).scale.x -= 0.5;
    }
    else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT && key == 'G' && objectList.size() > 0 && objSelected < objectList.size() ){
        objectList.at(objSelected).scale.y -= 0.3;
    }
    else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT && key == 'H' && objectList.size() > 0 && objSelected < objectList.size()) {
        objectList.at(objSelected).scale.z -= 0.3;
    }
   
	//Light Transformation
	//Translation
    else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT && key == 'q') {
		LightList.at(lSelected).pos[0] -= 0.5;
    }
    else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT && key == 'w') {
        LightList.at(lSelected).pos[1] -= 0.5;
    }
    else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT && key == 'e') {
        LightList.at(lSelected).pos[2] -= 0.5;
    }

    else {
        

    switch(key) {

        //Close program
        case ESCAPE:
             exit(0);
            break;

        case 'd':
        case 'D':
            if (objectList.size() > 0) {
                objectList.erase(objectList.begin());
                objSelected--;
            }
            break;

        case 'a':
        case 'A':
            objectList.at(objSelected).oMat = gMat;
            break;

        case 'y':
		case 'Y':
            createObject(1);
            cout << "cube \n";
            break;

        case 'u':
		case 'U':
            createObject(2);
            cout << "sphere \n";
            break;

        case 'i':
		case 'I':
            createObject(3);
            cout << "teapot \n";
            break;

        case 'o':
		case 'O':
            createObject(4);
            cout << "torus \n";
            break;

        case 'p':
		case 'P':
            createObject(5);
            cout << "Dodecahedron \n";
            break;

        case '1':
            gMat = 1;
            setMaterial(gMat);
            cout << "Current Material: Red Plastic\n";
            break;
        case '2':
            gMat = 2;
            setMaterial(gMat);
            cout << "Current Material: Jade\n";
            break;
        case '3':
            gMat = 3;
            setMaterial(gMat);
            cout << "Current Material: Yellow Rubber\n";
            break;
        case '4':
            gMat = 4;
            setMaterial(gMat);
            cout << "Current Material: Blue Matte\n";
            break;
        case '5':
            gMat = 5;
            setMaterial(gMat);
            cout << "Current Material: Pearl\n";
            break;
		case '6':
			gMat = 6;
            setMaterial(gMat);
			cout << "Current Material: Tex 1\n";
			break;
		case '7':
			gMat = 7;
            setMaterial(gMat);
			cout << "Current Material: Tex 2\n";
			break;
		case '8':
			gMat = 8;
            setMaterial(gMat);
			cout << "Current Material: Tex 3\n";
			break;


         //Object Rotation
        case 'z':
         if (objectList.size() > 0 && objSelected < objectList.size())
            objectList.at(objSelected).rotation.x += 2;
            break;

        case 'x':
         if (objectList.size() > 0 && objSelected < objectList.size())
            objectList.at(objSelected).rotation.z += 2;
            break;

        case 'c':
        if (objectList.size() > 0 && objSelected < objectList.size())
            objectList.at(objSelected).rotation.y += 2;
            break;

        //Object translation

        case 'b':
        if (objectList.size() > 0 && objSelected < objectList.size())
            objectList.at(objSelected).translation.x += 0.3;
            break;

        case 'n':
            if (objectList.size() > 0 && objSelected < objectList.size())
            objectList.at(objSelected).translation.y += 0.3;
            break;

        case 'm':
       if (objectList.size() > 0 && objSelected < objectList.size())
            objectList.at(objSelected).translation.z += 0.3;
            break;

        //Object Scaling
        case 'f':
            if (objectList.size() > 0 && objSelected < objectList.size())
            objectList.at(objSelected).scale.x += 0.3;
            break;

        case 'g':
            if (objectList.size() > 0 && objSelected < objectList.size())
            objectList.at(objSelected).scale.y += 0.3;
            break;

        case 'h':
            if (objectList.size() > 0 && objSelected < objectList.size())
            objectList.at(objSelected).scale.z += 0.3;
            break;

		case '`':
			if (lSelected == 0) {
				lSelected = 1;
				cout << "Light 1 Selected \n";
			}
			else {
				lSelected = 0;
				cout << "Light 0 Selected \n";
			}
			break;

		case 'q':
			LightList.at(lSelected).pos[0] += 0.5;
			break;

		case 'w':
			LightList.at(lSelected).pos[1]+= 0.5;
			break;

		case 'e':
			LightList.at(lSelected).pos[2] += 0.5;
			break;

        // NB LOAD SAVE
        case 's':
        {
            string fileNameSave = "";
            bool hasDot;
            while (fileNameSave == ""){
                cout << "Enter a file name: ";
                cin >> fileNameSave;
                
                if (fileNameSave.find('.') != string::npos) {
                    fileNameSave = "";
                    cout << "invalid file name. Try again and make sure you haven't added any '.'s\n";    
                }
                else{
                    fileSave(fileNameSave);
                    cout << "File has been saved!\n";
                    break;
                }                                
            }
    }
    break;
    
	case 'l':
       {
       string fileNameLoad = "";
       while (fileNameLoad == ""){
        cout << "Which file would you like to load?: ";
        cin >> fileNameLoad;
        if (fileNameLoad.find('.') != string::npos) {
                    fileNameLoad = "";
                    cout << "Invalid file name. Try again and make sure you haven't added any '.'s\n";    
                }
                else{
            fileNameLoad = fileNameLoad + ".csv";
            if (FILE *file = fopen(fileNameLoad.c_str(), "r")){
            
            fclose(file);
            for (int j = 0;  j < objectList.size(); j++){
            objectList.erase(objectList.begin() + j);
            }           
            fileLoad(fileNameLoad);
                        cout << "File has been loaded!\n";
                        break;
            }
            else{
            cout << "File doesn't exist\n";
            break;
            }
                }  
          }
       }
       break;
    
    case 'r':
        rPressed = true;
        break;
    } 

     }
}

/* NB saves the data in the vector into a file*/
void fileSave(string fileName){
    fileName = fileName + ".csv"; //creates it as a csv file
    const char *fileNam =  fileName.c_str(); // changes string to char so it can be supported by file handling

    ofstream fileSave;
    fileSave.open (fileNam);
    for (int i = 0; i < objectList.size(); i++){
        fileSave << objectList.at(i).type << ",";
        fileSave << objectList.at(i).oMat << ",";
    fileSave << objectList.at(i).position.x << ",o" <<  objectList.at(i).position.y << "," <<  objectList.at(i).position.z;
    fileSave << "," <<  objectList.at(i).rotation.x << "," <<  objectList.at(i).rotation.y << "," <<  objectList.at(i).rotation.z;
    fileSave << "," << objectList.at(i).translation.x << "," <<  objectList.at(i).translation.y << "," <<  objectList.at(i).translation.z;
    fileSave << "," << objectList.at(i).scale.x << "," <<  objectList.at(i).scale.y << "," <<  objectList.at(i).scale.z << "\n";    
    }
    fileSave.close();
}

/* NB saves the data from the file into the vector*/
void fileLoad(string fileName){
    /* changes string to char and opens file for loading*/
    const char *fileNam =  fileName.c_str();
    ifstream fileLoad;
    fileLoad.open (fileNam);
    string line;

    /*variable declaration*/
    int tempType, tempMat = 0;
    point3D tempPos, tempRot, tempTrans, tempScale = point3D(0,0,0);
    
    string tempTypeStr,tempMatStr, tempRotX, tempRotY, tempRotZ, tempPosX, tempPosY, tempPosZ;
    string tempTransX, tempTransY, tempTransZ, tempScaleX, tempScaleY, tempScaleZ;
    
    while(getline(fileLoad,line)){
        stringstream linestr(line);
        /* checks if the values are available and loads them got the idea from http://www.cplusplus.com/forum/beginner/81096/ */
        if (getline(linestr, tempTypeStr, ',') &&
        getline(linestr, tempMatStr, ',') &&
        getline(linestr, tempPosX, ',') &&
        getline(linestr, tempPosY, ',') &&
        getline(linestr, tempPosZ, ',') &&
        getline(linestr, tempRotX, ',') &&
        getline(linestr, tempRotY, ',') &&
        getline(linestr, tempRotZ, ',') &&
        getline(linestr, tempTransX, ',') &&
        getline(linestr, tempTransY, ',') &&
        getline(linestr, tempTransZ, ',') &&
        getline(linestr, tempScaleX, ',') &&
        getline(linestr, tempScaleY, ',') &&
        getline(linestr, tempScaleZ, ',')){

        /*changes string type to the desired types (int, and floats for point3D) */
        tempType = atoi(tempTypeStr.c_str());
        tempMat =  atoi(tempMatStr.c_str());
        tempPos = point3D(atof(tempPosX.c_str()), atof(tempPosY.c_str()) + 1, atof(tempPosZ.c_str()));
        tempRot = point3D(atof(tempRotX.c_str()), atof(tempRotY.c_str()), atof(tempRotZ.c_str()));
        tempTrans = point3D(atof(tempTransX.c_str()), atof(tempTransY.c_str()), atof(tempTransZ.c_str()));
        tempScale = point3D(atof(tempScaleX.c_str()), atof(tempScaleY.c_str()), atof(tempScaleZ.c_str()));
        
        
        /* creates a new object and pushes into the vector*/
        Object object;

            object.type = tempType;
            object.oMat = tempMat;
            object.position = tempPos;
            object.translation = tempTrans;
            object.scale = tempScale;
                object.rotation = tempRot;

            objectList.push_back(object);
        }


    }
    for (int i = 0; i < objectList.size(); i++){
        drawObject(i);
    }
    
        
     fileLoad.close();
    
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
            sceneRotZ += 2;
            break;

        //Rotate right along x axis
        case GLUT_KEY_DOWN:
            sceneRotZ -= 2;
            break;
    }
}

void idle(){
    //checks if r has been pressed
    if(rPressed == true){
        //deletes all the objects in the list
        for (int j = 0;  j < objectList.size(); j++){
            objectList.erase(objectList.begin() + j);
             }
        //once all objects are deleted, it sets rPressed to false 
        if (objectList.size() == 0){
            rPressed = false;
        }
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
    //displayReadMe();

    //glut initialization stuff:
    // set the window size, display mode, and create the window
    glutInit(&argc, argv);
    glutInitWindowSize(screenSizeX, screenSizeY);
    glutInitWindowPosition(450,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Object Modeler");

    //enable Z buffer test, otherwise things appear in the order they're drawn
    glEnable(GL_DEPTH_TEST);

    //setup the initial view
    // change to projection matrix mode, set the extents of our viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, aspect, 1, 100);
        
    //set background colour to grey
    glClearColor(0.6, 0.5, 0.6, 0);

    //lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	createLights();

	//Textures
	glEnable(GL_TEXTURE_2D);
	//glGenTextures(3, texArray);
	loadTextures();

    //enable culling
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    //register glut callbacks for keyboard and display function
    glutKeyboardFunc(kbd);
    glutDisplayFunc(display);
    glutSpecialFunc(special);
	glutIdleFunc(idle);

    //start the program!
    glutMainLoop();

    return 0;
}