#include "Sphere.h"
#include <iostream>
#include <fstream>
#include <string>

const int X = 0;
const int Y = 1;
const int Z = 2;

using namespace std;

double Sphere::Intersect(Point p_eye, Vector ray, Matrix transformMatrix) {
  Matrix inverseTransformMatrix = invert(transformMatrix);
  Vector ray__obj = inverseTransformMatrix * ray;
  Point p_eye__obj = inverseTransformMatrix * p_eye;

  double t = DBL_MAX;
  double r = 0.5;
  // Vector v_eye__obj = Vector(p_eye__obj[0], p_eye__obj[1], p_eye__obj[2]);
  //
  // double A = dot(ray__obj, ray__obj);
  // double B = 2 * dot(v_eye__obj, ray__obj);
  // double C = dot(v_eye__obj, v_eye__obj) - (0.5 * 0.5);
  //
  // double foo = (B * B) - (4 * A * C);
  // if (foo < 1) {
  //   return t;
  // }
  //
  // double t_negative = (-B  - sqrt(foo)) / (2 * A);
  // double t_positive = (-B  + sqrt(foo)) / (2 * A);
  //
  // if (t_negative >= 1) {
  //   t = t_negative;
  // }
  //
  // if (t_positive >= 1 && t_positive < t) {
  //   t = t_positive;
  // }

   t = (-p_eye__obj[0]) / ray__obj[0];
   Point p = p_eye__obj + (ray__obj * t);

   if (((p[1] * p[1]) + (p[2] * p[2])) < (r * r)) {
     return t;
   }

   t = (-p_eye__obj[1]) / ray__obj[1];
   Point p2 = p_eye__obj + (ray__obj * t);

   if (((p2[0] * p2[0]) + (p2[2] * p2[2])) < (r * r)) {
     return t;
   }

   t = (-p_eye__obj[2]) / ray__obj[2];
   Point p3 = p_eye__obj + (ray__obj * t);

   if (((p3[0] * p3[0]) + (p3[1] * p3[1])) < (r * r)) {
     return t;
   }

  return DBL_MAX;

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
