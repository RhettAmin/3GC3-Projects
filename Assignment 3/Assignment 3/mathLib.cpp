/* 
Author: Noel Brett
Student Number: 1130789
File: mathLib.cpp
-----------------------------
description and constructor for the header file.
Used for behaviour of particles in the main.cpp particle 3d fountain system
*/

#include "mathLib.h"
#include <math.h>

point3D::point3D() {
	x = 0;
	y = 0;
	z = 0;
};

point3D::point3D(float inX, float inY, float inZ) {
	x = inX;
	y = inY;
	z = inZ;
};

vec3D::vec3D() {
	x = 0;
	y = 0;
	z = 0;
};

vec3D::vec3D(float inX, float inY, float inZ) {
	x = inX;
	y = inY;
	z = inZ;
};

float vec3D::magnitude() {
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z,2));
	
};
colour::colour(){
	r = 0;
	g = 0;
	b = 0;
};

colour::colour(float inR, float inG, float inB){
	r = inR;
	g = inG;
	b = inB;
};

_3DMathLib::_3DMathLib(){
};

float _3DMathLib:: distance(point3D start, point3D end) {
	return sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2) + pow(end.z - start.z,2));
};
float _3DMathLib:: fastDist(point3D p1, point3D p2){
	return pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z,2);
};

float _3DMathLib:: length(vec3D vector){
	return vector.magnitude();
};

vec3D _3DMathLib:: normalize(vec3D vector){
	float vectorLen, x, y, z;
	vec3D vecPoint;
	vectorLen = vector.magnitude();
	x = (vector.x/vectorLen);
	y = (vector.y/vectorLen);
	z = (vector.z/vectorLen);
	vecPoint = vec3D(x,y,z);	
	return vecPoint;
};

vec3D _3DMathLib:: vectorMultiply (vec3D vector, float scalar){
	float x, y, z;
	vec3D vecPoint;
	x = vector.x * scalar;
	y = vector.y * scalar;
	z = vector.z * scalar;
	vecPoint = vec3D(x,y,z);
	return vecPoint;
};

vec3D _3DMathLib:: createVector (point3D p1, point3D p2){
	float x, y, z;
	vec3D newVector;
	newVector = vec3D(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
	return newVector;
	
};

point3D _3DMathLib:: movePoint(point3D point, vec3D vector){
	float x, y, z;
	point3D newPoint;
	x = vector.x + point.x;
	y = vector.y + point.y;
	z = vector.z + point.z;	
	return newPoint = point3D(x,y,z);
};
vec3D _3DMathLib:: crossProduct(vec3D p1, vec3D p2){
	float x, y, z;
	x = ((p1.y * p2.z) - (p2.y * p1.z)); 
	y = ((p1.x * p2.z) - (p2.x * p1.z));
	z = ((p1.x * p2.y) - (p2.x * p1.y));
	return normalize(vec3D(x,y,z));

};

float _3DMathLib:: dotProduct(point3D p1, point3D p2){
	float x, y, z;
	x = p1.x * p2.x;
	y = p1.y * p2.y;
	z = p1.z * p2.z;
	return x + y + z;

};

point3D _3DMathLib:: addPoints(point3D p1, point3D p2){
	float x, y, z;
	x = p1.x + p2.x;
	y = p1.y + p2.y;
	z = p1.z + p2.z;
	return point3D(x,y,z);

};

point3D _3DMathLib:: pointMultiply(point3D point, float scalar){
	float x, y, z;
	x = point.x * scalar;
	y = point.y * scalar;
	z = point.z * scalar;
	return point3D(x,y,z);

};

