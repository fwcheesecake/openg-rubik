#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

namespace Rubik
{
    extern GLfloat ang, xRang, yRang, zRang;
    extern GLuint textures[7];

    extern float theta;

    enum class Move
    {
        NONE,
        FRONT,
        RIGHT
    };
    extern Move moving;

    extern int direction;

    class Movements
    {
    public:
        // Draw a static cube
        static void drawCube();
        // turns the cube to the right
        static void turnFront();
        // turns the cube to the right
        static void turnRight();
    };

    class Definition
    {
    public:
        static GLuint loadTexture(const char *filename);
        static void initTextures();
        static void sticker(int n, int v1, int v2, int v3, int v4);

        static void corner1();
        static void corner2();
        static void corner3();
        static void corner4();
        static void corner5();
        static void corner6();
        static void corner7();
        static void corner8();

        static void edge1();
        static void edge2();
        static void edge3();
        static void edge4();
        static void edge5();
        static void edge6();
        static void edge7();
        static void edge8();
        static void edge9();
        static void edge10();
        static void edge11();
        static void edge12();
    };
}