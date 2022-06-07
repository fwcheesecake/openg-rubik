#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "rubik.h"

namespace Rubik
{
    GLfloat ang = 0.0;

    GLfloat xRang = 2.0;
    GLfloat yRang = 2.0;
    GLfloat zRang = 2.0;

    GLuint textures[7];

    float theta = 0.0;

    Move moving = Move::NONE;

    int direction = 1;

    /**
     * @brief rubik's cube's cells
     *
     */
    int cube[55] = {
        1, 5, 5, 1, 0, 5, 0, 1, 1, // left
        3, 4, 4, 3, 4, 4, 2, 3, 3, // front
        2, 2, 4, 2, 2, 4, 4, 4, 3, // right

        1, 1, 0, 1, 5, 0, 0, 0, 5, // back
        4, 2, 2, 2, 3, 3, 2, 3, 3, // top
        5, 0, 0, 5, 1, 0, 1, 5, 5, // bottom
        6,                         // extra value
    };

    int vertices[56][3] = {
        {-3, -3, 3}, // 0
        {-1, -3, 3}, // 1
        {1, -3, 3},  // 2
        {3, -3, 3},  // 3
        {3, -1, 3},  // 4
        {3, 1, 3},   // 5
        {3, 3, 3},   // 6
        {1, 3, 3},   // 7
        {-1, 3, 3},  // 8
        {-3, 3, 3},  // 9
        {-3, 1, 3},  // 10
        {-3, -1, 3}, // 11

        {-3, -3, 1}, // 12
        {-1, -3, 1}, // 13
        {1, -3, 1},  // 14
        {3, -3, 1},  // 15
        {3, -1, 1},  // 16
        {3, 1, 1},   // 17
        {3, 3, 1},   // 18
        {1, 3, 1},   // 19
        {-1, 3, 1},  // 20
        {-3, 3, 1},  // 21
        {-3, 1, 1},  // 22
        {-3, -1, 1}, // 23

        {-3, -3, -1}, // 24
        {-1, -3, -1}, // 25
        {1, -3, -1},  // 26
        {3, -3, -1},  // 27
        {3, -1, -1},  // 28
        {3, 1, -1},   // 29
        {3, 3, -1},   // 30
        {1, 3, -1},   // 31
        {-1, 3, -1},  // 32
        {-3, 3, -1},  // 33
        {-3, 1, -1},  // 34
        {-3, -1, -1}, // 35

        {-3, -3, -3}, // 36
        {-1, -3, -3}, // 37
        {1, -3, -3},  // 38
        {3, -3, -3},  // 39
        {3, -1, -3},  // 40
        {3, 1, -3},   // 41
        {3, 3, -3},   // 42
        {1, 3, -3},   // 43
        {-1, 3, -3},  // 44
        {-3, 3, -3},  // 45
        {-3, 1, -3},  // 46
        {-3, -1, -3}, // 47

        {-1, -1, 3}, // 48
        {-1, 1, 3},  // 49
        {1, -1, 3},  // 50
        {1, 1, 3},   // 51

        {1, -1, -3},  // 52
        {1, 1, -3},   // 53
        {-1, -1, -3}, // 54
        {-1, 1, -3}   // 55
    };

    double colors[7][3] = {
        {1.0, 0.5, 0.0}, // NARANJA   0
        {1.0, 1.0, 1.0}, // BLANCO    1
        {1.0, 0.0, 0.0}, // ROJO      2
        {1.0, 1.0, 0.0}, // AMARILLO  3
        {0.0, 0.0, 1.0}, // AZUL      4
        {0.0, 1.0, 0.0}, // VERDE     5
        {0.0, 0.0, 0.0}  // NEGRO     6
    };

    GLuint Definition::loadTexture(const char *filename)
    {
        GLuint texture;
        int width, height;
        unsigned char *data;
        FILE *file;

        file = fopen(filename, "rb");
        if (file == NULL)
            return 0;

        width = 256;
        height = 256;

        data = (unsigned char *)malloc(width * height * 3);
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

    void Definition::initTextures()
    {
        textures[0] = loadTexture("/home/fwcheesecake/CppProjects/openg-rubik/img/stckNaranja.bmp");
        textures[1] = loadTexture("/home/fwcheesecake/CppProjects/openg-rubik/img/stckBlanco.bmp");
        textures[2] = loadTexture("/home/fwcheesecake/CppProjects/openg-rubik/img/stckRojo.bmp");
        textures[3] = loadTexture("/home/fwcheesecake/CppProjects/openg-rubik/img/stckAmarillo.bmp");
        textures[4] = loadTexture("/home/fwcheesecake/CppProjects/openg-rubik/img/stckAzul.bmp");
        textures[5] = loadTexture("/home/fwcheesecake/CppProjects/openg-rubik/img/stckVerde.bmp");
    }

    void Definition::sticker(int n, int v1, int v2, int v3, int v4)
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

    void Definition::corner1()
    {
        Definition::sticker(9, 3, 4, 16, 15);
        Definition::sticker(42, 2, 50, 4, 3);
        Definition::sticker(2, 2, 3, 15, 14);
    }

    void Definition::corner2()
    {
        Definition::sticker(11, 5, 6, 18, 17);
        Definition::sticker(18, 6, 7, 19, 18);
        Definition::sticker(44, 51, 7, 6, 5);
    }

    void Definition::corner3()
    {
        Definition::sticker(20, 8, 9, 21, 20);
        Definition::sticker(27, 9, 10, 22, 21);
        Definition::sticker(36, 10, 9, 8, 49);
    }

    void Definition::corner4()
    {
        Definition::sticker(0, 0, 1, 13, 12);
        Definition::sticker(29, 11, 0, 12, 23);
        Definition::sticker(36, 0, 11, 48, 1);
    }

    void Definition::corner5()
    {
        Definition::sticker(8, 26, 27, 39, 38);
        Definition::sticker(15, 27, 28, 40, 39);
        Definition::sticker(45, 39, 40, 52, 38);
    }

    void Definition::corner6()
    {
        Definition::sticker(17, 29, 30, 42, 41);
        Definition::sticker(24, 30, 31, 43, 42);
        Definition::sticker(47, 41, 42, 43, 53);
    }

    void Definition::corner7()
    {
        Definition::sticker(26, 32, 33, 45, 44);
        Definition::sticker(33, 33, 34, 46, 45);
        Definition::sticker(53, 55, 44, 45, 46);
    }

    void Definition::corner8()
    {
        Definition::sticker(6, 24, 25, 37, 36);
        Definition::sticker(35, 35, 24, 36, 47);
        Definition::sticker(51, 37, 54, 47, 36);
    }

    void Definition::edge1()
    {
        Definition::sticker(1, 1, 2, 14, 13);
        Definition::sticker(39, 1, 48, 50, 2);
    }

    void Definition::edge2()
    {
        Definition::sticker(10, 4, 5, 17, 16);
        Definition::sticker(43, 50, 51, 5, 4);
    }

    void Definition::edge3()
    {
        Definition::sticker(19, 7, 8, 20, 19);
        Definition::sticker(41, 49, 8, 7, 51);
    }

    void Definition::edge4()
    {
        Definition::sticker(28, 10, 11, 23, 22);
        Definition::sticker(37, 11, 10, 49, 48);
    }

    void Definition::edge5()
    {
        Definition::sticker(5, 14, 15, 27, 26);
        Definition::sticker(12, 15, 16, 28, 27);
    }

    void Definition::edge6()
    {
        Definition::sticker(14, 17, 18, 30, 29);
        Definition::sticker(21, 18, 19, 31, 30);
    }

    void Definition::edge7()
    {
        Definition::sticker(23, 20, 21, 33, 32);
        Definition::sticker(30, 21, 22, 34, 33);
    }

    void Definition::edge8()
    {
        Definition::sticker(3, 12, 13, 25, 24);
        Definition::sticker(32, 23, 12, 24, 35);
    }

    void Definition::edge9()
    {
        Definition::sticker(7, 25, 26, 38, 37);
        Definition::sticker(48, 38, 52, 54, 37);
    }

    void Definition::edge10()
    {
        Definition::sticker(16, 28, 29, 41, 40);
        Definition::sticker(46, 40, 41, 53, 52);
    }

    void Definition::edge11()
    {
        Definition::sticker(25, 31, 32, 44, 43);
        Definition::sticker(50, 53, 43, 44, 55);
    }

    void Definition::edge12()
    {
        Definition::sticker(34, 34, 35, 47, 46);
        Definition::sticker(52, 54, 55, 46, 47);
    }

    void Movements::drawCube()
    {
        // CENTRO
        Definition::sticker(4, 13, 14, 26, 25);  // CENTRO IZQUIERDO
        Definition::sticker(13, 16, 17, 29, 28); // CENTRO FRONTAL
        Definition::sticker(22, 19, 20, 32, 31); // CENTRO DERECHO
        Definition::sticker(31, 22, 23, 35, 34); // CENTRO POSTERIOR
        Definition::sticker(40, 48, 49, 51, 50); // CENTRO SUPERIOR
        Definition::sticker(49, 52, 53, 55, 54); // CENTRO INFERIOR

        // cornerS
        Definition::corner1();
        Definition::corner2();
        Definition::corner3();
        Definition::corner4();
        Definition::corner5();
        Definition::corner6();
        Definition::corner7();
        Definition::corner8();
        // edgeS
        Definition::edge1();
        Definition::edge2();
        Definition::edge3();
        Definition::edge4();
        Definition::edge5();
        Definition::edge6();
        Definition::edge7();
        Definition::edge8();
        Definition::edge9();
        Definition::edge10();
        Definition::edge11();
        Definition::edge12();
    }
    void Movements::turnRight()
    {
        // center
        Definition::sticker(4, 13, 14, 26, 25);  // CENTRO IZQUIERDO
        Definition::sticker(13, 16, 17, 29, 28); // CENTRO FRONTAL

        Definition::sticker(31, 22, 23, 35, 34); // CENTRO POSTERIOR
        Definition::sticker(40, 48, 49, 51, 50); // CENTRO SUPERIOR
        Definition::sticker(49, 52, 53, 55, 54); // CENTRO INFERIOR

        // corners
        Definition::corner1();
        Definition::corner4();
        Definition::corner5();
        Definition::corner8();

        // edges
        Definition::edge1();
        Definition::edge2();
        Definition::edge4();
        Definition::edge5();
        Definition::edge8();
        Definition::edge9();
        Definition::edge10();
        Definition::edge12();

        Definition::sticker(54, 5, 10, 46, 41);

        glRotatef(45.00, 0.0f, 1.0f, 0.0f);

        Definition::corner2();
        Definition::corner3();
        Definition::corner6();
        Definition::corner7();

        Definition::edge3();
        Definition::edge6();
        Definition::edge7();
        Definition::edge11();

        Definition::sticker(22, 19, 20, 32, 31); // right center
    }

}