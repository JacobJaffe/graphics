#include <string.h>
#include <iostream>
#include <fstream>
#include <GL/glui.h>
#include <math.h>
#include "Cone.h"

Cone::Cone() {
  fprintf(stderr, "Cone Constructor\n");
  circleList = NULL;
  tipVertex = Point(0, 0.5, 0);
  baseVertex = Point(0, -0.5, 0);
  baseRadius = 0.5;
}

void Cone::setSegments(int x, int y) {

  if (m_segmentsY != y) {
    m_segmentsY = y;
    m_segmentsX = x;

    delete[] circleList;
    circleList = new Circle[m_segmentsY];

    Vector baseToTip = tipVertex - baseVertex;

    // divide cone vertically into circles
    for (int i = 0; i < m_segmentsY; i ++) {
      Point origin = baseVertex + (i * baseToTip / float(m_segmentsY));
      float radius = baseRadius - (float(i) * baseRadius / float(m_segmentsY));
      circleList[i] = Circle(origin, radius );

      // If we are making new circles, set their edges either way, so exit after
      circleList[i].setEdgesCount(m_segmentsX);
    }
    return;
  }

  if (m_segmentsX != x) {
    m_segmentsX = x;
    for (int i = 0; i < m_segmentsY; i++) {
      circleList[i].setEdgesCount(m_segmentsX);
    }
  }
}

void Cone::draw() {

  // draw base
  circleList[0].draw();

  // draw the connections of the circles
  for (int i = 0; i < m_segmentsY - 1; i++) {
    circleList[i].drawConnectCircleSlope(circleList[i + 1]);
  }

  // Draw the top circle to the tip
  circleList[m_segmentsY - 1].drawConnectPoint(tipVertex);

}

void Cone::drawNormal() {
  for (int i = 0; i < m_segmentsY; i++) {
    for (int j = 0; j < m_segmentsX; j++) {
      glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        Point point = circleList[i].vertexList[j];

        Vector circleTangentVector = circleList[i].getTangent(j);
        Vector slopeVector = tipVertex - point;

        Vector normalVector = cross(circleTangentVector, slopeVector);
        normalVector.normalize();

        glVertex3dv(
          point
          .unpack() );

          glVertex3dv(
            (point + normalVector / 10)
            .unpack() );
      glEnd();
    }
  }
}

Cone::~Cone() {
  fprintf(stderr, "Cone Deconstructor\n");
  delete [] circleList;
  fprintf(stderr, "Cone Deconstructor -- triangleList deleted\n");
}
