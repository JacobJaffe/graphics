#include <string.h>
#include <iostream>
#include <fstream>
#include <GL/glui.h>
#include <math.h>
#include "Circle.h"
#include <fstream>

// TODO: circles really need have the normal specified as part of it

Circle::Circle() {
  fprintf(stderr, "Circle Constructor\n");
  vertexList = NULL;
  radius = 0.5f;
  origin = Point(0.0, 0.0, 0.0);
  edgesCount = -1;
}

Circle::Circle(Point initialOrigin, float r) {
  fprintf(stderr, "Circle Constructor (overloaded)\n");
  vertexList = NULL;
  radius = r;
  origin = Point(initialOrigin);
  edgesCount = -1;
}

void Circle::setEdgesCount(int newEdgesCount) {
  // TODO: checks like this on all functions for not needlessly computing stuff.
  if (edgesCount != newEdgesCount) {
    edgesCount = newEdgesCount;
    Vector vUp = Vector(0, 1, 0);
    // calculate rotation matrix around center of circle (this assumes circle is on XZ axis)
    Matrix rotationMatrix = rot_mat(origin, vUp, double((2 * PI) / float(edgesCount)));

    delete[] vertexList;
    vertexList = new Point[edgesCount];

    // set first vertex
    vertexList[0] = Point(radius, origin[1], 0);

    // compute rest by rotating succesively
    for (int i = 1; i < edgesCount; i++) {
      vertexList[i] = rotationMatrix * vertexList[i - 1];
    }
  }
}

void Circle::setOrigin(Point o) {
  origin = o;
}

void Circle::setRadius(float r) {
  radius = r;
}

void Circle::drawNormal() {
  for (int i = 0; i < edgesCount; i++) {
    Point point = vertexList[i];
    Vector normalVector = point - origin;
    glBegin(GL_LINES);
      glColor3f(1.0, 0.0, 0.0);

      glVertex3dv(
        (point)
        .unpack() );

      glVertex3dv(
        (point + normalVector / 10)
        .unpack() );
    glEnd();
  }
};

// TODO: this might not draw the last tangent in a circle-- should check that
void Circle::drawTangent() {
  for (int i = 0; i < edgesCount; i++) {
    Point point = vertexList[i];
    Vector tangentVector = this->getTangent(i);
    glBegin(GL_LINES);
      glColor3f(1.0, 0.0, 0.0);

      glVertex3dv(
        (point - tangentVector / 20)
        .unpack() );

      glVertex3dv(
        (point + tangentVector / 20)
        .unpack() );
    glEnd();
  }
}

Vector Circle::getTangent(int vertex) {
  Point point = vertexList[vertex];
  Vector normalVector = point - origin;
  Vector tangentVector = cross(Vector(0, 1, 0), normalVector);
  tangentVector.normalize();
  return tangentVector;
}

void Circle::draw() {
  fprintf(stderr, "Circle::draw --You probably want to specify the normal vector\n");
  Vector normal = Vector(0, 1, 0);
  this->draw(normal);
}

void Circle::draw(Vector &normalVector) {
  glNormal3dv(normalVector.unpack());
  glBegin(GL_TRIANGLE_FAN);
     glVertex3dv(origin.unpack());
     for (int i = 0; i < edgesCount; i++) {
       glVertex3dv(vertexList[i].unpack());
     }

     // connect the circle
     glVertex3dv(vertexList[0].unpack());
  glEnd();
}

// For connecting two circles (OF THE SAME NUMBER OF EDGES) together with triangles
void Circle::drawConnectCircleSlope(Circle &otherCircle) {

  glBegin(GL_TRIANGLE_STRIP);
  for (int i = 0; i < edgesCount + 1; i++) {
    // to get the last time around also (TODO: just add at the end of loop)
    int index = i % edgesCount;

    Point p1 = vertexList[index];
    Point p2 = otherCircle.vertexList[index];

    // Compute normal for each point; they SHOULD BE THE SAME FOR BOTH (we are going 'up' the slope)
    Vector slopeVector = p2 - p1;
    Vector tangentVector = this->getTangent(index);
    Vector normalVector = cross (tangentVector, slopeVector);
    normalVector.normalize();
    glNormal3dv(normalVector.unpack());

     // draw each vertex
     glVertex3dv(p1.unpack());
     glVertex3dv(p2.unpack());
  }

  glEnd();
}

// For connecting two circles together, on a curved surface (think sphere)
void Circle::drawConnectCircleCurved(Circle &otherCircle, Point &center) {
  glBegin(GL_TRIANGLE_STRIP);
  for (int i = 0; i < edgesCount + 1; i++) {
    // to get the last time around also (TODO: just add at the end of loop)
    int index = i % edgesCount;

    Point p1 = vertexList[index];
    Point p2 = otherCircle.vertexList[index];

    Vector normalVector1 = p1 - center;
    Vector normalVector2 = p2 - center;
    normalVector1.normalize();
    normalVector2.normalize();

    glNormal3dv(normalVector1.unpack());
    glVertex3dv(p1.unpack());

    glNormal3dv(normalVector2.unpack());
    glVertex3dv(p2.unpack());
  }

  glEnd();
}

void Circle::drawConnectPoint(Point &tip) {
  Vector pointNormal = tip - origin;
  glBegin(GL_TRIANGLE_FAN);

    // normal for the point
    glNormal3dv(pointNormal.unpack());

    // Connect to the point
    glVertex3dv(tip.unpack());

    for (int i = 0; i < edgesCount + 1; i++) {
      int index = i % edgesCount;
      Point p1 = vertexList[index];
      Vector slopeVector = tip - p1;
      Vector tangentVector = this->getTangent(index);
      Vector normalVector = cross (tangentVector, slopeVector);
      normalVector.normalize();
      glNormal3dv(normalVector.unpack());

      glVertex3dv(p1.unpack());
    }
  glEnd();
}

void Circle::drawConnectPointCurved(Point &tip, Point &curveOrigin) {
  Vector pointNormal = tip - origin;
  glBegin(GL_TRIANGLE_FAN);

    // normal for the point
    glNormal3dv(pointNormal.unpack());

    // Connect to the point
    glVertex3dv(tip.unpack());

    for (int i = 0; i < edgesCount + 1; i++) {
      int index = i % edgesCount;
      Point p1 = vertexList[index];
      Vector normalVector = p1 - curveOrigin;
      normalVector.normalize();
      glNormal3dv(normalVector.unpack());

      glVertex3dv(p1.unpack());
    }
  glEnd();
}

Circle::~Circle() {
  fprintf(stderr, "Circle Deconstructor\n");
  delete [] vertexList;
}
