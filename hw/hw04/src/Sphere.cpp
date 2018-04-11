#include "Sphere.h"
#include <iostream>
#include <fstream>
#include <string>

const int X = 0;
const int Y = 1;
const int Z = 2;

using namespace std;

double Sphere::Intersect(Point p_eye_world, Vector ray_world, Matrix transformMatrix) {
  Matrix inverseTransformMatrix = invert(transformMatrix);
  Vector ray = inverseTransformMatrix * ray_world;
  Point eye = inverseTransformMatrix * p_eye_world;

  double t = DBL_MAX;
  double r = 0.5;

  double A = (ray[0] * ray[0]) + (ray[1] * ray[1]) + (ray[2] * ray[2]);
  double B = 2 * ((ray[0] * eye[0]) + (ray[2] * eye[2]) + (ray[1] * eye[1]));
  double C = (eye[0] * eye[0]) + (eye[1] * eye[1]) + (eye[2] * eye[2]) - (r * r);

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


  return t;
};

Vector Sphere::findIsectNormal(Point p_eye, Vector ray, double dist)
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
