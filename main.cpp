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

static GLfloat ang, xRang = 2.0, yRang = 2.0, zRang = 2.0;
GLuint textures[7];

/**
 * @brief rubik's cube's cells
 * 
 */
int cube[55] = {
	1,5,5,1,0,5,0,1,1, // left
	3,4,4,3,4,4,2,3,3, // front
	2,2,4,2,2,4,4,4,3, // right

	1,1,0,1,5,0,0,0,5, // back
	4,2,2,2,3,3,2,3,3, // top
	5,0,0,5,1,0,1,5,5, // bottom
	6,  // extra value
};

int vertices[56][3] = {
	{-3, -3,  3},		// 0
	{-1, -3,  3},		// 1
	{ 1, -3,  3},		// 2
	{ 3, -3,  3},       // 3
	{ 3, -1,  3},       // 4
	{ 3,  1,  3},       // 5
	{ 3,  3,  3},       // 6
	{ 1,  3,  3},       // 7
	{-1,  3,  3},       // 8
	{-3,  3,  3},       // 9
	{-3,  1,  3},       // 10
	{-3, -1,  3},       // 11

	{-3, -3,  1},		// 12
	{-1, -3,  1},		// 13
	{ 1, -3,  1},		// 14
	{ 3, -3,  1},       // 15
	{ 3, -1,  1},       // 16
	{ 3,  1,  1},       // 17
	{ 3,  3,  1},       // 18
	{ 1,  3,  1},       // 19
	{-1,  3,  1},       // 20
	{-3,  3,  1},       // 21
	{-3,  1,  1},       // 22
	{-3, -1,  1},       // 23

	{-3, -3, -1},		// 24
	{-1, -3, -1},		// 25
	{ 1, -3, -1},		// 26
	{ 3, -3, -1},       // 27
	{ 3, -1, -1},       // 28
	{ 3,  1, -1},       // 29
	{ 3,  3, -1},       // 30
	{ 1,  3, -1},       // 31
	{-1,  3, -1},       // 32
	{-3,  3, -1},       // 33
	{-3,  1, -1},       // 34
	{-3, -1, -1},       // 35

	{-3, -3, -3},		// 36
	{-1, -3, -3},		// 37
	{ 1, -3, -3},		// 38
	{ 3, -3, -3},       // 39
	{ 3, -1, -3},       // 40
	{ 3,  1, -3},       // 41
	{ 3,  3, -3},       // 42
	{ 1,  3, -3},       // 43
	{-1,  3, -3},       // 44
	{-3,  3, -3},       // 45
	{-3,  1, -3},       // 46
	{-3, -1, -3},       // 47

	{-1, -1, 3},       // 48
	{-1,  1, 3},       // 49
	{ 1, -1, 3},       // 50
	{ 1,  1, 3},       // 51

	{ 1, -1, -3},       // 52
	{ 1,  1, -3},       // 53
	{-1, -1, -3},       // 54
	{-1,  1, -3}        // 55
};

double colors[7][3] = {
	{1.0, 0.5, 0.0},    //NARANJA   0
	{1.0, 1.0, 1.0},    //BLANCO    1
	{1.0, 0.0, 0.0},    //ROJO      2
	{1.0, 1.0, 0.0},    //AMARILLO  3
	{0.0, 0.0, 1.0},    //AZUL      4
	{0.0, 1.0, 0.0},    //VERDE     5
	{0.0, 0.0, 0.0}     //NEGRO     6
};

/**
 * @brief A function to load textures from bitmaps 
 * 
 * @param filename 
 * @return GLuint 
 */
GLuint loadTexture(const char* filename)
{
	GLuint texture;
	int width, height;
	unsigned char* data;
	FILE* file;

	file = fopen(filename, "rb");
	if (file == NULL)
		return 0;

	width = 256;
	height = 256;

	data = (unsigned char*)malloc(width * height * 3);
	fseek(file, 54, 0);

	fread(data, width * height * 3, 1, file);
	fclose(file);

	for (int i = 0; i < width * height; i++)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	free(data);
	return texture;
}

static void resize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-xRang, xRang, -yRang, yRang, -zRang, zRang);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void sticker(int n, int v1, int v2, int v3, int v4)
{
	double len = 0.35;
	int color = cube[n];
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[color]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3d(vertices[v1][0] * len, vertices[v1][1] * len, vertices[v1][2] * len);
	glTexCoord2f(0.0, 0.0);
	glVertex3d(vertices[v2][0] * len, vertices[v2][1] * len, vertices[v2][2] * len);
	glTexCoord2f(1.0, 0.0);
	glVertex3d(vertices[v3][0] * len, vertices[v3][1] * len, vertices[v3][2] * len);
	glTexCoord2f(1.0, 1.0);
	glVertex3d(vertices[v4][0] * len, vertices[v4][1] * len, vertices[v4][2] * len);
	/*glVertex3d(-1.0f, -1.0f, 1.0f);
	  glVertex3d(-1.0f, 1.0f, 1.0f);
	  glVertex3d(1.0f, 1.0f, 1.0f);
	  glVertex3d(1.0f, -1.0f, 1.0f);*/
	glEnd();
	glDisable(GL_TEXTURE);
}

static void corner1()
{
	sticker(9, 3, 4, 16, 15);
	sticker(42, 2, 50, 4, 3);
	sticker(2, 2, 3, 15, 14);
}

static void corner2()
{
	sticker(11, 5, 6, 18, 17);
	sticker(18, 6, 7, 19, 18);
	sticker(44, 51, 7, 6, 5);
}

static void corner3()
{
	sticker(20, 8, 9, 21, 20);
	sticker(27, 9, 10, 22, 21);
	sticker(36, 10, 9, 8, 49);
}

static void corner4()
{
	sticker(0, 0, 1, 13, 12);
	sticker(29, 11, 0, 12, 23);
	sticker(36, 0, 11, 48, 1);
}

static void corner5()
{
	sticker(8, 26, 27, 39, 38);
	sticker(15, 27, 28, 40, 39);
	sticker(45, 39, 40, 52, 38);
}

static void corner6()
{
	sticker(17, 29, 30, 42, 41);
	sticker(24, 30, 31, 43, 42);
	sticker(47, 41, 42, 43, 53);
}

static void corner7()
{
	sticker(26, 32, 33, 45, 44);
	sticker(33, 33, 34, 46, 45);
	sticker(53, 55, 44, 45, 46);
}

static void corner8()
{
	sticker(6, 24, 25, 37, 36);
	sticker(35, 35, 24, 36, 47);
	sticker(51, 37, 54, 47, 36);
}

//edgeS

static void edge1()
{
	sticker(1, 1, 2, 14, 13);
	sticker(39, 1, 48, 50, 2);
}

static void edge2()
{
	sticker(10, 4, 5, 17, 16);
	sticker(43, 50, 51, 5, 4);
}

static void edge3()
{
	sticker(19, 7, 8, 20, 19);
	sticker(41, 49, 8, 7, 51);
}

static void edge4()
{
	sticker(28, 10, 11, 23, 22);
	sticker(37, 11, 10, 49, 48);
}

//edgeS CAPAS DE EN MEDIO SEGUNDA CAPA
static void edge5()
{
	sticker(5, 14, 15, 27, 26);
	sticker(12, 15, 16, 28, 27);
}

static void edge6()
{
	sticker(14, 17, 18, 30, 29);
	sticker(21, 18, 19, 31, 30);
}

static void edge7()
{
	sticker(23, 20, 21, 33, 32);
	sticker(30, 21, 22, 34, 33);
}

static void edge8()
{
	sticker(3, 12, 13, 25, 24);
	sticker(32, 23, 12, 24, 35);
}

static void edge9()
{
	sticker(7, 25, 26, 38, 37);
	sticker(48, 38, 52, 54, 37);
}

static void edge10()
{
	sticker(16, 28, 29, 41, 40);
	sticker(46, 40, 41, 53, 52);
}

static void edge11()
{
	sticker(25, 31, 32, 44, 43);
	sticker(50, 53, 43, 44, 55);
}

static void edge12()
{
	sticker(34, 34, 35, 47, 46);
	sticker(52, 54, 55, 46, 47);
}

void drawcube()
{
	glRotatef(180.00, 0.0f, 0.0f, 1.0f);
	//CENTRO
	sticker(4, 13, 14, 26, 25);     //CENTRO IZQUIERDO
	sticker(13, 16, 17, 29, 28);    //CENTRO FRONTAL
	sticker(22, 19, 20, 32, 31);    //CENTRO DERECHO
	sticker(31, 22, 23, 35, 34);    //CENTRO POSTERIOR
	sticker(40, 48, 49, 51, 50);    //CENTRO SUPERIOR
	sticker(49, 52, 53, 55, 54);    //CENTRO INFERIOR

	//cornerS
	corner1();
	corner2();
	corner3();
	corner4();
	corner5();
	corner6();
	corner7();
	corner8();
	//edgeS
	edge1();
	edge2();
	edge3();
	edge4();
	edge5();
	edge6();
	edge7();
	edge8();
	edge9();
	edge10();
	edge11();
	edge12();
}

void turnRight()
{
	// center
	sticker(4, 13, 14, 26, 25);     //CENTRO IZQUIERDO
	sticker(13, 16, 17, 29, 28);    //CENTRO FRONTAL

	sticker(31, 22, 23, 35, 34);    //CENTRO POSTERIOR
	sticker(40, 48, 49, 51, 50);    //CENTRO SUPERIOR
	sticker(49, 52, 53, 55, 54);    //CENTRO INFERIOR

	// corners
	corner1();


	corner4();
	corner5();

	corner8();
	// edges
	edge1();
	edge2();

	edge4();
	edge5();

	edge8();
	edge9();
	edge10();

	edge12();

	sticker(54, 5, 10, 46, 41);

	glRotatef(45.00, 0.0f, 1.0f, 0.0f);

	corner2();
	corner3();
	corner6();
	corner7();

	edge3();
	edge6();
	edge7();
	edge11();

	sticker(22, 19, 20, 32, 31);    // right center


}

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	GLfloat rotate_x = -54.75;
	GLfloat rotate_y = 0;
	GLfloat rotate_z = -135;

	glRotatef(rotate_x, 1.0f, 0.0f, 0.0f);
	//glRotatef(rotate_y, 0.0f, 1.0f, 0.0f);
	glRotatef(rotate_z, 0.0f, 0.0f, 1.0f);

	drawcube();
	//turnRight();

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
	glutPostRedisplay();
}

static void idle(void)
{
	glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);										// Initialize glut
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);	// Visualization mode
	glutInitWindowSize(700, 700);								// Window initial size
	glutInitWindowPosition(0, 0);								// Window initial position
	glutCreateWindow("Rubik OpenGL");							// Create window

	textures[0] = loadTexture("/home/fwcheesecake/CppProjects/rubik/img/stckNaranja.bmp");
	textures[1] = loadTexture("/home/fwcheesecake/CppProjects/rubik/img/stckBlanco.bmp");
	textures[2] = loadTexture("/home/fwcheesecake/CppProjects/rubik/img/stckRojo.bmp");
	textures[3] = loadTexture("/home/fwcheesecake/CppProjects/rubik/img/stckAmarillo.bmp");
	textures[4] = loadTexture("/home/fwcheesecake/CppProjects/rubik/img/stckAzul.bmp");
	textures[5] = loadTexture("/home/fwcheesecake/CppProjects/rubik/img/stckVerde.bmp");

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glutReshapeFunc(resize);									// Set the glut rezise function reference
	glutDisplayFunc(display);									// Set the glut display function reference
	glutKeyboardFunc(key);										// Set the glut keyboard function
	glutIdleFunc(idle);											//  Set the glut idle function
	glutMainLoop();												// Todo funciona en un ciclo
	return EXIT_SUCCESS;
}
