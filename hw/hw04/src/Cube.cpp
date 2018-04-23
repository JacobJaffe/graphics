#include "Cube.h"
#include <iostream>
#include <fstream>
#include <string>

const int X = 0;
const int Y = 1;
const int Z = 2;

using namespace std;

bool isAboutEqual(double a, double b);

double plane_intersect_front(Point p_eye, Vector ray)
{
	double t = DBL_MAX;

	double t_guess = (0.5 - p_eye[Z]) / ray[Z];
	Point p = p_eye + (t_guess * ray);
	if ((p[X] >= -0.5) && (p[X] <= 0.5)) {
		if ((p[Y] >= -0.5) && (p[Y] <= 0.5)) {
			if (t >= (1 - EPSILON)) {
				t = t_guess;
			}
		}
	}
	return t;
}

double plane_intersect_back(Point p_eye, Vector ray)
{
  double t = DBL_MAX;

	double t_guess = (-0.5 - p_eye[Z]) / ray[Z];
	Point p = p_eye + (t_guess * ray);
	if ((p[X] >= -0.5) && (p[X] <= 0.5)) {
		if ((p[Y] >= -0.5) && (p[Y] <= 0.5)) {
			if (t >= 1) {
				t = t_guess;
			}
		}
	}
	return t;
}

double plane_intersect_right(Point p_eye, Vector ray)
{
  double t = DBL_MAX;

	double t_guess = (0.5 - p_eye[X]) / ray[X];
	Point p = p_eye + (t_guess * ray);
	if ((p[Z] >= -0.5) && (p[Z] <= 0.5)) {
		if ((p[Y] >= -0.5) && (p[Y] <= 0.5)) {
			if (t >= 1) {
				t = t_guess;
			}
		}
	}
	return t;
}

double plane_intersect_left(Point p_eye, Vector ray)
{
  double t = DBL_MAX;

	double t_guess = (-0.5 - p_eye[X]) / ray[X];
	Point p = p_eye + (t_guess * ray);
	if ((p[Z] >= -0.5) && (p[Z] <= 0.5)) {
		if ((p[Y] >= -0.5) && (p[Y] <= 0.5)) {
			if (t >= 1) {
				t = t_guess;
			}
		}
	}
	return t;
}

double plane_intersect_top(Point p_eye, Vector ray)
{
  double t = DBL_MAX;

	double t_guess = (0.5 - p_eye[Y]) / ray[Y];
	Point p = p_eye + (t_guess * ray);
	if ((p[Z] >= -0.5) && (p[Z] <= 0.5)) {
		if ((p[X] >= -0.5) && (p[X] <= 0.5)) {
			if (t >= 1) {
				t = t_guess;
			}
		}
	}
	return t;
}

double plane_intersect_bot(Point p_eye, Vector ray)
{
  double t = DBL_MAX;

	double t_guess = (-0.5 - p_eye[Y]) / ray[Y];
	Point p = p_eye + (t_guess * ray);
	if ((p[Z] >= -0.5) && (p[Z] <= 0.5)) {
		if ((p[X] >= -0.5) && (p[X] <= 0.5)) {
			if (t >= 1) {
				t = t_guess;
			}
		}
	}
	return t;
}

double Cube::Intersect(Point p_eye, Vector ray, Matrix transformMatrix) {
  Matrix inverseTransformMatrix = invert(transformMatrix);
  ray = inverseTransformMatrix * ray;
  p_eye = inverseTransformMatrix * p_eye;

  double t_front = plane_intersect_front(p_eye, ray);
  double t_back = plane_intersect_back(p_eye, ray);
  double t_right = plane_intersect_right(p_eye, ray);
  double t_left = plane_intersect_left(p_eye, ray);
  double t_top = plane_intersect_top(p_eye, ray);
  double t_bot = plane_intersect_bot(p_eye, ray);

  return min( min( min(t_front, t_back), min(t_right, t_left)), min(t_top, t_bot)) ;
};

bool isAboutEqual(double a, double b) {
	double fake_epsilon = 0.01;
	double c = (a - b);
	c = c < 0 ? -c : c;
	bool is_about_equal =  c < fake_epsilon;
	return is_about_equal;
}

Vector Cube::findIsectNormal(Point p_eye, Vector ray, double dist)
{

  if (dist < 1) {
    std::cerr << "ERROR: findIsectNormal called but dist < 1" << std::endl;
  }

  Vector normal = Vector(0, 0, 0);
  Point p = p_eye + (dist * ray);

  // TODO: this is grainy, becuase there is a slight epsillion rounding error.
  if (isAboutEqual(p[X], -0.5)) {
    normal[X] = -1;
  } else if (isAboutEqual(p[X], 0.5)) {
    normal[X] = 1;
	} else if (isAboutEqual(p[Y], -0.5)) {
    normal[Y] = -1;
	} else if (isAboutEqual(p[Y], 0.5)) {
    normal[Y] = 1;
	} else if (isAboutEqual(p[Z], -0.5)) {
    normal[Z] = -1;
	} else if (isAboutEqual(p[Z], 0.5)) {
    normal[Z] = 1;
  } else {
    std::cerr << "P: " << p[X] << ", " << p[Y] << ", " << p[Z] << std::endl;
  }

  normal.normalize();
  return normal;
}
