#include "Cone.h"
#include <iostream>
#include <fstream>
#include <string>

const int X = 0;
const int Y = 1;
const int Z = 2;

using namespace std;

double checkCap(Point eye, Vector ray) {
  double cap = (-0.5 - eye[1]) / ray[1];

  double t = DBL_MAX;
  double r = 0.5;

  Point p_intersect = eye + (cap) * ray;
  if ( (p_intersect[0] * p_intersect[0])  + (p_intersect[2] * p_intersect[2]) < (r * r)) {
      t = cap;
  }
  return t;
}

double Cone::Intersect(Point p_eye_world, Vector ray_world, Matrix transformMatrix) {
  Matrix inverseTransformMatrix = invert(transformMatrix);
  Vector ray = inverseTransformMatrix * ray_world;
  Point eye = inverseTransformMatrix * p_eye_world;

  double t = DBL_MAX;
  double r = 0.5;

  // double A =
  //   ((ray[0] * ray[0]) + (ray[2] * ray[2])  - ((r * r) * (ray[1] * ray[1])));
  //
  // double B =
  //   2 * ray[0] + 2 * ray[2] - ((r * r) * ray[1]);
  //
  //
  // double C =
  //   ((eye[0] * eye[0]) - 2 * eye[0])
  //   + ((eye[2] * eye[2]) - 2 * eye[2])
  //   - (((r * r * r * r) + (r * r));

  double A = (ray[0] * ray[0]) + (ray[2] * ray[2]) - ((r * r) * (ray[1] * ray[1]));
  double B = 2 * ((ray[0] * eye[0]) + (ray[2] * eye[2])) - ((r * r) * ray[1]);
  double C = (eye[0] * eye[0]) + (eye[2] * eye[2]) - (((r * r * r * r) + (r * r)));

  double foo = (B * B) - (4 * A * C);
  if (foo < 0) {
    return t;
  }

  double t_negative = (-B  - sqrt(foo)) / (2 * A);
  double t_positive = (-B  + sqrt(foo)) / (2 * A);

  if (t_negative >= 1) {
    t = t_negative;
  }

  if (t_positive >= 1 && t_positive < t) {
    t = t_positive;
  }

  Point p = eye + (t * ray);
  if (p[1] > r or p[1] < -r) {
    return checkCap(eye, ray);
  }

  return t;
};


// TODO:
Vector Cone::findIsectNormal(Point p_eye, Vector ray, double dist)
{

  if (dist < 1) {
    std::cerr << "ERROR: findIsectNormal called but dist < 1" << std::endl;
  }

  Vector normal = Vector(0, 0, 0);
  Point p = p_eye + (dist * ray);

  Point p_origin;
  normal = p - p_origin;

  normal.normalize();
  return normal;
}
