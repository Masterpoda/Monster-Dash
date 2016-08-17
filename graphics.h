/*
                    ***** graphics.h *****

OpenGL graphics program, illustrating use of GLUT and callback functions.

Author: John M. Weiss, Ph.D.
Class:  CSC300 Data Structures
Date:   Fall 2015

Modifications:
*/

// useful graphics function prototypes

// guard against multiple inclusions
#ifndef _Graphics_H_
#define _Graphics_H_

void initOpenGL( void );
void DrawLine( float x1, float y1, float x2, float y2, const float color[] );
void DrawRectangle( float x1, float y1, float x2, float y2, const float color[] );
void DrawFilledRectangle( float x1, float y1, float x2, float y2, const float color[] );
void DrawEllipse( float xRadius, float yRadius, int x, int y, const float color[] );
void DrawFilledEllipse( float xRadius, float yRadius, int x, int y, const float color[] );
void DrawTextString( char *string, int x, int y, const float color[] );

#endif
