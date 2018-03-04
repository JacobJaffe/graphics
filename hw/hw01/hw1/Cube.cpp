#include <string.h>
#include <iostream>
#include <fstream>
#include <GL/glui.h>
#include <math.h>
#include "Cube.h"
#include "Polygon.h"

// Helper function
Rectangle rectangleFromVerticies(Point *vertexList, int a, int b, int c, int d) {
  return Rectangle(vertexList[a], vertexList[b], vertexList[c], vertexList[d]);
}

Cube::Cube() {
  fprintf(stderr, "Cube Constructor\n");

  // First, the corners of the cube
  vertexList = new Point[vertexCount];

  // Start with the front facing side
  vertexList[0] = Point(-0.5, 0.5, 0.5);
  vertexList[1] = Point(0.5, 0.5, 0.5);
  vertexList[2] = Point(-0.5, -0.5, 0.5);
  vertexList[3] = Point(0.5, -0.5, 0.5);

  // Now the back side
  vertexList[4] = Point(-0.5, 0.5, -0.5);
  vertexList[5] = Point(0.5, 0.5, -0.5);
  vertexList[6] = Point(-0.5, -0.5, -0.5);
  vertexList[7] = Point(0.5, -0.5, -0.5);

  // Now, the rectangles from those corners
  rectangleList = new Rectangle[rectangleCount];
  rectangleList[0] = rectangleFromVerticies(vertexList, 0, 1, 2, 3);
  // rectangleList[1] = rectangleFromVerticies(vertexList, 1, 5, 3, 7);
  // rectangleList[2] = rectangleFromVerticies(vertexList, 5, 4, 7, 6);
  // rectangleList[3] = rectangleFromVerticies(vertexList, 4, 0, 6, 2);
  // rectangleList[4] = rectangleFromVerticies(vertexList, 4, 5, 0, 1);
  // rectangleList[5] = rectangleFromVerticies(vertexList, 2, 3, 6, 7);

}

void Cube::setSegments(int x, int y) {
  m_segmentsX = x;
  m_segmentsY = y;
  for (int i = 0; i < 1; i++) {
    rectangleList[i].setSegments(x, y);
  }
}

void Cube::draw() {
  for (int i = 0; i < 1; i++) {
    rectangleList[i].draw();
  }
}

void Cube::drawNormal() {
  for (int i = 0; i < 1; i++) {
    rectangleList[i].drawNormal();
  }
};

Cube::~Cube() {
  fprintf(stderr, "Cube Deconstructor\n");
  delete [] rectangleList;
  fprintf(stderr, "Cube Deconstructor -- rectangleList deleted\n");
  delete [] vertexList;

}
