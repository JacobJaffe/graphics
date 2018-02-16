#include <GL/glui.h>
#include "solarSystem.h"

solarSystem::solarSystem(){

}

solarSystem::~solarSystem(){

}

// Render this with push and pop operations
// or alternatively implement helper functions
// for each of the planets with parameters.
void solarSystem::render(){

	// Some ideas for constants that can be used for
	// rotating the planets.
	static float rotation = 0.1;
	static float orbitX = 0.15;
	static float orbitSpeed = 0.2;
	static float moonOrbitX = 0.1;
	static float moonOrbitY = 0.1;
	static float moonOrbitSpeed = 1;

	// The Sun
		glPushMatrix();
			glRotatef(rotation,0,1,0);
			glColor3f(0.96f,0.85f,0.26f);
			glutSolidSphere(1.2,10,10);
		glPopMatrix();
	// Add more planets, moons, and rings here!

	// Planet 1
	glPushMatrix();
		glRotatef(rotation * 10,0,1,0);
		glTranslatef(3, 0.0f, 0.0f);
		glColor3f(0.0f, 1.0f,0.0f);
		glutSolidSphere(0.3,10,10);

		float moon_orbit_radius = 0.5;
		draw_circle(moon_orbit_radius);

		glPushMatrix();
			glRotatef(rotation * 20,0,1,0);
			glTranslatef(moon_orbit_radius, 0.0f, 0.0f);
			glColor3f(0.4f, 1.0f,0.5f);
			glutSolidSphere(0.1,10,10);
		glPopMatrix();
	glPopMatrix();


	// Planet 2 (On tilted axis)
	glPushMatrix();
	float planet_orbit2 = 10;
	glRotatef(20, 0.6, 0.2, 0.7);
	draw_circle(planet_orbit2);

	glRotatef(rotation * 10,0,1,0);
	glTranslatef(planet_orbit2, 0.0f, 0.0f);
	glColor3f(0.7f, 0.0f, 0.6f);

	// spin plannet on its axis
	glPushMatrix();
	glRotatef(rotation * 20, 0.6, 1, 0.2);
	glutSolidCube(2);
	glPopMatrix();

	float moon_orbit_radius2 = 0.5;
	draw_circle(moon_orbit_radius2);

	glPushMatrix();
		glRotatef(rotation * 20,0,1,0);
		glTranslatef(moon_orbit_radius2, 0.0f, 0.0f);
		glColor3f(0.4f, 1.0f,0.5f);
		glutSolidSphere(0.2,10,10);
	glPopMatrix();


	draw_circle(4.5);
	glPopMatrix();


	rotation+= 0.01;
	orbitX+=orbitSpeed;
	moonOrbitX+=moonOrbitSpeed;
	moonOrbitY+=moonOrbitSpeed;

}
