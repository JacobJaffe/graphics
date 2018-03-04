#include <string.h>
#include <iostream>
#include <fstream>
#include <GL/glui.h>
#include <math.h>
#include "Cylinder.h"

Cylinder::Cylinder() {
  fprintf(stderr, "Cylinder Constructor\n");
  circleList = NULL;
  baseVertex = Point(0, -0.5, 0);
  topVertex = Point(0, 0.5, 0);
  radius = 0.5;
}

void Cylinder::setSegments(int x, int y) {
  if (m_segmentsY != y) {
    m_segmentsY = y;
    m_segmentsX = x;

    delete[] circleList;
    circleList = new Circle[m_segmentsY + 1];

    Vector baseToTop = topVertex - baseVertex;

    // divide cylinder vertically into circles
    for (int i = 0; i < m_segmentsY + 1; i ++) {
      Point origin = baseVertex + (i * baseToTop / float(m_segmentsY));
      circleList[i] = Circle(origin, radius );

      // If we are making new circles, set their edges either way, so exit after
      circleList[i].setEdgesCount(m_segmentsX);
    }
    return;
  }

  if (m_segmentsX != x) {
    m_segmentsX = x;
    for (int i = 0; i < m_segmentsY + 1; i++) {
      circleList[i].setEdgesCount(m_segmentsX);
    }
  }
}


void Cylinder::draw() {
  // draw base
  Vector bottomNormal = Vector(0, -1, 0);
  circleList[0].draw(bottomNormal);

  for (int i = 0; i < m_segmentsY; i++) {
    circleList[i].drawConnectCircleSlope(circleList[i + 1]);
  }

  // draw top
  Vector topNormal = Vector(0, 1, 0);
  circleList[m_segmentsY].draw(topNormal);
}

void Cylinder::drawNormal() {
  for (int i = 0; i < m_segmentsY + 1; i++) {
    circleList[i].drawNormal();
  }
}

Cylinder::~Cylinder() {
  fprintf(stderr, "Cylinder Deconstructor\n");
  delete [] circleList;
  fprintf(stderr, "Cylinder Deconstructor -- circleList deleted\n");
}
