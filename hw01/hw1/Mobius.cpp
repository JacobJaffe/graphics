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
 for(float a = 0; a < 2.1 * PI; a += 0.05)
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

 glEnd();
}
