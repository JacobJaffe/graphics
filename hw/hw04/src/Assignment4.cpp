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

typedef struct Intersection {
	double t;
	ScenePrimitive *primitive;
	Vector isectNormal__object;
	Vector isectNormal__world;
	Point point;
	Vector ray__object;
  Point p_eye__object;
} Intersection;

Intersection drawSceneNode(SceneNode *root, Point p_eye, Vector ray,  Matrix transformMatrix);
void setShape (ScenePrimitive *primitive);
SceneColor getColor(Intersection intersection, Vector ray);

SceneColor getPixel(int x, int y);

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

int SPECULAR_COMPONENT = 3;

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
Shape* shape = NULL;


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
			SceneColor p = getPixel(i, j);
			setPixel(pixels, i, j, p.r * 255, p.g * 255, p.b * 255);
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
	Vector guiLook(lookX, lookY, lookZ);
	guiLook.normalize();
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
	glui->add_spinner("Specular Exponent", GLUI_SPINNER_INT, &SPECULAR_COMPONENT);

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

Vector generateRay(int x, int y) {

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

SceneColor getPixel(int x, int y) {
	SceneNode* root = parser->getRootNode();

	Vector ray = generateRay(x, y);

	// // get eye Point
	Point p_eye = camera->GetEyePoint();

	ray.normalize();

	Matrix transformMatrix = Matrix();
	Intersection intersection = drawSceneNode(root, p_eye, ray, transformMatrix);

	intersection.point = p_eye + ray * intersection.t;

	SceneColor p = getColor(intersection, ray);
	return p;
}
SceneColor getColor(Intersection intersection, Vector ray)
{
	SceneColor p;
	p.r = 0;
	p.g = 0;
	p.b = 0;

	if (intersection.primitive == NULL) {
		return p;
	}

	Vector V = Vector(ray);
	V.normalize();

	SceneGlobalData sgd;
	if (!parser->getGlobalData(sgd)) {
		fprintf(stderr, "\nERROR: DID NOT GET SCENE GLOBAL DATA\n");
	}

	int num_lights = parser->getNumLights();
	SceneColor O_a = intersection.primitive->material.cAmbient;
	SceneColor O_d = intersection.primitive->material.cDiffuse;
	SceneColor O_s = intersection.primitive->material.cSpecular;


	// iterate over our fancy union of colors
	for (int j = 0; j < 3; j++) {

		// ambiant light of scene
		p.channels[j] = sgd.ka * O_a.channels[j];
		// fprintf(stderr, "Scene Global Data Ambient: %f\n", sgd.ka);
		// fprintf(stderr, "Object Ambient: %f\n", O_a);

		// fprintf(stderr, "Scene Global Data Specular: %f\n", sgd.ks);
		// fprintf(stderr, "Object Specular: %f\n", O_s.channels[j]);


		for (int i = 0; i < num_lights; i++) {
			SceneLightData sld;
			if (!parser->getLightData(i, sld)) {
				fprintf(stderr, "\nERROR: DID NOT GET LIGHT DATA FOR LIGHT: %d\n", i);
				continue;
			}
			Vector N = intersection.isectNormal__world;
			//N.normalize();
			Vector L = sld.pos - intersection.point;
			L.normalize();
			double difuse = sgd.kd * O_d.channels[j] * dot(N, L);

			Vector R = L - (2 * dot(V, N) * N);
			R.normalize();

			double foo = dot(V, R);
			foo = abs(foo);
			double specular = sgd.ks * O_s.channels[j] * pow(foo, SPECULAR_COMPONENT);
			double intensity = sld.color.channels[j];
			specular = specular < 0 ? 0 : specular;
			difuse = difuse < 0 ? 0 : difuse;

			p.channels[j] += intensity * (difuse + specular);
		}
	}

	for (int i = 0; i < 3; i++) {
		if (p.channels[i] > 1.0) {
			p.channels[i] = 1.0;
		}
		if (p.channels[i] < 0) {
		  p.channels[i] = 0.0;
		}
	}

	return p;
}

void setShape (ScenePrimitive *primitive) {
	int shapeType = primitive->type;
	switch (shapeType) {
	case SHAPE_CUBE:
		shape = cube;
		break;
	case SHAPE_CYLINDER:
		shape = cylinder;
		break;
	case SHAPE_CONE:
	// TODO: fix
		shape = cylinder;
		break;
	case SHAPE_SPHERE:
		shape = sphere;
		break;
	case SHAPE_SPECIAL1:
		shape = cube;
		break;
	default:
		shape = cube;
	}
}

Intersection drawSceneNode(SceneNode *root, Point p_eye, Vector ray, Matrix transformMatrix)
{
		Intersection intersection;
		intersection.primitive = NULL;
		intersection.t = DBL_MAX;

		double far = camera->GetFarPlane();

		int numTransformations = root->transformations.size();
		for (int i = 0; i <numTransformations; i++) {
			SceneTransformation *transformation = root->transformations[i];
			switch (transformation->type) {

				case TRANSFORMATION_ROTATE: {
					Vector v_rot = Vector(transformation->rotate[0], transformation->rotate[1], transformation->rotate[2]);
					double a = transformation->angle * 180 / PI;
					Matrix m_rot = rot_mat(v_rot, a);
					transformMatrix = transformMatrix * m_rot;
					break;
				}

				case TRANSFORMATION_SCALE: {
					Vector v_scale = Vector(transformation->scale[0], transformation->scale[1], transformation->scale[2]);
					Matrix m_scale = scale_mat(v_scale);
					transformMatrix = transformMatrix * m_scale;
					break;
				}

				case TRANSFORMATION_TRANSLATE: {
					Vector v_trans = Vector(transformation->translate[0], transformation->translate[1], transformation->translate[2]);
					Matrix m_trans = trans_mat(v_trans);
					transformMatrix = transformMatrix * m_trans;
					break;
				}
			}
		}

		int numPrimitives = root->primitives.size();

		for (int i = 0; i < numPrimitives; i++) {
		    setShape(root->primitives[i]);
				double t_shape = shape->Intersect(p_eye, ray, transformMatrix);

				if ((t_shape >= 1) && (t_shape <= far) && (t_shape < intersection.t)) {

					// compute object space ray and eye
					Matrix inverseTransformMatrix = invert(transformMatrix);
					Vector ray__object = inverseTransformMatrix * ray;
					Point p_eye__object = inverseTransformMatrix * p_eye;

					// find normal of surface at intersect point
					Vector n = shape->findIsectNormal(p_eye__object, ray__object, t_shape);
					Vector n_world = transformMatrix * n;

					intersection.t = t_shape;
					intersection.isectNormal__object = n;
					intersection.isectNormal__world = n_world;
					intersection.ray__object = ray__object;
					intersection.p_eye__object = p_eye__object;
					intersection.primitive = root->primitives[i];

				}
		}

		int numChildren = root->children.size();
		for (int i = 0; i < numChildren; i++) {
		    Intersection child_intersection = drawSceneNode(root->children[i], p_eye, ray, transformMatrix);
				if ((child_intersection.t >= 1) && (child_intersection.t <= far) && (child_intersection.t < intersection.t)) {
					intersection = child_intersection;
				}
		}

		return intersection;
}
