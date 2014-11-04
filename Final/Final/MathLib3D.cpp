#include "MathLib3D.h"
#include <iostream>
#include <cmath>

point3D::point3D(){
	x = 0;
	y = 0;
	z = 0;
}
point3D::point3D(float inX,float inY, float inZ){
	x = inX;
	y = inY;
	z = inZ;
}

vec3D::vec3D(){
	x = 0;
	y = 0;
	z = 0;
}

vec3D::vec3D(float inX,float inY, float inZ){
	x = inX;
	y = inY;
	z = inZ;
	magnitude = abs (sqrt(pow(x,2) + pow(y,2) + pow(z,2)));
};

float vec3D::getMagnitude(){
	return magnitude;
};
void vec3D::setMagnitude(float mag){
	magnitude = mag;
};


colourStruc::colourStruc(){
	r = 0; 
	g = 0;
	b = 0;
}
colourStruc::colourStruc(float inR, float inG, float inB){
	r = inR;
	g = inG;
	b = inB;
}


float MathLib3D::pDistance(point3D p1, point3D p2){
	float distance = sqrt ( pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2) + pow((p2.z - p1.z), 2) );
	return distance;
};

float MathLib3D::pDistance(point3D *p1, point3D *p2){
	float distance = sqrt ( pow((p2->x - p1->x), 2) + pow((p2->y - p1->y), 2) + pow((p2->z - p1->z), 2) );
	return distance;
};

point3D MathLib3D::fastDistance(point3D p1,point3D p2,point3D p3){
	// comparing points p2 and p3 relative to p1
	float est2_1;
	float est3_1;
	 
	est2_1 = pow((p2.x - p1.x),2) + pow((p2.y - p1.y),2) + pow((p2.z - p1.z), 2);
	est3_1 = pow((p3.x - p1.x),2) + pow((p3.y - p1.y),2) + pow((p3.z - p1.z), 2);

	if(est2_1 > est3_1){
		return(p3);
	} else{
		return(p2);
	}
};

float MathLib3D::vecLength(vec3D vector){
	float x = vector.x;
	float y = vector.y;
	float z = vector.z;

	float result = abs ( sqrt( pow(x,2) + pow(y,2) + pow(z,2) ) );
	vector.setMagnitude(result);
	return (result);
};

vec3D MathLib3D::normalize(vec3D vector){
	float x = vector.x/vector.getMagnitude();
	float y = vector.y/vector.getMagnitude();
	float z = vector.z/vector.getMagnitude();

	vec3D returnVector = vec3D(x,y,z);
	return(returnVector);
};

vec3D MathLib3D::vectorMultiply(vec3D vector,float scalar){
	float x = vector.x * scalar;
	float y = vector.y * scalar;
	float z = vector.z * scalar;

	vec3D returnVec = vec3D(x,y,z);
	return (returnVec);
};

vec3D MathLib3D::createVector(point3D p1, point3D p2){
	float x = p2.x-p1.x;
	float y = p2.y-p1.y;
	float z = p2.z-p1.z;
	vec3D returnVec = vec3D(x,y,z);
	return(returnVec);
};

point3D MathLib3D::movePoint(point3D point, vec3D vector){
	float x = point.x + vector.x;
	float y = point.y + vector.y;
	float z = point.z + vector.z;
	point3D returnPoint = point3D(x,y,z);
	return(returnPoint);
};

int MathLib3D::randInt(int min, int max) {
	return (rand() % (max - min) + min);
};

float MathLib3D::randFloat(float min, float max) {
	return (( (float) rand() / (float) RAND_MAX * (max - min)) + min);
};