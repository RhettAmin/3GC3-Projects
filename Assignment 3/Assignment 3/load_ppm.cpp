#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>

int width = 0;
int height = 0;


GLubyte *image;


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(width,0);
	glPixelZoom(-1, 1);
    glDrawPixels(width,height,GL_RGB, GL_UNSIGNED_BYTE, image);
	glFlush();
}


void myreshape(int h, int w)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLfloat) width, 0.0, (GLfloat) height);
	glMatrixMode(GL_MODELVIEW);
}

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

void main(int argc, char *argv[])
{
	int k;

	image = LoadPPM("snail_a.ppm", &width, &height, &k); //you can put whatever (ASCII) PPM file you want here

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(width, height);
    glutCreateWindow("image");
    glClearColor (1.0, 1.0, 1.0, 1.0);
	glutReshapeFunc(myreshape);
    glutDisplayFunc(display);
    glutMainLoop();

}
