#define NUM_OPENGL_LIGHTS 8

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glui.h>
#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "SceneParser.h"
#include "Camera.h"

using namespace std;


typedef struct Pixel {
	int r;
	int g;
	int b;
	double t;
} Pixel;

Pixel drawSceneNode(SceneNode *root, Point p_eye, Vector ray, double t);


Pixel getPixel(int x, int y);

/** These are the live variables passed into GLUI ***/
int  isectOnly = 1;

int	 camRotU = 0;
int	 camRotV = 0;
int	 camRotW = 0;
int  viewAngle = 45;
float eyeX = 2;
float eyeY = 2;
float eyeZ = 2;
float lookX = -2;
float lookY = -2;
float lookZ = -2;

/** These are GLUI control panel objects ***/
int  main_window;
string filenamePath = "data/general/test.xml";
GLUI_EditText* filenameTextField = NULL;
GLubyte* pixels = NULL;
int pixelWidth = 0, pixelHeight = 0;
int screenWidth = 0, screenHeight = 0;

/** these are the global variables used for rendering **/
Cube* cube = new Cube();
Cylinder* cylinder = new Cylinder();
Cone* cone = new Cone();
Sphere* sphere = new Sphere();
SceneParser* parser = NULL;
Camera* camera = new Camera();

void setupCamera();
void updateCamera();

void setPixel(GLubyte* buf, int x, int y, int r, int g, int b) {
	buf[(y*pixelWidth + x) * 3 + 0] = (GLubyte)r;
	buf[(y*pixelWidth + x) * 3 + 1] = (GLubyte)g;
	buf[(y*pixelWidth + x) * 3 + 2] = (GLubyte)b;
}

void callback_start(int id) {
	cout << "start button clicked!" << endl;

	if (parser == NULL) {
		cout << "no scene loaded yet" << endl;
		return;
	}

	pixelWidth = screenWidth;
	pixelHeight = screenHeight;

	updateCamera();

	if (pixels != NULL) {
		delete pixels;
	}
	pixels = new GLubyte[pixelWidth  * pixelHeight * 3];
	memset(pixels, 0, pixelWidth  * pixelHeight * 3);

	cout << "(w, h): " << pixelWidth << ", " << pixelHeight << endl;

	for (int i = 0; i < pixelWidth; i++) {
		for (int j = 0; j < pixelHeight; j++) {

			// TODO
			// //replace the following code
			// if ((i % 5 == 0) && (j % 5 == 0)) {
			// 	setPixel(pixels, i, j, 255, 0, 0);
			// }
			// else {
			// 	setPixel(pixels, i, j, 128, 128, 128);
			// }

			Pixel p = getPixel(i, j);
			setPixel(pixels, i, j, p.r, p.g, p.b);
		}
	}
	glutPostRedisplay();
}



void callback_load(int id) {
	char curDirName [2048];
	if (filenameTextField == NULL) {
		return;
	}
	printf ("%s\n", filenameTextField->get_text());

	if (parser != NULL) {
		delete parser;
	}
	parser = new SceneParser (filenamePath);

	bool success = parser->parse();
	cout << "success? " << success << endl;
	if (success == false) {
		delete parser;
		parser = NULL;
	}
	else {
		setupCamera();
	}
}


/***************************************** myGlutIdle() ***********/

void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	glutPostRedisplay();
}


/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
	float xy_aspect;

	xy_aspect = (float)x / (float)y;
	glViewport(0, 0, x, y);
	camera->SetScreenSize(x, y);

	screenWidth = x;
	screenHeight = y;

	glutPostRedisplay();
}


/***************************************** setupCamera() *****************/
void setupCamera()
{
	SceneCameraData cameraData;
	parser->getCameraData(cameraData);

	camera->Reset();
	camera->SetViewAngle(cameraData.heightAngle);
	if (cameraData.isDir == true) {
		camera->Orient(cameraData.pos, cameraData.look, cameraData.up);
	}
	else {
		camera->Orient(cameraData.pos, cameraData.lookAt, cameraData.up);
	}

	viewAngle = camera->GetViewAngle();
	Point eyeP = camera->GetEyePoint();
	Vector lookV = camera->GetLookVector();
	eyeX = eyeP[0];
	eyeY = eyeP[1];
	eyeZ = eyeP[2];
	lookX = lookV[0];
	lookY = lookV[1];
	lookZ = lookV[2];
	camRotU = 0;
	camRotV = 0;
	camRotW = 0;
	GLUI_Master.sync_live_all();
}

void updateCamera()
{
	std::cout << "UpdateCamera START" << std::endl;
	camera->Reset();

	Point guiEye (eyeX, eyeY, eyeZ);
	Point guiLook(lookX, lookY, lookZ);
	camera->SetViewAngle(viewAngle);

	// need an instance of the vector for a by reference call
	Vector up = camera->GetUpVector();
	camera->Orient(guiEye, guiLook, up);
	camera->RotateU(camRotU);
	camera->RotateV(camRotV);
	camera->RotateW(camRotW);
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (parser == NULL) {
		return;
	}

	if (pixels == NULL) {
		return;
	}

	// Point p = camera->GetEyePoint();
	// std::cout << "EyePoint: " << p[0] << " " << p[1] << " " << p[2] << std::endl;
	// std::cout << "eyeY: " << eyeY << std::endl;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(pixelWidth, pixelHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	glutSwapBuffers();
}

void onExit()
{
	delete cube;
	delete cylinder;
	delete cone;
	delete sphere;
	delete camera;
	if (parser != NULL) {
		delete parser;
	}
	if (pixels != NULL) {
		delete pixels;
	}
}

/**************************************** main() ********************/

int main(int argc, char* argv[])
{
	atexit(onExit);

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	main_window = glutCreateWindow("COMP 175 Assignment 4");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI* glui = GLUI_Master.create_glui("GLUI");

	filenameTextField = new GLUI_EditText( glui, "Filename:", filenamePath);
	filenameTextField->set_w(300);
	glui->add_button("Load", 0, callback_load);
	glui->add_button("Start!", 0, callback_start);
	glui->add_checkbox("Isect Only", &isectOnly);

	GLUI_Panel *camera_panel = glui->add_panel("Camera");
	(new GLUI_Spinner(camera_panel, "RotateV:", &camRotV))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateU:", &camRotU))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateW:", &camRotW))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "Angle:", &viewAngle))
		->set_int_limits(1, 179);

	glui->add_column_to_panel(camera_panel, true);

	GLUI_Spinner* eyex_widget = glui->add_spinner_to_panel(camera_panel, "EyeX:", GLUI_SPINNER_FLOAT, &eyeX);
	eyex_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyey_widget = glui->add_spinner_to_panel(camera_panel, "EyeY:", GLUI_SPINNER_FLOAT, &eyeY);
	eyey_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyez_widget = glui->add_spinner_to_panel(camera_panel, "EyeZ:", GLUI_SPINNER_FLOAT, &eyeZ);
	eyez_widget->set_float_limits(-10, 10);

	GLUI_Spinner* lookx_widget = glui->add_spinner_to_panel(camera_panel, "LookX:", GLUI_SPINNER_FLOAT, &lookX);
	lookx_widget->set_float_limits(-10, 10);
	GLUI_Spinner* looky_widget = glui->add_spinner_to_panel(camera_panel, "LookY:", GLUI_SPINNER_FLOAT, &lookY);
	looky_widget->set_float_limits(-10, 10);
	GLUI_Spinner* lookz_widget = glui->add_spinner_to_panel(camera_panel, "LookZ:", GLUI_SPINNER_FLOAT, &lookZ);
	lookz_widget->set_float_limits(-10, 10);

	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();

	return EXIT_SUCCESS;
}



Pixel foo(Point p_eye, Vector ray) {
	// black
	Pixel pixel;
	pixel.r = 0;
	pixel.g = 0;
	pixel.b = 0;
	pixel.t = -1;

	// t is when ray intersects x y plane

	// respect to the camera at postive Z, looking negatie Z
	double t_front = 0.5 -p_eye[2] / ray[2];
	double t_back = -0.5 -p_eye[2] / ray[2];

	double t_left = -0.5 -p_eye[0] / ray[0];
	double t_right = 0.5 -p_eye[0] / ray[0];

	double t_top = 0.5 -p_eye[1] / ray[1];
	double t_bot = -0.5 -p_eye[1] / ray[1];

	double near = camera->GetNearPlane();

	double t = -1;

	t = t_front;

	if ( ((t_back < t) || (t == -1)) && (t_back > near))
	{
		t = t_back;
	}
	if ( ((t_left < t) || (t == -1)) && (t_left > near))
	{
		t = t_left;
	}
	if ( ((t_right < t) || (t == -1)) && (t_right > near))
	{
		t = t_right;
	}
	if ( ((t_top < t) || (t == -1)) && (t_top > near))
	{
		t = t_top;
	}
	if ( ((t_bot < t) || (t == -1)) && (t_bot > near))
	{
		t = t_bot;
	}


	if (t < near) {
			return pixel;
	}


	Point p = p_eye + ray*t;


	if (t == t_front) {
		if ((p[0] >= - 0.5) && (p[0] <= 0.5)) {
			if ((p[1] >= - 0.5) && (p[1] <= 0.5)) {
				pixel.b = 255;
				pixel.g = 0;
				pixel.r = 0;
			}
		}
	}

	if (t == t_back) {
		if ((p[0] >= - 0.5) && (p[0] <= 0.5)) {
			if ((p[1] >= - 0.5) && (p[1] <= 0.5)) {
				pixel.b = 0;
				pixel.g = 0;
				pixel.r = 255;
			}
		}
	}

	if (t == t_right) {
		if ((p[2] >= - 0.5) && (p[2] <= 0.5)) {
			if ((p[1] >= - 0.5) && (p[1] <= 0.5)) {
				pixel.b = 0;
				pixel.g = 255;
				pixel.r = 0;
			}
		}
	}

	if (t == t_left) {
		if ((p[2] >= - 0.5) && (p[2] <= 0.5)) {
			if ((p[1] >= - 0.5) && (p[1] <= 0.5)) {
				pixel.b = 0;
				pixel.g = 255;
				pixel.r = 255;
			}
		}
	}

	if (t == t_top) {
		if ((p[2] >= - 0.5) && (p[2] <= 0.5)) {
			if ((p[0] >= - 0.5) && (p[0] <= 0.5)) {
				pixel.b = 255;
				pixel.g = 0;
				pixel.r = 255;
			}
		}
	}

	if (t == t_bot) {
		if ((p[2] >= - 0.5) && (p[2] <= 0.5)) {
			if ((p[0] >= - 0.5) && (p[0] <= 0.5)) {
				pixel.b = 255;
				pixel.g = 255;
				pixel.r = 0;
			}
		}
	}



	return pixel;
}

Vector generateRay(int x, int y) {

	// This is way 2:

	// float a = ((2.0 * x) / pixelWidth) - 1;
	// float b = 1 - ((2.0 * y) / pixelHeight);
	//
	// // This is the point on film plane, from x, y on screen
	// Point p_pixel_film = Point(a, b, -1);
	//
	//
	// // construct Matrix for undoing normalization
	// Vector v_scale = camera->getScaleVector();
	// Vector v_trans = camera->getTranslationVector();
	// Matrix m_rot = camera->GetProjectionMatrix();
	//
	// Matrix m_unScale = inv_scale_mat(v_scale);
	// Matrix m_unTrans = inv_trans_mat(v_trans);
	// Matrix m_unRotate = transpose(m_rot);
	//
	// Matrix m_filmToWorld = m_unTrans * m_unRotate * m_unScale;
	//
 	// // Get pixel in world space
	// Point p_pixel_world = m_filmToWorld * p_pixel_film;
	//
	// // get eye in world space
	// Point p_eye_world = camera->GetEyePoint();
	//
	// // get vector in world space
	// Vector v = p_pixel_world - p_eye_world;
	//
	// return v;

	// This is way 1, which is easy cause we have the LookVector reliably.
	Vector LookV = camera->GetLookVector();

	// Unit vectors of the film plane
	Vector v = camera->GetUpVector();
	v.normalize();

	Vector u = cross(LookV, v);
	u.normalize();

	// eye point
	Point P = camera->GetEyePoint();

	double near = camera->GetNearPlane();
	Point Q = P + near * LookV;

	// Width, height of film plane in the world coordinates
	double angle_degrees = camera->GetViewAngle();
	double angle_radians = angle_degrees * PI / 180;

	double H = near * tan(angle_radians / 2);
	double aspectRatio = camera->GetScreenWidthRatio();
	double W = H * aspectRatio;

	double a = (-W) + (2 * W) * x / pixelWidth;
	double b = (-H) + (2 * H) * y / pixelHeight;

	// 3D point on the film plane
	Point S = Q + (a * u) + (b * v);

	Vector ray = S - P;
	return ray;
}

Pixel getPixel(int x, int y) {
	SceneNode* root = parser->getRootNode();

	Vector ray = generateRay(x, y);

	// // get eye Point
	Point p_eye = camera->GetEyePoint();

	ray.normalize();

	// Pixel pixel = drawSceneNode(root, p_eye, ray, -1);

	Pixel pixel = foo(p_eye, ray);

	return pixel;
}

Pixel drawSceneNode(SceneNode *root, Point p_eye, Vector ray, double t)
{
    glPushMatrix();

        int numTransformations = root->transformations.size();
				for (int i = 0; i <numTransformations; i++) {
					SceneTransformation *transformation = root->transformations[i];
					switch (transformation->type) {

					case TRANSFORMATION_ROTATE:
						glRotatef(transformation->angle * 180 / PI, transformation->rotate[0], transformation->rotate[1], transformation->rotate[2]);
						break;

					case TRANSFORMATION_SCALE:
						glScalef(transformation->scale[0], transformation->scale[1], transformation->scale[2]);
						break;

					case TRANSFORMATION_TRANSLATE:
						glTranslatef(transformation->translate[0], transformation->translate[1], transformation->translate[2]);
						break;
					}
				}

        int numPrimitives = root->primitives.size();
        for (int i = 0; i < numPrimitives; i++) {

					// TODO: get T for each primative
            //foo(root->primitives[i]);
        }

        int numChildren = root->children.size();
        for (int i = 0; i < numChildren; i++) {
            drawSceneNode(root->children[i], p_eye, ray, t);
        }
    glPopMatrix();
}
