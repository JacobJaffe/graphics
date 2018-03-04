#include <string.h>
#include <iostream>
#include <fstream>
#include <GL/glui.h>
#include <math.h>
#include "Mobius.h"

void Mobius::draw() {
  glBegin(GL_TRIANGLE_STRIP);

// heavily borrowed from https://www.opengl.org/discussion_boards/showthread.php/159496-Moebius-Strip,
// mostly working on fixing that code, cause it didn't exactly... work.

// TODO: look into doing this with matrix transformations?!
 for(float a = 0; a < 2.0 * PI; a += 2.0 / (m_segmentsX / 2))
 {
   for(float r = -1; r <= 1; r += 2)
   {
      Point p = Point();
      p[0] = cos(a)* ( 1+ (r/2 * cos(a/2)) );
      p[1] = sin(a)* ( 1+ (r/2 * cos(a/2)) );
      p[2] = r/2 *sin(a/2);

      glVertex3dv(p.unpack());
   }
 }

// connect back to start
/* gotta go in reverse here cause it has flipped */
float a = 0;
float r = 1;
  {
     Point p = Point();
     p[0] = cos(a)* ( 1+ (r/2 * cos(a/2)) );
     p[1] = sin(a)* ( 1+ (r/2 * cos(a/2)) );
     p[2] = r/2 *sin(a/2);

     glVertex3dv(p.unpack());
  }

r = -1;
   {
      Point p = Point();
      p[0] = cos(a)* ( 1+ (r/2 * cos(a/2)) );
      p[1] = sin(a)* ( 1+ (r/2 * cos(a/2)) );
      p[2] = r/2 *sin(a/2);

      glVertex3dv(p.unpack());
   }

 glEnd();
}


// TODO: this is awful code, but is a nice proof of concept
void Mobius::drawNormal() {
  Point p1;
  Point p2;
  Point p3;
   for(float a = 0; a < 2.1 * PI; a += 2.1 / (m_segmentsX / 2))
   {

     // glNormal3dv(normal.unpack());
     for(float r = -1; r <= 1; r += 2)
     {
       glBegin(GL_LINES);
       glColor3f(1.0, 0.0, 0.0);
        Point p = Point();
        p[0] = cos(a)* ( 1+ (r/2 * cos(a/2)) );
        p[1] = sin(a)* ( 1+ (r/2 * cos(a/2)) );
        p[2] = r/2 *sin(a/2);

        p3 = Point(p1);
        if (r == -1) {
          p1 = Point(p);
        } else {
          p2 = Point(p);
        }

        Vector v1 = p2 - p1;
        Vector v2 = p3 - p1;
        Vector normal = cross(v1, v2);
        normal.normalize();

        glVertex3dv(p.unpack());
        glVertex3dv((p + normal / 10).unpack());
        glEnd();
     }
   }

   for(float a = 0; a < 2.1 * PI; a += 2.1 / (m_segmentsX / 2))
   {

     // glNormal3dv(normal.unpack());
     for(float r = -1; r <= 1; r += 2)
     {
       glBegin(GL_LINES);
       glColor3f(1.0, 0.0, 0.0);
        Point p = Point();
        p[0] = cos(a)* ( 1+ (r/2 * cos(a/2)) );
        p[1] = sin(a)* ( 1+ (r/2 * cos(a/2)) );
        p[2] = r/2 *sin(a/2);

        p3 = Point(p1);
        if (r == 1) {
          p1 = Point(p);
        } else {
          p2 = Point(p);
        }

        Vector v1 = p2 - p1;
        Vector v2 = p3 - p1;
        Vector normal = cross(v1, v2);
        normal.normalize();
        normal = normal * -1;

        glVertex3dv(p.unpack());
        glVertex3dv((p + normal / 10).unpack());
        glEnd();
     }
   }
}
