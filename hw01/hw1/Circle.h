#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"
#include "Algebra.h"

class Circle : public Shape {
public:
	Circle();
  Circle(Point initialOrigin, float r);
	~Circle();

  void setEdgesCount(int count);
	void setOrigin(Point origin);
	void setRadius(float radius);
	void draw();
	void draw(Vector &normalVector);
  void drawNormal();
	void drawTangent();
	Vector getTangent(int vertex);

	// circles must have same edge count
	void drawConnectCircleSlope(Circle &otherCircle);
	void drawConnectCircleCurved(Circle &otherCircle, Point &point);
	void drawConnectPoint(Point &point);
	void drawConnectPointCurved(Point &tip, Point &curveOrigin);

  // TODO: make this safe
  Point* vertexList;
	Point origin;

private:
  // TODO: quarternion for angle of Circle
  float radius;
	int edgesCount;
};

#endif
