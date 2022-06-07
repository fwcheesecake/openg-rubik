/**
 * @file main.cpp
 * @author Antonio Reyna Espinoza (antoniore102001@gmail.com)
 * @brief Open GL rubik's cube implementation
 * @version 0.1
 * @date 2022-06-07
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "class/rubik.h"

using namespace Rubik;

static void resize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-xRang, xRang, -yRang, yRang, -zRang, zRang);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	GLfloat rotate_x = -54.75;
	GLfloat rotate_y = 0;
	GLfloat rotate_z = -135;

	glRotatef(rotate_x, 1.0f, 0.0f, 0.0f);
	// glRotatef(rotate_y, 0.0f, 1.0f, 0.0f);
	glRotatef(rotate_z, 0.0f, 0.0f, 1.0f);

	// drawcube();
	Movements::turnRight();
	//Movements::turnFront();

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		moving = Move::RIGHT;
		direction = 1;
		break;
	case 'R':
		moving = Move::RIGHT;
		direction = -1;
		break;
	case 'f':
		moving = Move::FRONT;
		direction = 1;
		break;
	case 'F':
		moving = Move::FRONT;
		direction = -1;
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

static void idle(void)
{
	if (moving != Move::NONE)
	{
		theta += 0.5;
		float value = abs(theta / 90.0);
		if (value == 4)
		{
			theta = 0;
			moving = Move::NONE;
		}
		else if (value == 1 || value == 2 || value == 3)
			;
		{
			moving = Move::NONE;
		}
	}
	glutPostRedisplay();
}

/* Program entry point */
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);									  // Initialize glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Visualization mode
	glutInitWindowSize(700, 700);							  // Window initial size
	glutInitWindowPosition(0, 0);							  // Window initial position
	glutCreateWindow("Rubik OpenGL");						  // Create window

	Definition::initTextures();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glutReshapeFunc(resize);  // Set the glut rezise function reference
	glutDisplayFunc(display); // Set the glut display function reference
	glutKeyboardFunc(key);	  // Set the glut keyboard function
	glutIdleFunc(idle);		  //  Set the glut idle function
	glutMainLoop();			  // Todo funciona en un ciclo
	return EXIT_SUCCESS;
}
