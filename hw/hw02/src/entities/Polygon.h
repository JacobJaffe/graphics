#ifndef POLYGON_H
#define POLYGON_H

#include "Shape.h"

// TODO: consider how to make the class an abstract class
class Polygon : public Shape {
public:
	Polygon(){};
	~Polygon(){};

	void draw(){};
  void drawNormal(){};
};

//TODO: make polygons have protected borderVertexCount

class Rectangle : public Polygon {
public:
  Rectangle() {
    borderVertexList[0] = Point(0.0, 0.0, 0.0);
    borderVertexList[1] = Point(0.0, 0.0, 0.0);
    borderVertexList[2] = Point(0.0, 0.0, 0.0);
    borderVertexList[3] = Point(0.0, 0.0, 0.0);
  }

  Rectangle(Point p0, Point p1, Point p2, Point p3) {
    borderVertexList[0] = Point(p0);
    borderVertexList[1] = Point(p1);
    borderVertexList[2] = Point(p2);
    borderVertexList[3] = Point(p3);

  };

  ~Rectangle() {
    fprintf(stderr, "Rectangle Destructor\n");
  }

  void draw() {

    Point p0 = borderVertexList[0];
    Point p1 = borderVertexList[1];
    Point p2 = borderVertexList[2];
    Point p3 = borderVertexList[3];

    // vector on 'x': p0 -> p1
    Vector vX = p1 - p0;

    // vector on 'y': p2 -> p3
    Vector vY = p3 - p1;

    // make each relative to a scale
    vX = vX / m_segmentsX;
    vY = vY / m_segmentsY;

    Vector normalVector = cross(vY, vX);
    normalVector.normalize();

		// TODO: Fix
			normalVector = -normalVector;
    glNormal3dv(normalVector.unpack());

    for (int i = 0; i < m_segmentsX; i ++) {
      glBegin(GL_TRIANGLE_STRIP); // sets the shapes that openGL draws and determines
                                  // the number of vertices that are necessary
      for (int j = 0; j < m_segmentsY; j++) {

        glVertex3dv(
          (p0 + (vX * i) + (vY * j))
          .unpack() );  // set the three vertices for the triangle
        glVertex3dv(
          (p0 + (vX * (i + 1)) + (vY * j))
          .unpack());  // set the three vertices for the triangle
        glVertex3dv(
          (p0 + (vX * i) + (vY * (j + 1)))
          .unpack());  // set the three vertices for the triangle
        glVertex3dv(
          (p0 + (vX * (i + 1)) + (vY * (j + 1)))
          .unpack());  // set the three vertices for the triangle
      }
      glEnd();
    }
  };

  void drawNormal() {
    Point p0 = borderVertexList[0];
    Point p1 = borderVertexList[1];
    Point p2 = borderVertexList[2];
    Point p3 = borderVertexList[3];

    // vector on 'x': p0 -> p1
    Vector vX = p1 - p0;

    // vector on 'y': p2 -> p3
    Vector vY = p3 - p1;

    // make each relative to a scale
    vX = vX / m_segmentsX;
    vY = vY / m_segmentsY;
    Vector normalVector = cross(vY, vX);
    normalVector.normalize();

		for (int i = 0; i < m_segmentsX + 1; i ++) {
      for (int j = 0; j < m_segmentsY + 1; j++) {
				glBegin(GL_LINES);
					glColor3f(1.0, 0.0, 0.0);
					Point point = (p0 + (vX * i) + (vY * j));
	        glVertex3dv(point.unpack());
					glVertex3dv((point + normalVector / 10).unpack());
				glEnd();
      }
    }
  };

private:
  const static int borderVertexCount = 4;
  Point borderVertexList[borderVertexCount];
};

#endif
