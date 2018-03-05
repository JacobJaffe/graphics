#include "Camera.h"
#include <stdio.h>

// TODO
Camera::Camera() {
}

// TODO
Camera::~Camera() {
}

// TODO
void Camera::Orient(Point& eye, Point& focus, Vector& up) {
}

void Camera::Orient(Point& eye, Vector& look, Vector& up) {
	_eyePoint = Point(eye); // ensure copy
	_lookVector = Vector(look);
	_upVector = Vector(up);
}

// TODO
Matrix Camera::GetProjectionMatrix() {
	double c = -_nearPlane / _farPlane;

	// 1) Scaling Matrix
	Matrix S;
	double radians = _viewAngle * PI / 180;
	S[0] = (double(1.0) / (_farPlane * tan(radians / 2)));
	S[5] = (double(1.0) / (_farPlane * tan(radians / 2)));
	S[10] = (double(1.0) / _farPlane);

	// 2) Perspective unhinging Matrix
	Matrix M;
	M[10] = (-1 / (c + 1));
	M[11] = (c / (c + 1));
	M[14] = -1;
	M[15] = 0;

	Matrix ProjectionMatrix = M * S;
	return ProjectionMatrix;
}


void Camera::SetViewAngle (double viewAngle) {
	_viewAngle = viewAngle;
}

void Camera::SetNearPlane (double nearPlane) {
	_nearPlane = nearPlane;
}

void Camera::SetFarPlane (double farPlane) {
	_farPlane = farPlane;
}

void Camera::SetScreenSize (int screenWidth, int screenHeight) {
	fprintf(stderr, "SetScreenSize: %d, %d\n", screenWidth, screenHeight);
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
}

// TODO
Matrix Camera::GetModelViewMatrix() {
	// 1) Transform Matrix
	Matrix T; // identity
	T[3] = -_eyePoint[0];
	T[7] = -_eyePoint[1];
	T[11] = -_eyePoint[2];

	// 2) Rotation Matrix
	Matrix R;

		// compute the w, u, v vectors of the camera
		Vector w = Vector(_lookVector);
		w.normalize();
		w.negate();

		Vector u = cross(_upVector, w);
		u.normalize();

		Vector v = cross(w, u); // already normalized

	// now fill rotation Matrix
	R[0] = u[0];
	R[1] = u[1];
	R[2] = u[2];

	R[4] = v[0];
	R[5] = v[1];
	R[6] = v[2];

	R[8] = w[0];
	R[9] = w[1];
	R[10] = w[2];

	Matrix V = R * T;
	return V;
}

// TODO
void Camera::RotateV(double angle) {
}

// TODO
void Camera::RotateU(double angle) {
}

// TODO
void Camera::RotateW(double angle) {
}

// TODO
void Camera::Translate(const Vector &v) {
}

// TODO
void Camera::Rotate(Point p, Vector axis, double degrees) {
}

Point Camera::GetEyePoint() {
	return _eyePoint;
}

Vector Camera::GetLookVector() {
	return _lookVector;
}

// TODO
Vector Camera::GetUpVector() {
	return _upVector;
}

double Camera::GetViewAngle() {
	return _viewAngle;
}

double Camera::GetNearPlane() {
	return _nearPlane;
}

double Camera::GetFarPlane() {
	return _farPlane;
}

int Camera::GetScreenWidth() {
	return _screenWidth;
}

int Camera::GetScreenHeight() {
	return _screenHeight;
}

// TODO
double Camera::GetFilmPlaneDepth() {
	return 0;
}

double Camera::GetScreenWidthRatio() {
	// TODO: ensure this is not the reciprical
	return (double)_screenWidth / (double)_screenHeight;
}
