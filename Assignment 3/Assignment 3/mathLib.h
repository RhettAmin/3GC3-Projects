/* 
Author: Noel Brett
student number: 1137089
File: mathLib.h
Description: math library header file

*/
#ifndef _H_3DMATHLIB_
#define _H_3DMATHLIB_

class point3D {
	public:
		float x;
		float y;
		float z;
		point3D();
		point3D(float inX, float inY, float inZ);

};

class vec3D{
	public:
		float x;
		float y;
		float z;
		float magnitude();
		vec3D();
		vec3D(float inX, float inY, float inZ);
};

class colour{
	public:
		float r;
		float g;
		float b;
		colour();
		colour(float inR, float inG, float inB);

		
};

class _3DMathLib {
	public:
		_3DMathLib();
		static vec3D crossProduct (vec3D p1, vec3D p2);
		static float dotProduct (point3D p1, point3D p2);
		static float distance (point3D start, point3D end);
		static float fastDist (point3D p1, point3D p2);
		static float length (vec3D vector);
		static vec3D normalize (vec3D vector);
		static vec3D vectorMultiply (vec3D vector, float scalar);
		static vec3D createVector (point3D p1, point3D p2);
		static point3D movePoint (point3D point, vec3D vector);
		static point3D addPoints (point3D p1, point3D p2);
		static point3D pointMultiply (point3D point, float scalar);

};

#endif
