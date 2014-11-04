//Class Point2D
//represents a 2D point with floats/floats for x and y components
class point2D{
public:
	point2D();
	point2D(float inX, float inY);
	float x;
	float y;
};

//Class Vector2D
//represents a 2D vector with floats/floats for x, y and magnitude
class vec2D{
private:
	float magnitude;
public:
	float x;
	float y;
	vec2D();
	vec2D(float x, float y);
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
class MathLib2D{
public:
	static float pDistance(point2D p1, point2D p2);
	static point2D fastDistance(point2D p1,point2D p2,point2D p3);
	static float vecLength(vec2D vector);
	static vec2D normalize(vec2D vector);
	static vec2D vectorMultiply(vec2D vector,float scalar);
	static vec2D createVector(point2D p1, point2D p2);
	static point2D movePoint(point2D point, vec2D vector);
};
