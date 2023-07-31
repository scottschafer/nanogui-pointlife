//
//  RenderUtils.hpp
//  pointlife
//
//  Created by Scott Schafer on 7/29/23.
//

#ifndef RenderUtils_hpp
#define RenderUtils_hpp

#include <GLUT/glut.h>
//#include <gl/GL.h>
#include "../globals.h"

inline void setLineWidth( float w) {
    glLineWidth(w * Globals::magnification);
  }

inline void setPointSize(float s) {
    glPointSize(s * Globals::magnification);
  }
  
inline void drawString(int x, int y, char *string)
  {
    int len, i;
    
    glRasterPos2f(x, y);
    len = (int) strlen(string);
    for (i = 0; i < len; i++) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
  }
  
  
inline void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)   {
      float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
      float x = r * cosf(theta);//calculate the x component
      float y = r * sinf(theta);//calculate the y component
      glVertex2f(x + cx, y + cy);//output vertex
    }
    glEnd();
  }

#endif /* RenderUtils_hpp */
