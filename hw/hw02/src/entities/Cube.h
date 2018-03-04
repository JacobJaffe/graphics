#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"
#include "Polygon.h"

class Cube : public Shape {
public:
	Cube();
	~Cube();

	// we need this to set the segments of the children primatives
	void setSegments(int x, int y);

	void draw();

	void drawNormal();

private:
	const static int rectangleCount = 6;
	Rectangle* rectangleList;

	const static int vertexCount = 8;
	Point* vertexList;

};

#endif
