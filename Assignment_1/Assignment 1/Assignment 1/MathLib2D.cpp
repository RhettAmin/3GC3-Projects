#include "MathLib2D.h"
#include <cmath>

point2D::point2D(){
	x = 0;
	y = 0;
}
point2D::point2D(float inX,float inY){
	x = inX;
	y = inY;
}

vec2D::vec2D(){
	x = 0;
	y = 0;
}

vec2D::vec2D(float inX,float inY){
	x = inX;
	y = inY;
	magnitude = abs (sqrt( pow(x,2) + pow(y,2)));
};

float vec2D::getMagnitude(){
	return magnitude;
};
void vec2D::setMagnitude(float mag){
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


float MathLib2D::pDistance(point2D p1, point2D p2){
	float distance = sqrt (pow((p2.x-p1.x),2)+pow((p2.y-p1.y),2));
	return distance;
};

point2D MathLib2D::fastDistance(point2D p1,point2D p2,point2D p3){
	// comparing points p2 and p3 relative to p1
	float est2_1;
	float est3_1;

	est2_1 = pow((p2.x-p1.x),2)+pow((p2.y-p1.y),2);
	est3_1 = pow((p3.x-p1.x),2)+pow((p3.y-p1.y),2);

	if(est2_1 > est3_1){
		return(p3);
	} else{
		return(p2);
	}
};

float MathLib2D::vecLength(vec2D vector){
	float x = vector.x;
	float y = vector.y;

	float result = abs (sqrt( pow(x,2) + pow(y,2)));
	vector.setMagnitude(result);
	return (result);
};

vec2D MathLib2D::normalize(vec2D vector){
	float x = vector.x/vector.getMagnitude();
	float y = vector.y/vector.getMagnitude();

	vec2D returnVector = vec2D(x,y);
	return(returnVector);
};

vec2D MathLib2D::vectorMultiply(vec2D vector,float scalar){
	float x = vector.x * scalar;
	float y = vector.y * scalar;

	vec2D returnVec = vec2D(x,y);
	return (returnVec);
};

vec2D MathLib2D::createVector(point2D p1, point2D p2){
	float x = p2.x-p1.x;
	float y = p2.y-p1.y;
	vec2D returnVec = vec2D(x,y);
	return(returnVec);
};

point2D MathLib2D::movePoint(point2D point, vec2D vector){
	float x = point.x+vector.x;
	float y = point.y+vector.y;
	point2D returnPoint = point2D(x,y);
	return(returnPoint);
};