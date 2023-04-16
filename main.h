#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <malloc.h>
#include "menu.h"
#include "stb-master/stb_easy_font.h"
#include "textures.h"

using namespace std;

GLuint texture;
GLuint sprite;
GLuint background;

int width = 1037;
int height = 671;
float gravity = 0.4;
float speed = 6.0;
float currentframe=2;
float chSiX;
float chSiY;
bool directional;
bool gameState=0;
int n=0;


typedef struct
{
    float left, right, top, buttom, textX, textY;
} sritCoord;

typedef struct
{
    float x, y, dx, dy;
} Robot;

Robot character;
sritCoord vrtcoord;
sritCoord BackGroundCoord;

void Robot_Init(Robot *obj, float x1, float y1, float dx1, float dy1)
{
    obj->x=x1;
    obj->y=y1;
    obj->dx=dx1;
    obj->dy=dy1;
}

void Reflect (float *da, float *a, BOOL cond, float wall)
{
    if (!cond) return;
    *da*=-0;
    *a=wall;
}

void Robot_Move(Robot *obj)
{
    obj->y+=obj->dy;
    obj->x+=obj->dx;
    Reflect(&obj->dy, &obj->y, obj->y<0,0);
    Reflect(&obj->dx,&obj->x, obj->x<0, 0);
    Reflect(&obj->dx,&obj->x, obj->x>width-chSiX-0, width-chSiX-0);
    obj->dy-=gravity;
    if (GetKeyState(VK_LEFT)<0 && gameState==1)
    {
        currentframe+=0.2;
        obj->dx-=speed;
        obj->x+=obj->dx;
        obj->dx=0;
        if (currentframe>8) currentframe-=7;
        directional=1;

    }

    if (GetKeyState(VK_RIGHT)<0 && gameState==1)
    {
        currentframe+=0.2;
        obj->dx+=speed;
        obj->x+=obj->dx;
        obj->dx=0;
        if (currentframe>8) currentframe-=7;
        directional=0;


    }
    if (GetKeyState(VK_UP)<0 && (obj->y<410) && gameState==1)
    {
        obj->dy =speed*1.2;
        obj->y+=obj->dy;
    }
}



void Sprite_animation(GLuint texture, int n, Robot *obj)
{

    static float svertix[]= {0,0, 80,0, 80,80, 0,80};
    static float TexCord[]= {0,0, 0.12,0, 0.12,1, 0,1};
    static float line[]= {0,0, 90,0, 90,90, 0,90};

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.7);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2,GL_FLOAT,0,svertix);
    glTexCoordPointer(2,GL_FLOAT,0,TexCord);

    static float spriteXsize=779.438;
    static float spriteYsize=68.761;
    static float charsizey=20.5;
    static float charsizex=97;
    vrtcoord.left=(charsizex*n)/spriteXsize;
    vrtcoord.right=vrtcoord.left+(charsizex/spriteXsize);
    vrtcoord.top=(charsizey*0)/spriteYsize;
    vrtcoord.buttom=vrtcoord.top+(charsizey/spriteYsize);

    TexCord[1] = TexCord[3] = vrtcoord.buttom;
    TexCord[5] = TexCord[7] = vrtcoord.top;

    if (GetKeyState(VK_LEFT)<0 && gameState==1)
    {
        TexCord[2] = TexCord[4] = vrtcoord.left;
        TexCord[0] = TexCord[6] = vrtcoord.right;
    }
    if (GetKeyState(VK_RIGHT)<0 && gameState==1)
    {
        TexCord[0] = TexCord[6] = vrtcoord.left;
        TexCord[2] = TexCord[4] = vrtcoord.right;
    }




    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisable(GL_ALPHA_TEST);

    glLineWidth(3);
    glVertexPointer(2,GL_FLOAT,0,line);
    glDrawArrays(GL_LINE_LOOP, 0,4);



    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();

    chSiX=charsizex;
    chSiY=charsizey;

}



void Main_Init()
{
    Robot_Init(&character, width/2.0,height/2.0,0,0);
    Load_Texture("robot.png", &sprite, GL_CLAMP, GL_CLAMP, GL_NEAREST);
    Load_Texture("background.png", &background, GL_REPEAT, GL_CLAMP, GL_NEAREST);
}

void Menu_Init(){
    Menu_AddButton("START", 448, 10, 150, 70, 2);
    Menu_AddButton("EXIT", 448, 100, 150, 70, 2);
    //Menu_AddButton("II", 10, 10, 30, 30, 2);
}
/*void Menu_Init1(){
    Menu_AddButton("II", 10, 10, 30, 30, 2);
}*/




void Show_Background(GLuint texture)
{
    static float svertix[]= {-1037,0, -1037,671, 1037*2,671, 1037*2,0};
    static float TexCord[]= {-2,1, -2,0, 2,0, 2,1 };

    glClearColor(0,0,0,0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2,GL_FLOAT,0,svertix);
    glTexCoordPointer(2,GL_FLOAT,0,TexCord);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


void MouseDown()
{
    int buttonId = Menu_MouseDown();
    if(buttonId < 0) return;
    char *name = Menu_GetButtonName(buttonId);
    switch (buttonId)
    {
    case 0:
        if (gameState == 0)
        {
            gameState = 1;
            //Menu_Clear();
        }
        break;
    case 1:
        PostQuitMessage(0);
        break;
    }
}


void Robot_Show(Robot *obj)
{
    glPushMatrix();
    glTranslatef(obj->x,obj->y,0);
    Sprite_animation(sprite, currentframe, &character);
    glPopMatrix();

}

#endif // MAIN_H_INCLUDED
