//Class point3D
//represents a 2D point with floats/floats for x and y components
class point3D{
public:
	point3D();
	point3D(float inX, float inY, float inZ);
	float x;
	float y;
	float z;
};

//Class Vector2D
//represents a 2D vector with floats/floats for x, y and magnitude
class vec3D{
private:
	float magnitude;
public:
	float x;
	float y;
	float z;
	vec3D();
	vec3D(float x, float y, float z);
	float getMagnitude();
	void setMagnitude(float mag);

};

//Class ColourStrcture
//represents a colour using floats/floats for r, g, b components
class colourStruc{
public:
	float r;
	float g;
	float b;
	colourStruc();
	colourStruc(float r, float g, float b);

};

//Class MathLibrary
//Functions within the math
class MathLib3D{
public:
	static float pDistance(point3D p1, point3D p2);
	static point3D fastDistance(point3D p1, point3D p2,point3D p3);
	static float vecLength(vec3D vector);
	static vec3D normalize(vec3D vector);
	static vec3D vectorMultiply(vec3D vector,float scalar);
	static vec3D createVector(point3D p1, point3D p2);
	static point3D movePoint(point3D point, vec3D vector);
};
