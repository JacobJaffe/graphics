#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"
#include "Circle.h"
#include "Polygon.h"

class Cylinder : public Shape {
public:
	Cylinder();
	~Cylinder();

	void draw();
	void setSegments(int x, int y);

	void drawNormal();

private:
	Circle* circleList;
	float radius;
	Point baseVertex;
	Point topVertex;

};
#endif
