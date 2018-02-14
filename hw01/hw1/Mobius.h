#ifndef MOBIUS_H
#define MOBIUS_H

#include "Shape.h"
#include "Algebra.h"

class Mobius : public Shape {
public:
	Mobius(){};
	~Mobius(){};

	void draw();

	void drawNormal(){};
};

#endif
