/*  =================== File Information =================
	File Name: ppm.h
	Description:
	Author: Michael Shah
	Last Modified: 4/2/14

	Purpose:
	Usage:

	Further Reading resources:
	http://antongerdelan.net/opengl/raycasting.html
	===================================================== */
#include <string.h>
#include <iostream>
#include <fstream>
#include <GL/glui.h>
#include <math.h>
#include "object.h"
#include "camera.h"

/** These are the live variables passed into GLUI ***/
int main_window;

// int top_view_window;
// int left_view_window;
// int bottom_view_window;

int wireframe = 1;
int filled = 0;
int grid = 1;

bool castRay = false;
bool drag = false;
Point oldCenter;
Point oldIsectPoint;
float oldT;

//// Used for intersection
Point spherePosition(0, 0, 0);

// function declarations

// Window information
double windowXSize = 800;
double windowYSize = 800;


double mouseX = 0;
double mouseY = 0;

/* 	======== Our Scene ========
	We are going to create a single sphere that we load a texture onto.

	This object is assigned a unique id.
	=========================== */
object* myObject = new object(175);
Camera* camera = new Camera();

Vector generateRay(double pixelX, double pixelY) {
	double z = camera->GetNearPlane();
	Point p_eye = camera->GetEyePoint();

	double angle = PI * camera->GetViewAngle() / 180.0;
	double scale = z * tan(angle);
	Point p_nearPlane = Point(scale * pixelX, scale * pixelY, z);
	Vector ray = p_nearPlane - p_eye;
	std::cout << "ANGLE: " << angle << std::endl;
	std::cout << "Point p_eye: " << p_eye[0] << ", " << p_eye[1] << ", " << p_eye[2] << std::endl;
	std::cout << "Point p_nearPlane: " << p_nearPlane[0] << ", " << p_nearPlane[1] << ", " << p_nearPlane[2] << std::endl;

	std::cout << "Ray: " << ray[0] << ", " << ray[1] << ", " << ray[2] << std::endl;
	ray.normalize();
	return ray;
}

Point getEyePoint() {
	Point eye = camera->GetEyePoint();
	return eye;
}

Point getIsectPointWorldCoord(Point eye, Vector ray, double t) {
	Point p = eye + t * ray;
	return p;
}

/* =======================================================^ Mouse and Ray Casting Blocks of Code(above) ^=============================== */
double Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix) {

	double t = -1;
	double r = 0.5;

	double A = dot(rayV, rayV);

	Vector eye_p_v = Vector(eyePointP[0], eyePointP[1], eyePointP[2]);

	double B = 2 * dot(eye_p_v, rayV);
	double C = dot(eye_p_v, eye_p_v) - (r * r);

	double foo = (B * B) - (4 * A * C);
	double sqrt_of_b4ac = sqrt(foo);
	double t1 = (-B + sqrt_of_b4ac) / 2 * A;
	double t2  = (-B - sqrt_of_b4ac) / 2 * A;
	t = t1 > t2 ? t2 : t1;

	std::cout << "T: ";
	std::cout << t << ", "  <<  A << ", " << B << ", " << C<< ", " << foo  << ", "<< sqrt_of_b4ac;
	std::cout << std::endl;
	std::cout << eyePointP[0] << ", " << eyePointP[1] << ", " <<  eyePointP[2] << ", " << rayV[2];
	std::cout << std::endl;
	return t;
}


/* =======================================================v Mouse and Ray Casting Blocks of Code (below) v=============================== */

/*	===========================================
Updates the mouse cooridnates
This function turns a x and y screen cooridnate into a OpenGL world coordinate
in object space
=========================================== */
void updateMouse(int x, int y){
	mouseX = (2.0f * x)/windowXSize - 1.0f;
		mouseY = 1.0f - (2.0f * y)/ windowYSize;
}

/*	===========================================
This function is called everytime the mouse moves
In order to get our ray to draw nicely, we update the mouse coordinates
where the ray is first cast, and then draw the ray, and then draw the rest
of our scene.
=========================================== */
void myGlutMotion(int x, int y)
{
	// Get the x and y coordinates
	updateMouse(x, y);

	if (drag == true) {
		/*

		Insert code here


		*/
	}

}


/***************************************** myGlutMouse() ***********/
/*	This function is called everytime the mouse is clicked

	Updates the ray that we use for picking

*/
void myGlutMouse(int button, int button_state, int x, int y)
{
	updateMouse(x, y);

	// Cast a ray to the screen
	// Setup a flag so we can see the actual ray that is being cast.
	// note that left button casts a ray like normal
	// right button drags the object

	if (button == GLUT_LEFT_BUTTON) { //left mouse button
		if (button_state == GLUT_UP){
			castRay = false;
		}
		if ((button_state == GLUT_DOWN) && (castRay == false)){
			castRay = true;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) { //right mouse button

		if (button_state == GLUT_UP){
			drag = false;
		}
		else if ((button_state == GLUT_DOWN) && (drag == false)){
			/*

			Insert code here


			*/
		}
	}

	// redraw the scene
	glutPostRedisplay();
}







/*	===================================
	Draw a 10x10 grid under our scene
	=================================== */
void draw_grid(){
	int grid_size = 10;
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(-grid_size/2,0,-grid_size/2);
    // Draw a grid under the object
        for(int i = 0 ; i < grid_size; i=i+1){
            glBegin(GL_LINES);
                glColor3f( 1.0, 1.0, 1.0 );
                glVertex3f( 0.0, 0.0, i );  glVertex3f( grid_size, 0.0, i ); /* X axis grid */
                glVertex3f( i, 0.0, 0.0 );  glVertex3f( i, 0.0, grid_size ); /* X axis grid */
            glEnd();
        }
    	// Draw the final two lines in our scene on the opposite corners
        glBegin(GL_LINES);
            glColor3f( 1.0, 1.0, 1.0 );
            glVertex3f( 0.0, 0.0, grid_size );  glVertex3f( grid_size, 0.0, grid_size ); /* X axis grid */
            glVertex3f( grid_size, 0.0, 0.0 );  glVertex3f( grid_size, 0.0, grid_size ); /* X axis grid */
        glEnd();
    glPopMatrix();
    glEnable( GL_LIGHTING );
}


/***************************************** myGlutDisplay() *****************/
/* 	============================
	Renders the scene
	============================ */
void myGlutDisplay(void)
{
	camera->Reset();
	// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	Matrix projection = camera->GetProjectionMatrix();
	glLoadMatrixd(projection.unpack());

	Point p1 = Point(0, 0, 4);
	Vector v1 = Vector(0, 0, -1);
	Vector v2 = Vector(0, 1, 0);
	camera->Orient(p1, v1, v2);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(camera->GetModelViewMatrix().unpack());

	if (castRay == true){
		Point eyePointP = getEyePoint();
		Vector rayV = generateRay(mouseX, mouseY);

		Vector sphereTransV(spherePosition[0], spherePosition[1], spherePosition[2]);

		/****** Note the Matrix passed into Intersect is no longer just an identity matrix!! **********/
		double t = Intersect(eyePointP, rayV, trans_mat(sphereTransV));


		Point isectPointWorldCoord = getIsectPointWorldCoord(eyePointP, rayV, t);

		if (t > 0) {
			glColor3f(1, 0, 0);
			glPushMatrix();
				glTranslated(spherePosition[0], spherePosition[1], spherePosition[2]);
				glutWireCube(1.0f);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(isectPointWorldCoord[0], isectPointWorldCoord[1], isectPointWorldCoord[2]);
				glutSolidSphere(0.05f, 10, 10);
			glPopMatrix();
		}
	}



	glPushMatrix();

	//move the sphere to the designated position
	glTranslated(spherePosition[0], spherePosition[1], spherePosition[2]);

	// Draw the objects filled
		if (filled) {
			glEnable(GL_POLYGON_OFFSET_FILL);
			glColor3f(1.0, 1.0, 1.0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glPushMatrix();
				glRotatef(90,0,1,0);
				myObject->drawTexturedSphere();
			glPopMatrix();
		}


		// Draw a wireframe object
		if (wireframe) {
			glDisable(GL_POLYGON_OFFSET_FILL);
			glColor3f(1.0, 1.0, 1.0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glPushMatrix();
				glRotatef(90,0,1,0);
				myObject->drawTexturedSphere();
			glPopMatrix();
		}
	glPopMatrix();

	// Draw the grid
	// grid is a live variable tied to the GLUI_Checkbox object
	if(grid){
		draw_grid();
	}

	glutSwapBuffers();
}





/*	==========================================
	GlutKeyboard that adds in user interaction
	========================================== */
void myGlutKeyboard(unsigned char Key, int x, int y)
{
    switch(Key)
    {
        case 27:	 // Case '27' maps to the 'ESC' key, this case will fall through into the next body of code.
        case 'q':    exit(0); break;
        default:	std::cout << "No callback for key" << std::endl;
    };

    glutPostRedisplay();
}




/* =======================
   myGlutIdle()
   ======================= */
void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window){
		glutSetWindow(main_window);
	}

	glutPostRedisplay();
}


/* 	======================
	myGlutReshape()

	When the window is resized, update aspect ratio to get correct viewing frustrum.
	====================== */
void myGlutReshape(int x, int y)
{
	float xy_aspect;

	xy_aspect = (float)x / (float)y;
	// Pixel coordinate system
	glViewport(0, 0, x, y);

	windowXSize = x;
	windowYSize = y;

	camera->SetScreenSize(x, y);

	glutPostRedisplay();
}


/* ==============================
	Reclaim memory we've allocated
   ============================== */
void onExit()
{
	delete myObject;
}

/**************************************** main() ********************/
void glutInitValues(){
	/****************************************/
	/*       Set up OpenGL lighting         */
	/****************************************/
	glShadeModel(GL_SMOOTH);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	static float one[] = { 1, 1, 1, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);
	/****************************************/
	/*          Enable z-buferring          */
	/****************************************/
	glEnable(GL_DEPTH_TEST);
	glPolygonOffset(1, 1);

	/****************************************/
	/*		Setup textured Objects          */
	/****************************************/

	// Apply all of these functions to our windows.
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);
	glutMotionFunc( myGlutMotion );
}


int main(int argc, char* argv[])
{
	atexit(onExit);

	int border = 5;
	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowXSize, windowYSize);
	// CREATE THE MAIN WINDOW
	main_window = glutCreateWindow("COMP 175 In Class Lab 8");
	glutInitValues();


	// Set the base texture of our object
	myObject->setTexture(0,".//data//pink.ppm");
	// Set a second texture layer to our object
	myObject->setTexture(1,".//data//smile.ppm");

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/
	GLUI *glui = GLUI_Master.create_glui("GLUI");
	// Animation panel

	// Render Panel
	GLUI_Panel *render_panel = glui->add_panel("Render");
	new GLUI_Checkbox(render_panel, "Wireframe", &wireframe);
	new GLUI_Checkbox(render_panel, "Filled", &filled);
	new GLUI_Checkbox(render_panel, "Grid", &grid);

	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);
	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);
	GLUI_Master.set_glutMouseFunc( myGlutMouse );
	GLUI_Master.set_glutKeyboardFunc(myGlutKeyboard);

	// Start the GLUT processin loop
	glutMainLoop();

	return EXIT_SUCCESS;
}
