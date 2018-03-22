#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "../math/Algebra.h"
#include "Circle.h"

class Sphere : public Shape {
public:
	Sphere();
	~Sphere();

	void draw();

	void drawNormal();

	void setSegments(int x, int y);

private:
	Point origin;
	Point baseVertex;
	Point topVertex;
	float radius;
	Circle* circleList;
};

#endif
