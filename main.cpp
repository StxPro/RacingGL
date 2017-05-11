//
//  main.cpp
//  RacingGL
//
//  Created by Napoleon Cortes and Javier Dalma on 3/29/17.
//  Copyright © 2017 Javier Dalma. All rights reserved.
//

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include "glut.h"  
#include <iostream>
#include <fstream>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <cmath>

using namespace std;


struct Vertex
{
    float x, y, z, p;
};

float largo = 500;
float meta = 100;
const int cosas = 40;

Vertex lento[cosas];
Vertex danio[cosas];
Vertex vida[cosas];
Vertex rapido[cosas];
Vertex invertir[cosas];
float v = 0.03;
float g = 0.01;

float g1 = g;
float g2 = g;

float g11 = g;
float g22 = g;

static int width;
static float height;

float zP1= largo + meta - 2.0;
float zP2= largo + meta - 2.0;

float xP1=3.0f;
float xP2=-3.0f;


float increasPosition=0.1f;


bool isCollidingXRight = false;
bool isCollidingXLeft = false;
bool isCollidingZFront = false;
bool isCollidingZBack = false;

bool collission = false;

bool col1Left = false;
bool col1Right = false;
bool col1Front = false;

bool col2Left = false;
bool col2Right = false;
bool col2Front = false;



float redCubeCamPosZ= largo + meta + 1.0;
float redCubeCamPosX=1.0;

float blueCubeCamPosZ= largo + meta + 1.0;
float blueCubeCamPosX=-1.0;



float v1 = v;
float v2 = v;

float v11 = v;
float v22 = v;

float vida1 = 7.0;
float vida2 = 7.0;

bool izq1 = false;
bool izq2 = false;
bool der1 = false;
bool der2 = false;

GLfloat gold_Diffuse[] = { 0.75164, 0.60648, 0.22648, 1.0 };
GLfloat gold_Specular[] = { 0.628281, 0.555802, 0.366065, 1.0 };
GLfloat gold_Shinny[] = { 51.2 };
GLfloat gold_Ambient[] = { 0.24725, 0.1995, 0.0745, 1.0 };

struct arr
{
	double array[3];
};

struct arr1
{
	double array[1];
};

void pintar(arr spec, arr1 shiny, arr amb, arr dif) {
	//glPushMatrix();
	GLfloat mat_spec[] = { spec.array[0], spec.array[1], spec.array[2] };
	GLfloat mat_shiny[] = { shiny.array[0] };
	GLfloat mat_amb[] = { amb.array[0], amb.array[1], amb.array[2] };
	GLfloat mat_dif[] = { dif.array[0], dif.array[1], dif.array[2] };
	// Set material properties, as defined above 
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiny);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif);
}



void genObjects() {
    srand(time(NULL));
    int x, z;
    for (int i = 0; i < cosas; i++) {
        //srand(time(NULL));
		z = rand() % (int(largo)-30) + int(meta);  //in the range 100 to 380
        x = rand() % 10 + (-5);  //in the range -5 a 5
        lento[i].x = x;
        lento[i].y = 0.5;
		lento[i].z = z;
		lento[i].p = 1.0;
        
        
        printf(" Lento x: %i z: %i \n", x, z);
        
		z = rand() % (int(largo) - 30) + int(meta);  //in the range 100 to 380
        x = rand() % 10 + (-5);  //in the range -5 a 5
        rapido[i].x = x;
        rapido[i].y = 0.5;
        rapido[i].z = z;
		rapido[i].p = 1.0;
        
        printf(" Repido x: %i z: %i \n", x, z);
        
		z = rand() % (int(largo) - 30) + int(meta);  //in the range 100 to 380
        x = rand() % 10 + (-5);  //in the range -5 a 5
        danio[i].x = x;
        danio[i].y = 0.5;
        danio[i].z = z;
		danio[i].p = 1.0;
        
        printf(" Danio x: %i z: %i \n", x, z);
        
        
		z = rand() % (int(largo) - 30) + int(meta);  //in the range 100 to 380
        x = rand() % 10 + (-5);  //in the range -5 a 5
        vida[i].x = x;
        vida[i].y = 0.5;
        vida[i].z = z;
		vida[i].p = 1.0;
        
        printf(" Vida x: %i z: %i \n", x, z);


		z = rand() % (int(largo) - 30) + int(meta);  //in the range 100 to 380
		x = rand() % 10 + (-5);  //in the range -5 a 5
		invertir[i].x = x;
		invertir[i].y = 0.5;
		invertir[i].z = z;
		invertir[i].p = 1.0;

		printf(" Invertir x: %i z: %i \n", x, z);
    }
}

#include "TextureSetup.h"
static GLuint name[6];
RGBpixmap myTexs[6];

void SetMaterials(GLfloat *mat_surf, GLfloat *mat_spec, GLfloat *mat_shiny)
{
	// Set material properties, as defined above
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiny);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_surf);
}

void SetLights(GLenum theLight, GLfloat *position, GLfloat *color)
{
	// Finish setting up the two lights (position, and component values (specular and diffuse))
	glLightfv(theLight, GL_POSITION, position);
	glLightfv(theLight, GL_DIFFUSE, color);
	glLightfv(theLight, GL_SPECULAR, color);

	// Activate (enable) lights
	glEnable(theLight);
}

#pragma region "Modelo jerarquico coches"

typedef struct treenode {
    // Transform
    void(*transform)();
    // Render
    void(*draw)();
    // Child
    struct treenode *child;
    // Sibling
    struct treenode *sibling;
} treenode;

//4 roots
treenode root;
treenode root2;


//car body
treenode carBody;

//carFront
treenode carFront;

//right tire
treenode rightFrontTire;
treenode rightBackTire;


//left arm
treenode leftFrontTire;
treenode leftBackTire;


//Angle for base
static float angle10 = 0.0;



// Set the increment
static float angleIncrement = 5.0;


void drawCylinder(GLdouble baseRadius,
                  GLdouble topRadius,
                  GLdouble height,
                  GLint slices,
                  GLint stacks)
{
    GLUquadricObj *qobj;
    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluCylinder(qobj, baseRadius, topRadius, height, slices, stacks);
}
//---------------------------------------------------------------------------------------Draw Car--------------------------------------------

//DrawCarBody-----------------------------------------------
void DrawCarBody(void)
{

    
    glPushMatrix();
    
    glTranslated(0.0, 2, 0.0);
    glScalef(2.0, 2, 3.0);
    glutSolidCube(1.0);
    glPopMatrix();
}

//DrawCarHead-----------------------------------------------
void DrawCarHead(void)
{
    glPushMatrix();
    glTranslated(0, 1.5, -2);
    glScalef(2, 1, 1);
    glutSolidCube(1);
    glPopMatrix();
}

//DrawRightFrontTire-----------------------------------------------------------------------------
void DrawRightFrontTire(void)
{
    glPushMatrix();
	pintar({ 0.50, 0.50, 0.50 }, { 32.0 }, { 0.0, 0.0, 0.0 }, { 0.01, 0.01, 0.01 });
    glRotated(90, 0, 1, 0);
    glTranslated(1.5, 1.0, 1);
    drawCylinder(0.5, 0.5, 0.5, 10, 10);
    glPopMatrix();
    
}

//DrawLeftFrontTire-----------------------------------------------------------------------------
void DrawLeftFrontTire(void)
{
    glPushMatrix();
	pintar({ 0.50, 0.50, 0.50 }, { 32.0 }, { 0.0, 0.0, 0.0 }, { 0.01, 0.01, 0.01 });
    glRotated(90, 0, 1, 0);
    glTranslated(1.5, 1.0, -1.5);
    drawCylinder(0.5, 0.5, 0.5, 10, 10);
    glPopMatrix();
    
}

//DrawRightBackTire-----------------------------------------------------------------------------
void DrawRightBackTire(void)
{

    
    glPushMatrix();
	pintar({ 0.50, 0.50, 0.50 }, { 32.0 }, { 0.0, 0.0, 0.0 }, { 0.01, 0.01, 0.01 });
    glRotated(90, 0, 1, 0);
    glTranslated(-1, 1.0, 1);
    drawCylinder(0.5, 0.5, 0.5, 10, 10);
    glPopMatrix();
}

//DrawRightBackTire-----------------------------------------------------------------------------
void DrawLeftBackTire(void)
{
    glPushMatrix();
	pintar({ 0.50, 0.50, 0.50 }, { 32.0 }, { 0.0, 0.0, 0.0 }, { 0.01, 0.01, 0.01 });
    glRotated(90, 0, 1, 0);
    glTranslated(-1, 1.0, -1.5);
    drawCylinder(0.5, 0.5, 0.5, 10, 10);
    glPopMatrix();
    
}



//---------------------------------------------------------Body Transformations-------------------------------------

//Base Transformation--------------------------------------------------------------------------
void CarBody() {
    glRotatef(angle10, 0.0, 1.0, 0.0);
}

//Head Transformation--------------------------------------------------------------------------
void FrontPartCar() {
    glRotatef(0, 0.0, 1.0, 0.0);
}

//Right Hand Transfromations--------------------------------------------------------------------------

void TransformRightFrontTire() {
    //DontMove
}

void TransformLeftFrontTire() {
    //DontMove
}

void TransformRightBackTire() {
    //DontMove
}


void TransformLeftBackTire() {
    //DontMove
}





void TransformLeftElbowToWrist() {
    //DontMove
}

void TransformLeftWristToHand() {
    //DontMove
}


//Car Starting Positions--------------------------------------------------------------------------
void TransformR1() {
 //DontMove
    
}

void TransformR2() {
    glTranslated(4.0, 0.0, 0.0);
}




void DrawNothing() {
    
}



void CreateRobot()
{
    
    //Root
    root.transform = TransformR1;
    root.draw = DrawNothing;
    root.child = &carBody;
    root.sibling = NULL;
    
    /*root2.transform = TransformR2;
     root2.draw = DrawNothing;
     root2.child = &carBody;
     root2.sibling = NULL;*/
    
    
    //CarBody
    carBody.transform = CarBody;
    carBody.draw = DrawCarBody;
    carBody.child = &carFront;
    carBody.sibling = NULL;
    
    //CarFrontPart
    carFront.transform = FrontPartCar;
    carFront.draw = DrawCarHead;
    carFront.child = NULL;
    carFront.sibling = &rightFrontTire;
    
    
    //RightFrontTire
    rightFrontTire.transform = TransformRightFrontTire;
    rightFrontTire.draw = DrawRightFrontTire;
    rightFrontTire.child = NULL;
    rightFrontTire.sibling = &leftBackTire;
    
    //LeftBackTire
    leftBackTire.transform = TransformLeftBackTire;
    leftBackTire.draw = DrawLeftBackTire;
    leftBackTire.child = NULL;
    leftBackTire.sibling = &rightBackTire;
    
    //RightBackTire
    rightBackTire.transform = TransformRightBackTire;
    rightBackTire.draw = DrawRightBackTire;
    rightBackTire.child = NULL;
    rightBackTire.sibling = &leftFrontTire;
    
    
    //LeftFrontTire
	
    leftFrontTire.transform = TransformLeftFrontTire;
    leftFrontTire.draw = DrawLeftFrontTire;
    leftFrontTire.child = NULL;
    leftFrontTire.sibling = NULL;
    
    
    
}

void traverse(treenode *node)
{
    glPushMatrix();
    // Transform
    node->transform();
    // Draw
    node->draw();
    // Child
    if (node->child != NULL) traverse(node->child);
    glPopMatrix();
    // Sibling
    if (node->sibling != NULL) traverse(node->sibling);
}
#pragma endregion

void DrawObjetos() {
    float distance1 , distance2;
	for (int i = 0; i < cosas; i++) {

		if (lento[i].p) {
			distance1 = sqrt((lento[i].x - (xP1 + 0.0))*(lento[i].x - (xP1 + 0.0)) + (lento[i].z - zP1)*(lento[i].z - zP1));
			distance2 = sqrt((lento[i].x - (xP2 + 0.0))*(lento[i].x - (xP2 + 0.0)) + (lento[i].z - zP2)*(lento[i].z - zP2));
			if (distance1 > 1 && distance2 > 1) {
				glPushMatrix();

				//pintar({ 0.256777, 0.137622, 0.086014 }, { 12.8 }, { 0.19125, 0.0735, 0.0225 }, { 0.7038, 0.27048, 0.0828 });
				//Cobre
				pintar({ 0.580594f, 0.223257f, 0.0695701f }, { 51.2f }, { 0.2295f, 0.08825f, 0.0275f }, { 0.5508f, 0.2118f, 0.066f });
				/*glMaterialfv(GL_FRONT, GL_SPECULAR, gold_Specular);
				glMaterialfv(GL_FRONT, GL_SHININESS, gold_Shinny);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, gold_Diffuse);
				glMaterialfv(GL_FRONT, GL_AMBIENT, gold_Ambient);*/
				glTranslated(lento[i].x, lento[i].y, lento[i].z);
				glColor3f(0.7, 0.3, 0.0);
				glutSolidSphere(0.5, 10, 10);
				glPopMatrix();
			}
			else {
				lento[i].y = 20.5;
				lento[i].p = 0.0;
				if (distance1 <= 1 && v1 > (v11 / 2.0)) { v1 -= v1*0.2; if (g1<g11 *1.5) g1 += (g11*0.35); }
				if (distance2 <= 1 && v1 > (v11 / 2.0)) { v2 -= v1*0.2; if (g2<g22 *1.5) g2 += (g22*0.35); }
			}
		}

		if (rapido[i].p) {
			distance1 = sqrt((rapido[i].x - (xP1 + 0.0))*(rapido[i].x - (xP1 + 0.0)) + (rapido[i].z - zP1)*(rapido[i].z - zP1));
			distance2 = sqrt((rapido[i].x - (xP2 + 0.0))*(rapido[i].x - (xP2 + 0.0)) + (rapido[i].z - zP2)*(rapido[i].z - zP2));
			if (distance1 > 1 && distance2 > 1) {
				glPushMatrix();
				//cyan
				pintar({ 0.50196078f,0.50196078f,0.50196078f }, { 32.0 }, { 0.0f,0.1f,0.06f }, { 0.0f,0.50980392f,0.50980392f });
				glTranslated(rapido[i].x, rapido[i].y, rapido[i].z);
				glColor3f(0.2, 0.4, 0.7);
				glutSolidSphere(0.5, 10, 10);
				glPopMatrix();
			}
			else {
				rapido[i].y = 20.5;
				rapido[i].p = 0.0;
				if (distance1 <= 1) { v1 += v1*0.2; if (g1>g11 + (g11*0.35)) g1 -= (g11*0.35); }
				if (distance2 <= 1) { v2 += v2*0.2; if (g2>g22 + (g22*0.35)) g2 -= (g22*0.35); }
			}
		}

		/*glPushMatrix();
		 glTranslated(rapido[i].x, -0.5, rapido[i].z);
		 glColor3f(0.2, 0.4, 0.7);
		 glutSolidSphere(0.5, 10, 10);
		 glPopMatrix();*/

		if (danio[i].p) {
			distance1 = sqrt((danio[i].x - (xP1 + 0.0))*(danio[i].x - (xP1 + 0.0)) + (danio[i].z - zP1)*(danio[i].z - zP1));
			distance2 = sqrt((danio[i].x - (xP2 + 0.0))*(danio[i].x - (xP2 + 0.0)) + (danio[i].z - zP2)*(danio[i].z - zP2));
			if (distance1 > 1 && distance2 > 1) {
				glPushMatrix();
				//pintar({ 0.508273, 0.508273, 0.508273 }, { 51.2 }, { 0.19225, 0.19225, 0.19225 }, { 0.50754, 0.50754, 0.50754 });
				//gold
				//pintar({ 0.992157, 0.941176, 0.807843 }, { 27.8974 }, { 0.329412, 0.223529, 0.027451 }, { 0.780392, 0.568627, 0.113725 });
				//Rojo
				pintar({ 0.727811f, 0.626959f, 0.626959f }, { 76.8 }, { 0.1745f, 0.01175f, 0.01175f }, { 0.61424f, 0.04136f, 0.04136f });
				glTranslated(danio[i].x, danio[i].y, danio[i].z);
				glColor3f(0.9, 0.2, 0.1);
				glutSolidSphere(0.5, 10, 10);
				glPopMatrix();
			}
			else {
				danio[i].y = 20.5;
				danio[i].p = 0.0;
				if (distance1 <= 1) { vida1 -= 1.0; /*g1 = abs(g1);*/}
				if (distance2 <= 1) { vida2 -= 1.0; /*g2 = abs(g2);*/}
			}
		}
        
        /*glPushMatrix();
         glTranslated(danio[i].x, -0.5, danio[i].z);
         glColor3f(0.9, 0.2, 0.1);
         glutSolidSphere(0.5, 10, 10);
         glPopMatrix();*/
        if (i < int(cosas*0.3) && vida[i].p) {
            distance1 = sqrt((vida[i].x - (xP1 + 0.0))*(vida[i].x - (xP1 + 0.0)) + (vida[i].z - zP1)*(vida[i].z - zP1));
            distance2 = sqrt((vida[i].x - (xP2 + 0.0))*(vida[i].x - (xP2 + 0.0)) + (vida[i].z - zP2)*(vida[i].z - zP2));
            if (distance1 > 1 && distance2 > 1) {
                glPushMatrix();
				//Verde
				pintar({ 0.633f, 0.727811f, 0.633f}, { 76.8f }, { 0.0215f, 0.1745f, 0.0215f }, { 0.07568f, 0.61424f, 0.07568f });
                glTranslated(vida[i].x, vida[i].y, vida[i].z);
                glColor3f(0.1, 0.9, 0.2);
                glutSolidSphere(0.5, 10, 10);
                glPopMatrix();
            }
            else {
				vida[i].y = 20.5;
				vida[i].p = 0.0;
				//printf("vida %i", i);
				if (distance1 <= 1 && vida1 < 7.0) { vida1 += 1.0; /*g1 = abs(g1);*/}
				if (distance2 <= 1 && vida2 < 7.0) { vida2 += 1.0; /*g2 = abs(g2);*/}
            }
        }

		//gold
		//pintar({ 0.992157, 0.941176, 0.807843 }, { 27.8974 }, { 0.329412, 0.223529, 0.027451 }, { 0.780392, 0.568627, 0.113725 });
        /*
		if (i < int(cosas*0.1) && invertir[i].p) {
			distance1 = sqrt((invertir[i].x - (xP1 + 0.0))*(invertir[i].x - (xP1 + 0.0)) + (invertir[i].z - zP1)*(invertir[i].z - zP1));
			distance2 = sqrt((invertir[i].x - (xP2 + 0.0))*(invertir[i].x - (xP2 + 0.0)) + (invertir[i].z - zP2)*(invertir[i].z - zP2));
			if (distance1 > 1 && distance2 > 1) {
				glPushMatrix();
				//Gold
				pintar({ 0.992157, 0.941176, 0.807843 }, { 27.8974 }, { 0.329412, 0.223529, 0.027451 }, { 0.780392, 0.568627, 0.113725 });
				glTranslated(invertir[i].x, invertir[i].y, invertir[i].z);
				glColor3f(0.1, 0.9, 0.2);
				glutSolidSphere(0.5, 10, 10);
				glPopMatrix();
			}
			else {
				invertir[i].y = 20.5;
				invertir[i].p = 0.0;
				//printf("vida %i", i);
				if (distance1 <= 1) { g2 = -abs(g2); }
				if (distance2 <= 1) { g1 = -abs(g1); }
			}
		}*/
        
        /*glPushMatrix();
         glTranslated(vida[i].x, -0.5, vida[i].z);
         glColor3f(0.1, 0.9, 0.2);
         glutSolidSphere(0.5, 10, 10);
         glPopMatrix();*/
    }
}





void DrawVida(float vida) {
    float esp = 0.5;
	//Verde
	pintar({ 0.633f, 0.727811f, 0.633f }, { 76.8f }, { 0.0215f, 0.1745f, 0.0215f }, { 0.07568f, 0.61424f, 0.07568f });
    for (int i = lround(vida); i > 0; i--) {
        glPushMatrix();
        glTranslated(redCubeCamPosX - 2.5 + i, 6.5, redCubeCamPosZ - 5);
        glColor3f(0.3, 0.8, 0.2);
        glutSolidSphere(0.4, 10, 10);
        glPopMatrix();
    }
}

void DrawVida2(float vida) {
    float esp = 0.5;
	//Verde
	pintar({ 0.633f, 0.727811f, 0.633f }, { 76.8f }, { 0.0215f, 0.1745f, 0.0215f }, { 0.07568f, 0.61424f, 0.07568f });
    for (int i = lround(vida); i > 0; i--) {
        glPushMatrix();
        glTranslated(blueCubeCamPosX - 5.6 + i, 6.5, blueCubeCamPosZ - 5);
        glColor3f(0.2, 0.9, 0.4);
        glutSolidSphere(0.4, 10, 10);
        glPopMatrix();
    }
}





void DrawPista() {

	GLfloat grey_Diffuse[] = { 0.50754, 0.50754, 0.50754, 1.0 };
	GLfloat grey_Specular[] = { 0.508273,	0.508273,	0.508273, 1.0 };
	GLfloat grey_Shinny[] = { 0.4 };
	GLfloat grey_Ambient[] = { 0.19225, 0.19225, 0.19225, 1.0 };

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, name[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, grey_Specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, grey_Shinny);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey_Diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, grey_Ambient);

	// Texture coords
	GLfloat s[4], t[4];

	/*s[0] = -5.0f;
	t[0] = 0.0f;

	s[1] = 5.0f;
	t[1] = 0.0f;

	s[2] = 5.0f;
	t[2] = 300.0f;

	s[3] = -5.0f;
	t[3] = 300.0f;*/

	s[0] = 0.0f;
	t[0] = 0.0f;

	s[1] = 1.0f;
	t[1] = 0.0f;

	s[2] = 1.0f;
	t[2] = 100.0f;

	s[3] = 0.0f;
	t[3] = 100.0f;


	glTranslated(-1.0, 0, 0.0);
	glTranslated(1, -1, 0.0);
	//glScalef(13, 0.1, 300);
	/*glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, name[0]);
	glutSolidCube(1);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);*/

	glBegin(GL_QUADS);

	glTexCoord2f(s[0], t[0]);
	glNormal3f(0, 1, 0);
	glVertex3f(-7.0, 0, meta - 6);

	glTexCoord2f(s[1], t[1]);
	glNormal3f(0, 1, 0);
	glVertex3f(7.0, 0, meta - 6);

	glTexCoord2f(s[2], t[2]);
	glNormal3f(0, 1, 0);
	glVertex3f(7.0, 0, largo + meta);

	glTexCoord2f(s[3], t[3]);
	glNormal3f(0, 1, 0);
	glVertex3f(-7.0, 0, largo + meta);

	glEnd();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, name[3]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, grey_Specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, grey_Shinny);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey_Diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, grey_Ambient);

	// Texture coords
	GLfloat s2[4], t2[4];

	s2[0] = 0.0f;
	t2[0] = 0.0f;

	s2[1] = 1.0f;
	t2[1] = 0.0f;

	s2[2] = 1.0f;
	t2[2] = 1.0f;

	s2[3] = 0.0f;
	t2[3] = 1.0f;


	glTranslated(0, 0.2, -2.5);
	//glRotatef(-90.0, 1.0, 0.0, 0.0);

	glBegin(GL_QUADS);

	glTexCoord2f(s2[0], t2[0]);
	glNormal3f(0, 1, 0);
	glVertex3f(-7, 0, meta);

	glTexCoord2f(s2[1], t2[1]);
	glNormal3f(0, 1, 0);
	glVertex3f(7, 0, meta);

	glTexCoord2f(s2[2], t2[2]);
	glNormal3f(0, 1, 0);
	glVertex3f(7, 0, meta-7);

	glTexCoord2f(s2[3], t2[3]);
	glNormal3f(0, 1, 0);
	glVertex3f(-7, 0, meta-7);

	glEnd();
	glPopMatrix();

	//Negro
	pintar({ 0.50, 0.50, 0.50 }, { 32.0 }, { 0.0, 0.0, 0.0 }, { 0.01, 0.01, 0.01 });
	glPopMatrix();
	glPushMatrix();
	glTranslated(-7.0, -0.5, (largo / 2.0) + meta+0.5);
	glScalef(2, 1, largo + 20);//320
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(7.0, -0.5, (largo / 2.0) + meta+0.5);
	glScalef(2, 1, largo + 20);
	glutSolidCube(1);
	glPopMatrix();

}

void DrawCartel(float winner) {

	GLfloat grey_Diffuse[] = { 0.50754, 0.50754, 0.50754, 1.0 };
	GLfloat grey_Specular[] = { 0.508273,	0.508273,	0.508273, 1.0 };
	GLfloat grey_Shinny[] = { 0.4 };
	GLfloat grey_Ambient[] = { 0.19225, 0.19225, 0.19225, 1.0 };

	glPushMatrix();
	if (winner>0)
	glBindTexture(GL_TEXTURE_2D, name[4]);
	else glBindTexture(GL_TEXTURE_2D, name[5]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, grey_Specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, grey_Shinny);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey_Diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, grey_Ambient);

	// Texture coords
	GLfloat s[4], t[4];

	s[0] = 0.0f;
	t[0] = 0.0f;

	s[1] = 1.0f;
	t[1] = 0.0f;

	s[2] = 1.0f;
	t[2] = 1.0f;

	s[3] = 0.0f;
	t[3] = 1.0f;


	glTranslated(0, 5, -10);

	glBegin(GL_QUADS);

	glTexCoord2f(s[0], t[0]);
	glNormal3f(0, 1, 0);
	glVertex3f(-10, -10, 0);

	glTexCoord2f(s[1], t[1]);
	glNormal3f(0, 1, 0);
	glVertex3f(10, -10, 0);

	glTexCoord2f(s[2], t[2]);
	glNormal3f(0, 1, 0);
	glVertex3f(10, 0, 0);

	glTexCoord2f(s[3], t[3]);
	glNormal3f(0, 1, 0);
	glVertex3f(-10, 0, 0);

	glEnd();
	glPopMatrix();

}

void DrawFondo(float x, float z) {
	GLfloat grey_Diffuse[] = { 0.50754, 0.50754, 0.50754, 1.0 };
	GLfloat grey_Specular[] = { 0.508273,	0.508273,	0.508273, 1.0 };
	GLfloat grey_Shinny[] = { 0.4 };
	GLfloat grey_Ambient[] = { 0.19225, 0.19225, 0.19225, 1.0 };

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, name[2]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, grey_Specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, grey_Shinny);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey_Diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, grey_Ambient);

	// Texture coords
	GLfloat s[4], t[4];

	s[0] = 0.0f;
	t[0] = 0.0f;

	s[1] = 1.0f;
	t[1] = 0.0f;

	s[2] = 1.0f;
	t[2] = 1.0f;

	s[3] = 0.0f;
	t[3] = 1.0f;


	glTranslated(x, 6, z);

	glBegin(GL_QUADS);

	glTexCoord2f(s[0], t[0]);
	glNormal3f(0, 1, 0);
	glVertex3f(16, 10, 0);

	glTexCoord2f(s[1], t[1]);
	glNormal3f(0, 1, 0);
	glVertex3f(-11, 10, 0);

	glTexCoord2f(s[2], t[2]);
	glNormal3f(0, 1, 0);
	glVertex3f(-11, -16.5, 0);

	glTexCoord2f(s[3], t[3]);
	glNormal3f(0, 1, 0);
	glVertex3f(16, -16.5, 0);

	glEnd();
	glPopMatrix();
}

void DrawWinner(float winner) {
	glPushMatrix();
	glViewport(0, 0, width, height);
	glLoadIdentity();
	//gluPerspective(20.0, (GLfloat)width / (GLfloat)height, 0.10, 20.0);
	//gluLookAt(0.0, 0, 6, 0.0, 0, 0, 1.0, 0.0, 0.0);
	gluLookAt(0, 3.0, 6, winner, 1.0, -1.0, 0.0, 1.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, name[1]);
	//glutWireTeapot(1);

	DrawCartel(winner);

	glPopMatrix();
}

void DrawMinimap() {
	glPushMatrix();
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(20.0, (GLfloat)width / (GLfloat)height, 0.10, 20.0);

	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Se modificaron los valores para poder visualizar de mejor forma en 2D
	//Tambien se modifico para que el eje empieze en (0,0) a partir de la ezquina inferior izquierda
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//glBindTexture(GL_TEXTURE_2D, name[1]);
	//glutWireTeapot(1);
	glPushMatrix();
	GLfloat grey_Diffuse[] = { 0.50754, 0.50754, 0.50754, 1.0 };
	GLfloat grey_Specular[] = { 0.508273,	0.508273,	0.508273, 1.0 };
	GLfloat grey_Shinny[] = { 0.4 };
	GLfloat grey_Ambient[] = { 0.19225, 0.19225, 0.19225, 1.0 };

	glBindTexture(GL_TEXTURE_2D, name[0]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, grey_Specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, grey_Shinny);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey_Diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, grey_Ambient);

	// Texture coords
	GLfloat s[4], t[4];

	s[0] = 0.0f;
	t[0] = 0.0f;

	s[1] = 1.0f;
	t[1] = 0.0f;

	s[2] = 1.0f;
	t[2] = 100.0f;

	s[3] = 0.0f;
	t[3] = 100.0f;


	glTranslated(-9.5, 9.7, -10);
	//glRotatef(-180.0, 0.0, 0.0, 1.0);

	glBegin(GL_QUADS);

	glTexCoord2f(s[0], t[0]);
	glNormal3f(0, 1, 0);
	glVertex3f(19, -2.2, 0);

	glTexCoord2f(s[1], t[1]);
	glNormal3f(0, 1, 0);
	glVertex3f(19, 0, 0);

	glTexCoord2f(s[2], t[2]);
	glNormal3f(0, 1, 0);
	glVertex3f(0, 0, 0);

	glTexCoord2f(s[3], t[3]);
	glNormal3f(0, 1, 0);
	glVertex3f(0, -2.2, 0);

	glEnd();
	glPopMatrix();

	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, name[3]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, grey_Specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, grey_Shinny);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey_Diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, grey_Ambient);

	// Texture coords
	GLfloat s2[4], t2[4];

	s2[0] = 0.0f;
	t2[0] = 0.0f;

	s2[1] = 1.0f;
	t2[1] = 0.0f;

	s2[2] = 1.0f;
	t2[2] = 1.0f;

	s2[3] = 0.0f;
	t2[3] = 1.0f;


	//glTranslated(0, 0.2, -2.5);
	//glRotatef(-90.0, 1.0, 0.0, 0.0);
	//glTranslated(-9.5, 9.7, -10);
	glTranslated(8, 9.03, -9);

	glBegin(GL_QUADS);

	glTexCoord2f(s2[0], t2[0]);
	glNormal3f(0, 1, 0);
	//glVertex3f(1.5, -2.2, 0);
	glVertex3f(0, 0, 0);

	glTexCoord2f(s2[1], t2[1]);
	glNormal3f(0, 1, 0);
	//glVertex3f(1.5, 0, 0);
	glVertex3f(0, -2.0, 0);

	glTexCoord2f(s2[2], t2[2]);
	glNormal3f(0, 1, 0);
	//glVertex3f(0, 0, 0);
	glVertex3f(1.2, -2.0, 0);

	glTexCoord2f(s2[3], t2[3]);
	glNormal3f(0, 1, 0);
	//glVertex3f(0, -2.2, 0);
	glVertex3f(1.2, 0, 0);

	glEnd();
	glPopMatrix();

	glPushMatrix();
	pintar({ 0.256777, 0.137622, 0.086014 }, { 12.8 }, { 0.19125, 0.0735, 0.0225 }, { 0.7038, 0.27048, 0.0828 });
	//3.0 derecha  5.0 - 3.0
	//3.72 izquierda  -5.0 - 3.72
	//10 hay 0.72
	
	//3 -- -3+5=2 == 0.144
	//-4 -- 4+5=9 == 0.648
	//5 -- -5+5=0 == 0
	float xxP1 = ((-xP1 + 5)*0.72) / 10;
	//glTranslated(-3.8,3.72,-1);
	//zP1 = largo + meta - 2.0;

	//-3.8 inicio   largo + meta - 2.0 - meta
	//3.3 fin    103.0 (meta+3) - meta
	//largo - 5.0 hay 7.1

	//120 -- (120-100(meta))-3=17 == 17*7.1/595(largo-2) = 0.201  , 598=7.1, 0=0
	float zzP1 = ((zP1 - meta-3)*7.1) / (largo-2);
	glTranslated(-zzP1+3.3, 3.0 + xxP1, -1);
	glScalef(0.2, 0.2, 0.2);
	//glutSolidCube(0.4);
	glBegin(GL_QUADS);

	glNormal3f(0, 1, 0);
	glVertex3f(0, 0, 0);

	glNormal3f(0, 1, 0);
	glVertex3f(0, 0.8, 0);

	glNormal3f(0, 1, 0);
	glVertex3f(0.9, 0.8, 0);

	glNormal3f(0, 1, 0);
	glVertex3f(0.9, 0, 0);

	glEnd();


	glPopMatrix();

	glPushMatrix();
	pintar({ 0.992157, 0.941176, 0.807843 }, { 27.8974 }, { 0.329412, 0.223529, 0.027451 }, { 0.780392, 0.568627, 0.113725 });
	//glutSolidCube(0.2);
	//3.0 derecha  5.0 - 3.0
	//3.72 izquierda  -5.0 - 3.72
	//10 hay 0.72

	//3 -- -3+5=2 == 0.144
	//-4 -- 4+5=9 == 0.648
	//5 -- -5+5=0 == 0
	float xxP2 = ((-xP2 + 5)*0.72) / 10;
	//glTranslated(-3.8,3.72,-1);
	//zP1 = largo + meta - 2.0;

	//-3.8 inicio   largo + meta - 2.0 - meta
	//3.3 fin    103.0 (meta+3) - meta
	//largo - 5.0 hay 7.1

	//120 -- (120-100(meta))-3=17 == 17*7.1/595(largo-2) = 0.201  , 598=7.1, 0=0
	float zzP2 = ((zP2 - meta - 3)*7.1) / (largo - 2);
	glTranslated(-zzP2 + 3.3, 3.0 + xxP2, -1);
	glScalef(0.2, 0.2, 0.2);
	//glutSolidCube(0.4);
	glBegin(GL_QUADS);

	glNormal3f(0, 1, 0);
	glVertex3f(0, 0, 0);

	glNormal3f(0, 1, 0);
	glVertex3f(0, 0.8, 0);

	glNormal3f(0, 1, 0);
	glVertex3f(0.9, 0.8, 0);

	glNormal3f(0, 1, 0);
	glVertex3f(0.9, 0, 0);

	glEnd();


	glPopMatrix();

	
	glPopMatrix();
}

void DrawCar1(){
    
    
    
    //---------------------------Viewport 1------------------------------------
    glPushMatrix();
    glViewport(0, 0, width/2, height);
    glLoadIdentity();
    //gluPerspective(20.0, (GLfloat)width / (GLfloat)height, 0.10, 50.0);
    //glTranslated(xP1, 0.0, zP1);
    gluLookAt(redCubeCamPosX+2, 3.0, redCubeCamPosZ+3,redCubeCamPosX, 0.0, -1.0, 0.0, 1.0, 0.0);
    //gluLookAt(redCubeCamPosX, 0.0, redCubeCamPosZ+5, redCubeCamPosX, 0.0, -1.0, 0.0, 1.0, 0.0);
	
    DrawPista();
	glBindTexture(GL_TEXTURE_2D, name[1]);
	//gold
	pintar({ 0.992157, 0.941176, 0.807843 }, { 27.8974 }, { 0.329412, 0.223529, 0.027451 }, { 0.780392, 0.568627, 0.113725 });
    
    glPushMatrix();
    glTranslated(xP2, -0.5, zP2);
    glColor3f(0, 0, 1);
    //glutWireSphere(0.2, 10, 10);
    glScalef(0.5, 0.5, 0.5);
    traverse(&root);
    glPopMatrix();
    
    DrawObjetos();

	//Anaranjado
	pintar({ 0.256777, 0.137622, 0.086014 }, { 12.8 }, { 0.19125, 0.0735, 0.0225 }, { 0.7038, 0.27048, 0.0828 });
    
    glPushMatrix();
    glTranslated(xP1, -0.5, zP1);
    glColor3f(1, 0, 0);
    //glutWireSphere(0.2, 10, 10);
    glScalef(0.5, 0.5, 0.5);
    traverse(&root);
    glPopMatrix();
    
    
    DrawVida(vida1);
	DrawFondo(redCubeCamPosX-1, redCubeCamPosZ - 16.5);

	
    glPopMatrix();
    
}

void DrawCar2(){
    //----------------------------Viewport 2-----------------------------------
    glPushMatrix();
    glViewport(width/2, 0, width/2, height);
    glLoadIdentity();
    //gluPerspective(20.0, (GLfloat)width/2 / (GLfloat)height, 0.10, 50.0);
    //gluLookAt(blueCubeCamPosX, 0.0, blueCubeCamPosZ, blueCubeCamPosX, 0.0, 0, 0.0, 1.0, 0.0);
    gluLookAt(blueCubeCamPosX-2, 3.0, blueCubeCamPosZ+3, blueCubeCamPosX, 0.0, -1.0, 0.0, 1.0, 0.0);
    
    DrawPista();
	glBindTexture(GL_TEXTURE_2D, name[1]);
	//Anaranjado
	pintar({ 0.256777, 0.137622, 0.086014 }, { 12.8 }, { 0.19125, 0.0735, 0.0225 }, { 0.7038, 0.27048, 0.0828 });
    /*glPushMatrix();
     glTranslated(xP1, 0.0, zP1);
     glColor3f(1, 0, 0);
     glutWireCube(0.2);
     glPopMatrix();*/
    glPushMatrix();
    glTranslated(xP1, -0.5, zP1);
    glColor3f(1, 0, 0);
    //glutWireSphere(0.2, 10, 10);
    glScalef(0.5, 0.5, 0.5);
    traverse(&root);
    glPopMatrix();
    
    
    DrawObjetos();

	//gold
	pintar({ 0.992157, 0.941176, 0.807843 }, { 27.8974 }, { 0.329412, 0.223529, 0.027451 }, { 0.780392, 0.568627, 0.113725 });
    
    glPushMatrix();
    glTranslated(xP2, -0.5, zP2);
    glColor3f(0, 0, 1);
    //glutWireCube(0.2);
    glScalef(0.5, 0.5, 0.5);
    traverse(&root);
    glPopMatrix();
    
    DrawVida2(vida2);
	//DrawFondo(redCubeCamPosX - 1, redCubeCamPosZ - 16.5);
	DrawFondo(blueCubeCamPosX-4, blueCubeCamPosZ - 16.5);
    
    glPopMatrix();
}



void CheckCollisions(){
    
    col1Left = (xP1 <= (xP2 + 1.5) && xP1 >= (xP2 + 1.0) && zP1 <= (zP2 + 1.0) && zP1 >= (zP2 - 1.2));
    col1Right = (xP1 >= (xP2 - 1.5) && xP1 <= (xP2 - 1.0) && zP1 <= (zP2 + 1.0) && zP1 >= (zP2 - 1.2));
    //if (xP1 >= (xP2 + 1.0)) printf("111111111");
    //if (col1Right) printf("colision coche 1 Derecha");
    //if (col1Left) printf("colision coche 1 Izquierda");
    //col1Front = false;
    col1Front = (xP1 <= (xP2 + 1.5) && xP1 >= (xP2 - 1.5) && zP1 >= (zP2 + 0.8) && zP1 <= (zP2 + 1.2));
    //if (col1Front) printf("colision coche 1 EnFrente");
    
    col2Front = (xP1 <= (xP2 + 1.5) && xP1 >= (xP2 - 1.5) && zP2 >= (zP1 + 0.8) && zP2 <= (zP1 + 1.2));
    //if (col2Front) printf("colision coche 2 EnFrente");
    
}


static void display(void) {
	// Clear the buffer 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glColor3f(1.0f, 0.0f, 0.0f);

	if (zP1 >= 100 && zP2 >= 100 && vida1 > 0 && vida2 > 0) {
	//if (zP1 >= 100 && zP2 >= 100) {
		if (!col1Front) {
			zP1 -= v1;
			redCubeCamPosZ -= v1;
		}
		if (!col2Front) {
			zP2 -= v2;
			blueCubeCamPosZ -= v2;
		}
		//printf(" gllookat %f,%f,%f,%f,%f,%f \n", redCubeCamPosX, 0.0, redCubeCamPosZ, redCubeCamPosX, xP1, xP2);

		
		DrawCar1();
		
		DrawCar2();
		DrawMinimap();

		CheckCollisions();

		if (izq1 && (!col1Left  && xP1 >= -5)) {
			xP1 -= g1;
			redCubeCamPosX -= g1;
		}

		if (der1 && (!col1Right  && xP1 <= 5)) {
			xP1 += g1;
			redCubeCamPosX += g1;
		}

		if (der2 && (!col1Left && xP2 <= 5)) {
			xP2 += g2;
			blueCubeCamPosX += g2;
		}

		if (izq2 && (!col1Right && xP2 >= -5)) {
			xP2 -= g2;
			blueCubeCamPosX -= g2;
		}
		int vida1int = lround(vida1);
		//printf(" Vida 1: %i 1: %f \n", vida1int, vida2);
	}
	else {
		DrawCar1();
		DrawCar2();
		DrawMinimap();
		if (vida1 == 0) {
			DrawWinner(-1.0);
		}
		else if (vida2 == 0) {
			DrawWinner(1.0);
		}
		else if (zP1 > zP2) {//Ganador Jugador 1
			DrawWinner(-1.0);
		}
		else {//Ganador Jugador 2
			DrawWinner(1.0);
		}
	}
    glFlush();
}

static void reshape(int w, int h) {
    width = w;
    height = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    glMatrixMode(GL_MODELVIEW);
}



void player1Movement(unsigned char key, int x, int y) {
    switch (key)
    {
	case 'v':
		v1 += 0.001;
		v2 += 0.001;
		v11 += 0.001;
		v22 += 0.001;
		break;
	case 'V':
		v1 -= 0.001;
		v2 -= 0.001;
		v11 -= 0.001;
		v22 -= 0.001;
		break;

	case 'g':
		g1 += 0.001;
		g11 += 0.001;
		g2 += 0.001;
		g22 += 0.001;
		break;
	case 'G':
		g1 -= 0.001;
		g2 -= 0.001;
		g11 -= 0.001;
		g22 -= 0.001;
		break;
            
            //--------------------------Player 1 Movement---------------------------------
        case 'w':
            if (!col1Front) {
                //zP1 -= 1.0;
                //redCubeCamPosZ -= 1.0;
                printf(" gllookat %f,%f,%f,%f,%f,%f \n", redCubeCamPosX, 0.0, redCubeCamPosZ, redCubeCamPosX, xP1, xP2);
            }
            break;
            
        case 's':
            if (!col2Front) {
                //zP1 += 1.0;
                //redCubeCamPosZ += 1.0;
            }
            break;
            
        case 'a':
            /*if (!col1Left  && xP1 >= -5) {
             xP1 -= increasPosition;
             redCubeCamPosX -= increasPosition;
             
             }*/
            izq1 = true;
            break;
            
            
        case 'd':
            /*if (!col1Right  && xP1 <= 5) {
             xP1 += increasPosition;
             redCubeCamPosX += increasPosition;
             }*/
            der1 = true;
            break;
            
            //--------------------------Player 2 Movement---------------------------------
            
        case 'u':
            if (!col2Front) {
                //zP2 -= increasPosition;
                //blueCubeCamPosZ -= increasPosition;
            }
            break;
            
        case 'j':
            if (!col1Front) {
                //zP2 += increasPosition;
                //blueCubeCamPosZ += increasPosition;
            }
            break;
            
        case 'k':
            /*if (!col1Left && xP2 <= 5) {
             xP2 += increasPosition;
             blueCubeCamPosX += increasPosition;
             }*/
            der2 = true;
            break;
            
        case 'h':
            /*if (!col1Right && xP2 >= -5) {
             xP2 -= increasPosition;
             blueCubeCamPosX -= increasPosition;
             }*/
            izq2 = true;
            break;
            
            //--------------------------Quit---------------------------------
        case 27:
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}

void player1Movement2(unsigned char key, int x, int y) {
    switch (key)
    {
            
            //--------------------------Player 1 Movement---------------------------------
        case 'w':
            break;
            
        case 's':
            break;
            
        case 'a':
            izq1 = false;
            break;
            
            
        case 'd':
            der1 = false;
            break;
            
            //--------------------------Player 2 Movement---------------------------------
            
        case 'u':
            break;
            
        case 'j':
            break;
            
        case 'k':
            der2 = false;
            break;
            
        case 'h':
            izq2 = false;
            break;
            
            //--------------------------Quit---------------------------------
        case 27:
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP:
		//rotX += angleInc;
		break;
	case GLUT_KEY_DOWN:
		//rotX -= angleInc;
		break;
	case GLUT_KEY_LEFT:
		//rotY -= angleInc;
		izq2 = true;
		break;
	case GLUT_KEY_RIGHT:
		//rotY += angleInc;
		der2 = true;
		break;
	}
	glutPostRedisplay();
}

void processSpecialKeys2(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP:
		//rotX += angleInc;
		break;
	case GLUT_KEY_DOWN:
		//rotX -= angleInc;
		break;
	case GLUT_KEY_LEFT:
		//rotY -= angleInc;
		izq2 = false;
		break;
	case GLUT_KEY_RIGHT:
		//rotY += angleInc;
		der2 = false;
		break;
	}
	glutPostRedisplay();
}

void init(void)
{
    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    GLfloat myPos[] = {3.0, 6.0, 290.0, 2.0};
    glLightfv(GL_LIGHT0, GL_POSITION, myPos);
    
    //GLfloat spotLight_Position[] = {0.0, 0.0, 15.0, 1.0};
    GLfloat whiteLight[] = { 1.0, 1.0, 1.0, 1.0 };
    //glLightfv(GL_LIGHT1, GL_POSITION, spotLight_Position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, whiteLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);

    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1);*/
	// Set clear (background) color
	glClearColor(0.3, 0.3, 0.3, 0.0);

	// Set shading model to use
	glShadeModel(GL_SMOOTH);

	// Enable depth testing (for hidden surface removal)
	glEnable(GL_DEPTH_TEST);

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Set light properties...
	GLfloat white_light[] = { 0.9, 0.9, 0.9, 1.0 };
	GLfloat red_light[] = { 0.7, 0.5, 0.5, 1.0 };
	GLfloat blue_light[] = { 0.1, 0.1, 1.0, 1.0 };

	// and create two lights at two positions
	GLfloat light_pos0[] = { 0.0, 3.0, 1.0, 0.0 };
	GLfloat light_pos1[] = { -1.0, -1.0, 1.0, 0.0 };

	GLfloat cutoff[] = { 3.0 };
	GLfloat expo[] = { 2.0 };



	SetLights(GL_LIGHT0, &light_pos0[0], &white_light[0]);
	///SetLights(GL_LIGHT1, &light_pos1[0], &red_light[0]);
	/*
	glLightfv(GL_LIGHT2, GL_POSITION, light_pos2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, blue_light);
	glLightfv(GL_LIGHT2, GL_SPECULAR, blue_light);
	glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, cutoff);
	glLightfv(GL_LIGHT2, GL_SPOT_EXPONENT, expo);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir);
	glEnable(GL_LIGHT2);*/




	glEnable(GL_NORMALIZE);


	// initialize texture
	glGenTextures(6, name);
	InitializeTexture(myTexs[0], &name[0], "C:/pista.bmp");
	InitializeTexture(myTexs[2], &name[2], "C:/tierra.bmp");
	InitializeTexture(myTexs[3], &name[3], "C:/meta.bmp");
	InitializeTexture(myTexs[4], &name[4], "C:/ganador1.bmp");
	InitializeTexture(myTexs[5], &name[5], "C:/ganador2.bmp");
	/*myTexs[1].SetTexColor(1.0, 215.0 / 255.0, 0.0);
	myTexs[1].MakeShadedCircle();
	myTexs[1].SetTexture(name[2]);*/
	glEnable(GL_TEXTURE_2D);
}


int main(int argc, char** argv) {
    
    
    // GLUT INITIALIZATION
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("RaceGL");
	init();

    
    CreateRobot();
    genObjects();
    
    // Register callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(player1Movement);
    glutKeyboardUpFunc(player1Movement2);
	glutSpecialFunc(processSpecialKeys);
	glutSpecialUpFunc(processSpecialKeys2);
    //glutSetKeyRepeat(1);
    // glutSpecialFunc(player2Movement);
    
    
    glutIdleFunc(display);
    // Do main loop
    glutMainLoop();
    
    
    // Exit
    return 0;
}

