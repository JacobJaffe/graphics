#include <string.h>
#include "Algebra.h"
#include <GL/glui.h>

void print_point(Point p) {
  fprintf(stderr, "POINT: %f, %f, %f\n", p[0], p[1], p[2]);
};

int main(int argc, char* argv[])
{
  fprintf(stderr, "START ALGEBRA TEST\n------------------\n");

  Point p = Point(1, 1, 1);
  print_point(p);

  fprintf(stderr, "\nRot 45 X:\n");
  Matrix mx_45 = rotX_mat(PI/4);
  Point p_45x = mx_45 * p;
  print_point(p_45x);

  fprintf(stderr, "\nRot 45 Y:\n");
  Matrix my_45 = rotY_mat(PI/4);
  Point p_45y = my_45 * p;
  print_point(p_45y);

  fprintf(stderr, "\nRot 45 Z:\n");
  Matrix mz_45 = rotZ_mat(PI/4);
  Point p_45z = mz_45 * p;
  print_point(p_45z);

  // 3.1
  // Rotate about an arbitrary Vector
  float x = -0.8;
  float y = 2;
  float z = 1;
  Vector v = Vector(x, y, z);

  // Use 2 matricies:
  //  a) rotate rotY_mat to make on XY plane
  //  b) rotate rotZ_mat to make on X axis

  // 1) find angle to the XY plane (same as z to x in 2D)
  Vector x_axis = Vector(1, 0, 0);
  Vector v_zx = Vector(v[0], 0, v[2]);
  float angle_to_XY = acos(dot(v_zx, x_axis) / length(v_zx));
  fprintf(stderr, "Angle to XY: %f\n", angle_to_XY);
  Matrix m_to_XY = rotY_mat(angle_to_XY);

  Matrix 

  fprintf(stderr, "\n------------------\nEND ALGEBRA TEST\n");
  return 0;
}
