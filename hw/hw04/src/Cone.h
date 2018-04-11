#ifndef CONE_H
#define CONE_H

#include "Shape.h"

class Cone : public Shape {
public:
	Cone() {};
	~Cone() {};

	void draw() {
	};

	void drawNormal() {
	};

	double Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix);

	Vector findIsectNormal(Point eyePoint, Vector ray, double dist);
};

#endif
