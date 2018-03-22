

#ifndef CAMERA_H
#define CAMERA_H

#include "./math/Algebra.h"
#include <cstdio>
#include <iostream>

class Camera {
public:
  Camera();
  ~Camera();
  void Orient(Point& eye, Point& focus, Vector& up);
  void Orient(Point& eye, Vector& look, Vector& up);

  Matrix GetProjectionMatrix();
  Matrix GetModelViewMatrix();

  void RotateV(double angle);
  void RotateU(double angle);
  void RotateW(double angle);
  void Rotate(Point p, Vector axis, double degree);
  void Translate(const Vector &v);

  // Setters
  void SetViewAngle (double viewAngle);
  void SetNearPlane (double nearPlane);
  void SetFarPlane (double farPlane);
  void SetScreenSize (int screenWidth, int screenHeight);

  // Getters
  Point GetEyePoint();
  Vector GetLookVector();
  Vector GetUpVector();
  double GetViewAngle();
  double GetNearPlane();
  double GetFarPlane();
  int GetScreenWidth();
  int GetScreenHeight();

    // Added in HW03
    void Reset();

  double GetFilmPlaneDepth();
  double GetScreenWidthRatio();

private:
  Point _eyePoint;
  double _viewAngle;
  double _nearPlane;
  double _farPlane;
  int _screenWidth;
  int _screenHeight;
  Vector _upVector;
  Vector _lookVector;

  double _rotateV;
  double _rotateU;
  double _rotateW;

};
#endif
