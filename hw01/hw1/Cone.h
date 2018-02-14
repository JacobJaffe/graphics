#ifndef CONE_H
#define CONE_H

#include "Shape.h"
#include "Polygon.h"
#include "Circle.h"

class Cone : public Shape {
public:
	Cone();
	~Cone();

	void draw();
	void setSegments(int x, int y);

	void drawNormal();

private:
	float baseRadius;
	Point tipVertex;
	Point baseVertex;

	Circle* circleList;
};

#endif
