#include <string.h>
#include <iostream>
#include <fstream>
#include <GL/glui.h>
#include <math.h>
#include "Sphere.h"

Sphere::Sphere() {
  fprintf(stderr, "Sphere Constructor\n");
  circleList = NULL;
  baseVertex = Point(0, -0.5, 0);
  topVertex = Point(0, 0.5, 0);
  origin = Point(0, 0, 0);
  radius = 0.5;
}

void Sphere::setSegments(int x, int y) {
  if (m_segmentsY != y) {
    m_segmentsY = y;
    m_segmentsX = x;

    delete[] circleList;
    circleList = new Circle[m_segmentsY - 1];

    Vector baseToTip = topVertex - baseVertex;

    Matrix rotationMatrix = rotZ_mat(PI / m_segmentsY);
    Vector v = baseVertex - origin;



    // divide sphere vertically into circles, but we gotta be careful with counting, as both ends are tips
    for (int i = 0; i < m_segmentsY - 1; i ++) {
      v = rotationMatrix * v;
      Point origin = Point(0, v[1], 0);
      circleList[i] = Circle(origin, v[0]);
      circleList[i].setEdgesCount(m_segmentsX);
    }
    return;
  }

  if (m_segmentsX != x) {
    m_segmentsX = x;
    for (int i = 0; i < m_segmentsY - 1; i++) {
      circleList[i].setEdgesCount(m_segmentsX);
    }
  }
}

// TODO: fix bottom of sphere rendering
void Sphere::draw() {
  for (int i = 0; i < m_segmentsY - 2; i++) {
    circleList[i].drawConnectCircleCurved(circleList[i+1], origin);
  }

  // draw tips
  circleList[0].drawConnectPointCurved(baseVertex, origin);
  circleList[m_segmentsY-2].drawConnectPointCurved(topVertex, origin);
}

void Sphere::drawNormal() {
  for (int i = 0; i < m_segmentsY - 1; i++) {
    for (int j = 0; j < m_segmentsX; j++) {
      glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        Point point = circleList[i].vertexList[j];
        Vector normalVector = point - origin;
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

  // top normal
  glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    Vector topNormalVector = topVertex - origin;
    topNormalVector.normalize();
    glVertex3dv(topVertex.unpack() );

    glVertex3dv((topVertex + topNormalVector / 10).unpack() );
  glEnd();

  // bottom normal
  glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    Vector baseNormalVector = baseVertex - origin;
    baseNormalVector.normalize();
    glVertex3dv(baseVertex.unpack() );

    glVertex3dv((baseVertex + baseNormalVector / 10).unpack() );
  glEnd();
}

Sphere::~Sphere() {
  fprintf(stderr, "Sphere Deconstructor\n");
  delete [] circleList;
  fprintf(stderr, "Sphere Deconstructor -- circleList deleted\n");
}
