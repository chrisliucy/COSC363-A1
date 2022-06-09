//  ========================================================================
//  COSC363 Computer Graphics Assignment 1 OpenGL Museum
//  Student Name: Canying Liu
//  Student ID: 47108377
//  ========================================================================
 
#include <iostream>
#include <cmath> 
#include <GL/glut.h>
#include "loadTGA.h"

#define GL_CLAMP_TO_EDGE 0x812F
using namespace std;
GLuint txId[8];

float white[4] = {1.0, 1.0, 1.0, 1.0};
float black[4] = {0.0, 0.0, 0.0, 0.0};
float grey[4] = {0.2, 0.2, 0.2, 1.0};
float mat[4] = {1.0, 0.75, 0.5, 1.0};

float angle=0, look_x, look_z=-1, eye_x, eye_z=35;  //Camera parameters
int step=0, flag=0;
int bin_direction=0, bin_x=0, bin_z=0;     //bin movement parameters
float theta = 15.0;
float earth_x=9.1, earth_z=7.;             //planets rotation parameters
int is_up = 1;                             //magnetic diamond parameters
float diamond_height = 2.5;
float rotation_angle = 0.;                 //rotation parameters
int bin_counter = 0, mag_counter;          //counter for timer
float lgt0_pos[] = {0., 30., -8., 1.};
float lgt1_pos[] = {-7.0f, 13.0f, 8.0f, 1.0f};

//--------------------------------Texture-------------------------------

void loadTexture()				
{
	glGenTextures(8, txId); 

	glBindTexture(GL_TEXTURE_2D, txId[0]);
    loadTGA("back.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, txId[1]);
    loadTGA("front.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[2]);
    loadTGA("left.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[3]);
    loadTGA("right.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[4]);
    loadTGA("bottom.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glBindTexture(GL_TEXTURE_2D, txId[5]);
    loadTGA("top.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glBindTexture(GL_TEXTURE_2D, txId[6]);
    loadTGA("walls.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	glBindTexture(GL_TEXTURE_2D, txId[7]);
    loadTGA("ufo.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

//----------------------------Normal Vector-----------------------------

void normal(float x1, float y1, float z1, 
            float x2, float y2, float z2,
		      float x3, float y3, float z3 )
{
	  float nx, ny, nz;
	  nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
	  ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
	  nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(nx, ny, nz);
}

//----------------------Mesume Exhibits Structures----------------------
//======================================================================
//--------------------UFO Structure(Sweep Surfaces)---------------------
void ufo(int shadow)
{
	if (shadow == 1) {                         //is the shadow
		glColor4f(0.2, 0.2, 0.2, 1.);
	} else {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txId[7]);
	}
	
	const int N = 61;
	float vx_init[N] = {0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 1.8, 
		2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8, 4, 4.2, 4.4, 4.6, 
		4.8, 5, 5.2, 5.4, 12, 12, 11, 10, 9, 8, 7, 6.8, 6.6, 6.4, 6.2, 
		6, 5.8, 5.6, 5.4, 5.2, 5, 4.8, 4.6, 4.4, 4.2, 4, 3.8, 3.6, 3.4, 
		3.2, 3, 2.8, 2.6, 2.4, 2.2, 2, 0};
	float vy_init[N] = {5.5, 5.469, 5.485, 5.467, 5.442, 5.408, 5.367, 
		5.319, 5.262, 5.197, 5.123, 5.041, 4.949, 4.847, 4.734, 4.61, 
		4.473, 4.323, 4.158, 3.976, 3.775, 3.551, 3.3, 3.015, 2.685,
		2.291, 1.792, 1.044, 0, -0.98, -1.07, -1.176, -1.307, -1.471, 
		-1.681, -1.73, -1.783, -1.838, -1.898, -1.961, -2.028, -2.101, 
		-2.179, -2.262, -2.353, -2.451, -2.558, -2.674, -2.801, -2.941, 
		-3.096, -3.268, -3.46, -3.676, -3.922, -4.202, -4.525, -4.902, 
		-5.348, -5.882, -7};
	float vz_init[N]= {0};
	
	float vx[N], vy[N], vz[N];
	float wx[N], wy[N], wz[N];
	
	for (int i = 0; i < N; i++) {
		vx[i] = vx_init[i];
		vy[i] = vy_init[i];
		vz[i] = vz_init[i];
	}
	
	for (int j = 0; j < 36; j++) {
		for (int i = 0; i < N; i++) {
			wx[i] = vx[i] * cos(0.17453) + vz[i] * sin(0.17453);
			wy[i] = vy[i];
			wz[i] = -vx[i] * sin(0.17453) + vz[i] * cos(0.17453);
		}
		glBegin(GL_TRIANGLE_STRIP);
			for (int i = 0; i < N; i++) {
				if (i > 0) normal(vx[i-1], vy[i-1], vz[i-1],
				                  wx[i-1], wy[i-1], wz[i-1],
				                  vx[i], vy[i], vz[i]);
				if (shadow == 0) glTexCoord2f((float)j/36, (float)i/(N-1));
				glVertex3f(vx[i], vy[i], vz[i]);
				if (i > 0) normal(wx[i-1], wy[i-1], wz[i-1],
				                  wx[i], wy[i], wz[i],
				                  vx[i], vy[i], vz[i]);
				if (shadow == 0) glTexCoord2f((float)(j+1)/36, (float)i/(N-1));
				glVertex3f(wx[i], wy[i], wz[i]);
			}
		glEnd();
		for (int i = 0; i < N; i++) {
			vx[i] = wx[i];
			vy[i] = wy[i];
			vz[i] = wz[i];
		}
	}
	
	if (shadow == 0) glDisable(GL_TEXTURE_2D);
}
//-------------------------Robot Bin Structure--------------------------
void robotBin()
{
	glColor3f(0.133, 0.545, 0.133);    //eyes
    glPushMatrix();
      glTranslatef(-0.75, 5, 0);
      glutSolidTorus(0.2, 0.5, 20., 20.);
    glPopMatrix();
    
    glPushMatrix();
      glTranslatef(0.75, 5, 0);
      glutSolidTorus(0.2, 0.5, 20., 20.);
    glPopMatrix();
    
    glColor3f(0., 0., 0.);             //eye balls
    glPushMatrix();
      glTranslatef(-0.75, 5, 0);
      glutSolidSphere(0.25, 20., 20.);
    glPopMatrix();
    
    glPushMatrix();
      glTranslatef(0.75, 5, 0);
      glutSolidSphere(0.25, 20., 20.);
    glPopMatrix();
    
    glColor3f(0., 1., 0.);             //body
    glPushMatrix();
      glTranslatef(0, 3, 0);
      glScalef(2.5, 3, 2.5);
      glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0., 0.5, 0.);            //right leg
    glPushMatrix();
      glTranslatef(-0.65, 1.1, 0);
      glRotatef(-theta, 1, 0, 0);
      glTranslatef(0.65, -1.1, 0);
      glTranslatef(-0.65, 0.75, 0.);
      glScalef(0.75, 1.9, 1);
      glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();                    //left leg
      glTranslatef(0.65, 1.1, 0);
      glRotatef(theta, 1, 0, 0);
      glTranslatef(-0.65, -1.1, 0);
      glTranslatef(0.65, 0.75, 0.);
      glScalef(0.75, 1.9, 1);
      glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();                    //right arm
	  glTranslatef(-1.625, 4, 0);
      glRotatef(theta, 1, 0, 0);
      glTranslatef(1.625, -4, 0);
      glTranslatef(-1.625, 3, 0);
      glScalef(0.75, 1.7, 1);
      glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();                    //left arm
      glTranslatef(1.625, 4, 0);
	  glRotatef(-theta, 1, 0, 0);
      glTranslatef(-1.625, -4, 0);
      glTranslatef(1.625, 3, 0);
      glScalef(0.75, 1.7, 1);
      glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0., 0., 0.);            //mouth(to chuck in rubbish)
    glPushMatrix();
		glTranslatef(0., 3.5, 1.25);
        glScalef(1.8, 1, 0.5);
        glutSolidCube(1);
	glPopMatrix();
}
//-------------------------Planets Orbit Structure----------------------
void planets()
{
    glColor3f(1., 0.549, 0.);              //the Sun
    glPushMatrix();
		glTranslatef(7, 5, 7);
		glRotatef(15*rotation_angle*7.25, 0, 1, 0);
        glutSolidSphere(0.7, 30, 30);
	glPopMatrix();

	glColor3f(0., 0., 1);                  //the Earth
	    glPushMatrix();
		glTranslatef(earth_x, 5, earth_z);
		glRotatef(rotation_angle*23.4/3, 0, 1, 0);
        glutSolidSphere(0.4, 30, 30);
	glPopMatrix();

	glColor3f(0.863, 0.863, 0.863);        //the Moon
	    glPushMatrix();
		glTranslatef(earth_x+cos(-12*rotation_angle), 5, earth_z+sin(-12*rotation_angle));
        glRotatef(12*rotation_angle*6.68, 0, 1, 0);
        glutSolidSphere(0.2, 30, 30);
    glPopMatrix();
}
//----------------------Magnetic Diamond Structure----------------------
void magnet()
{
	GLUquadric *q;
	float shadowMat[16] = {lgt1_pos[1],0,0,0, -lgt1_pos[0],0,-lgt1_pos[2],-1, 
		                   0,0,lgt1_pos[1],0, 0,0,0,lgt1_pos[1]};

	glColor3f(0.545, 0., 0.);           //the north pole
    glPushMatrix();
	  glTranslatef(-7, 10, 8);
	  glRotatef(-90, 1, 0, 0);
	  q = gluNewQuadric();
	  gluDisk(q, 0.5, 1, 20, 20); 
	  gluQuadricDrawStyle(q, GLU_FILL);
	glPopMatrix();
	
	glDisable(GL_LIGHTING);             //shadow of the south pole
    glPushMatrix();
	  glMultMatrixf(shadowMat);
	  glTranslatef(-7, 1.5, 8);
	  glRotatef(-90, 1, 0, 0);
	  glColor4f(0.2, 0.2, 0.2, 1.0);
	  q = gluNewQuadric();
	  gluDisk(q, 0.5, 1, 20, 20); 
	  gluQuadricDrawStyle(q, GLU_FILL);
	glPopMatrix();
	
	glEnable(GL_LIGHTING);              //the south pole
	glColor3f(0., 0., 0.502);
    glPushMatrix();
	  glTranslatef(-7, 1.5, 8);
	  glRotatef(-90, 1, 0, 0);
	  q = gluNewQuadric();
	  gluDisk(q, 0.5, 1, 20, 20); 
	  gluQuadricDrawStyle(q, GLU_FILL);
	glPopMatrix();
	
	glDisable(GL_LIGHTING);             //shadow of the magnetic diamond
    glPushMatrix();
	  glMultMatrixf(shadowMat);
	  glTranslatef(-7, diamond_height, 8);
	  glScalef(0.7, 0.7, 0.7);
	  glColor4f(0.2, 0.2, 0.2, 1.0);
	  glutSolidOctahedron();
	glPopMatrix();

	glEnable(GL_LIGHTING);              //the magnetic diamond
    glColor3f(0.298, 0., 0.6);
    glPushMatrix();
	  glTranslatef(-7, diamond_height, 8);
	  glScalef(0.7, 0.7, 0.7);
	  glutSolidOctahedron();
	glPopMatrix();

}
//======================================================================

//-----------------------------Mesume Structure-------------------------

void mesume()
{
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[6]);

 	glBegin(GL_QUADS);
	  glTexCoord2f(0.0, 1.0);   glVertex3f(-10, 18, -16);   //back
	  glTexCoord2f(0.0, 0.0);   glVertex3f(-10, 0, -16);
	  glTexCoord2f(1.0, 0.0);   glVertex3f(10, 0, -16);
	  glTexCoord2f(1.0, 1.0);   glVertex3f(10, 18, -16);
	
	  glTexCoord2f(0.0, 1.0);   glVertex3f(-16, 18, 0);   //left-back
	  glTexCoord2f(0.0, 0.0);   glVertex3f(-16, 0, 0);
	  glTexCoord2f(1.0, 0.0);   glVertex3f(-10, 0, -16);
	  glTexCoord2f(1.0, 1.0);   glVertex3f(-10, 18, -16);
	
	  glTexCoord2f(0.0, 1.0);   glVertex3f(10, 18, -16);   //right-back
	  glTexCoord2f(0.0, 0.0);   glVertex3f(10, 0, -16);
	  glTexCoord2f(1.0, 0.0);   glVertex3f(16, 0, 0);
	  glTexCoord2f(1.0, 1.0);   glVertex3f(16, 18, 0);
	
	  glTexCoord2f(0.0, 1.0);   glVertex3f(-10, 18, 16);   //left-front
	  glTexCoord2f(0.0, 0.0);   glVertex3f(-10, 0, 16);
	  glTexCoord2f(1.0, 0.0);   glVertex3f(-16, 0, 0);
	  glTexCoord2f(1.0, 1.0);   glVertex3f(-16, 18, 0);
	
	  glTexCoord2f(0.0, 1.0);   glVertex3f(16, 18, 0);   //right-front
	  glTexCoord2f(0.0, 0.0);   glVertex3f(16, 0, 0);
	  glTexCoord2f(1.0, 0.0);   glVertex3f(10, 0, 16);
	  glTexCoord2f(1.0, 1.0);   glVertex3f(10, 18, 16);

	  glTexCoord2f(0.0, 1.0);   glVertex3f(-10, 18, 16);   //front
	  glTexCoord2f(0.0, 0.83);   glVertex3f(-10, 15, 16);
	  glTexCoord2f(0.3, 0.83);   glVertex3f(-4, 15, 16);
	  glTexCoord2f(0.375, 1.0);   glVertex3f(-2.5, 18, 16);
	
	  glTexCoord2f(0.0, 0.83);   glVertex3f(-10, 15, 16);
	  glTexCoord2f(0.0, 0.0);   glVertex3f(-10, 0, 16);
	  glTexCoord2f(0.3, 0.0);   glVertex3f(-4, 0, 16);
	  glTexCoord2f(0.3, 0.83);   glVertex3f(-4, 15, 16);
	
	  glTexCoord2f(0.65, 1);   glVertex3f(3, 18, 16);
	  glTexCoord2f(0.75, 0.83);   glVertex3f(5, 15, 16);
	  glTexCoord2f(1, 0.83);   glVertex3f(10, 15, 16);
	  glTexCoord2f(1, 1);   glVertex3f(10, 18, 16);
	
	  glTexCoord2f(0.75, 0.83);   glVertex3f(5, 15, 16);
	  glTexCoord2f(0.75, 0);   glVertex3f(5, 0, 16);
	  glTexCoord2f(1, 0);   glVertex3f(10, 0, 16);
	  glTexCoord2f(1, 0.83);   glVertex3f(10, 15, 16);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);                        //entrance 
	  glColor3f(0.831, 0.831, 0.831);
	  glVertex3f(-4, 15, 16);
	  glVertex3f(-4, 13.5, 16);
	  glVertex3f(3, 13.5, 16);
	  glVertex3f(3, 15, 16);

	  glColor3f(0.725, 0.689, 0.689);
	  glVertex3f(0, 23, 16);
	  glVertex3f(-4, 15, 16);
	  glVertex3f(3, 15, 16);
	  glVertex3f(3, 18, 16);
	
	  glColor3f(0.643, 0.580, 0.580);
	  glVertex3f(3, 18, 16);
	  glVertex3f(3, 0, 16);
	  glVertex3f(5, 0, 16);
	  glVertex3f(5, 15, 16);
	glEnd();
    glEnable(GL_LIGHTING);
}

//-----------------------------The Skybox-------------------------------

void walls()
{
	glColor3f(1., 1., 1.);   //replaced by the texture
	glEnable(GL_TEXTURE_2D);
	
	////////////////////// BACK WALL ///////////////////////
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	glBegin(GL_QUADS);
	  glTexCoord2f(0., 1.);   glVertex3f(-50, 50, -50);
	  glTexCoord2f(0., 0.);   glVertex3f(-50, 0, -50);
	  glTexCoord2f(1., 0.);   glVertex3f(50, 0, -50);
	  glTexCoord2f(1., 1.);   glVertex3f(50, 50, -50);
	glEnd();
	////////////////////// FRONT WALL ///////////////////////
	glBindTexture(GL_TEXTURE_2D, txId[1]);
	glBegin(GL_QUADS);
	  glTexCoord2f(1., 1.);   glVertex3f(-50, 50, 50);
	  glTexCoord2f(1., 0.);   glVertex3f(-50, 0, 50);
	  glTexCoord2f(0., 0.);   glVertex3f(50, 0, 50);
	  glTexCoord2f(0., 1.);   glVertex3f(50, 50, 50);
	glEnd();
	////////////////////// LEFT WALL ///////////////////////
	glBindTexture(GL_TEXTURE_2D, txId[2]);
	glBegin(GL_QUADS);
	  glTexCoord2f(0., 1.);   glVertex3f(-50, 50, 50);
	  glTexCoord2f(0., 0.);   glVertex3f(-50, 0, 50);
	  glTexCoord2f(1., 0.);   glVertex3f(-50, 0, -50);
	  glTexCoord2f(1., 1.);   glVertex3f(-50, 50, -50);
	glEnd();
	////////////////////// RIGHT WALL ///////////////////////
	glBindTexture(GL_TEXTURE_2D, txId[3]);
	glBegin(GL_QUADS);
	  glTexCoord2f(0., 1.);   glVertex3f(50, 50, -50);
	  glTexCoord2f(0., 0.);   glVertex3f(50, 0, -50);
	  glTexCoord2f(1., 0.);   glVertex3f(50, 0, 50);
	  glTexCoord2f(1., 1.);   glVertex3f(50, 50, 50);
	glEnd();
	////////////////////// TOP WALL /////////////////////////
	glBindTexture(GL_TEXTURE_2D, txId[5]);
	glBegin(GL_QUADS);
	  glTexCoord2f(1., 1.);   glVertex3f(-50, 50, -50);
	  glTexCoord2f(1., 0.);   glVertex3f(-50, 50, 50);
	  glTexCoord2f(0., 0.);   glVertex3f(50, 50, 50);
	  glTexCoord2f(0., 1.);   glVertex3f(50, 50, -50);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
}
void floor()
{
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[4]);
    
    glNormal3f(0, 1, 0);
	glBegin(GL_QUADS);
	  glTexCoord2f(1., 0.);   glVertex3f(-50, 0, -50);
	  glTexCoord2f(1., 1.);   glVertex3f(-50, 0, 50);
	  glTexCoord2f(0., 1.);   glVertex3f(50, 0, 50);
	  glTexCoord2f(0., 0.);   glVertex3f(50, 0, -50);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
}

//-----------------------Animation Functions----------------------------
//======================================================================
//----------------------Robot Bin Movement------------------------------
void binMovement()
{
	glPushMatrix();
	  glTranslatef(bin_x, 0, bin_z);
	  glTranslatef(-10, 0, -6);
	  glRotatef(bin_direction, 0, 1, 0);
	  robotBin();
	glPopMatrix();
}

void movementPath()
{
	theta = -theta;
	if (bin_direction == 0) {
		if (bin_z < 6) {
			bin_z += 1;
		} else if (bin_x == 0) {
			bin_direction += 90;
		} else {
			bin_direction -= 90;
		}
	} else if (bin_direction == 180 || bin_direction == -180) {
		if (bin_z > 0) {
			bin_z -= 1;
		} else {
			bin_direction = 0;
		}
	} else if (bin_direction == 90) {
		if (bin_x < 20) {
			bin_x += 1;
		} else {
			bin_direction += 90;
		}
	} else {
		if (bin_x > 0) {
			bin_x -= 1;
		} else {
			bin_direction -= 90;
		}
	}
}
//----------------------Magnetic Diamond Movement-----------------------
void diamondMovement()
{
	if (is_up == 1 && diamond_height < 9) {
		diamond_height += 0.07;
	} else if (is_up == -1 && diamond_height > 2.7) {
		diamond_height -= 0.07;
	} else if (mag_counter == 50){
		is_up = -is_up;
		mag_counter = 0;
	} else {
		mag_counter += 1;
	}
}
//------------------------Planets Orbiting------------------------------
void planetsRotation()
{
	rotation_angle += 0.01;
    earth_x = 7 + 2.1 * cos(-rotation_angle);
    earth_z = 7 + 2.1 * sin(-rotation_angle);
}
//======================================================================

//-----------------------------Timer Function---------------------------

void sceneTimer(int value)
{
	bin_counter += 30;
	if (bin_counter % 180 == 0) {
		movementPath();
	}
	
	diamondMovement();
	planetsRotation();
	
    glutPostRedisplay();
    glutTimerFunc(30, sceneTimer, 0);
}


//---------------------------Keyboard Function--------------------------
void move_camera()
{
	eye_x += step * 0.1 * sin(-angle);
	eye_z += step * 0.1 * cos(-angle);
	
	if (eye_x <= -49) eye_x = -47;
	else if (eye_x >= 49) eye_x = 47;
	if (eye_z <= -49) eye_z = -47;
	else if (eye_z >= 49) eye_z = 47;
	
	look_x = eye_x + 100 * sin(angle);
	look_z = eye_z - 100 * cos(angle);
}	
void special(int key, int x, int y)
{
	step = 0;
	if(key == GLUT_KEY_LEFT) angle -= 0.1;
	else if (key == GLUT_KEY_RIGHT) angle += 0.1;
	else if(key == GLUT_KEY_DOWN) step = 1;
	else if(key == GLUT_KEY_UP) step = -1;
	flag = 1;

	glutPostRedisplay();
}
//--------------------------Display Function----------------------------

void display() 
{
	if (flag == 1) {
		move_camera();
		flag = 0;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);								
	glLoadIdentity();
	gluLookAt(eye_x, 5, eye_z,   look_x, 5, look_z,   0, 1, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
    float shadowMat[16] = {lgt0_pos[1],0,0,0, -lgt0_pos[0],0,-lgt0_pos[2],-1, 
	                      0,0,lgt0_pos[1],0, 0,0,0,lgt0_pos[1]};
    glLightfv(GL_LIGHT0,GL_POSITION, lgt0_pos);
    glLightfv(GL_LIGHT1, GL_POSITION, lgt1_pos);
    
	// Buidling up the scene
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glPushMatrix();
	  glTranslatef(0, -0.1, 0);
	  walls();
	  floor();
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);

	glEnable(GL_LIGHT1);
	mesume();
	magnet();
	binMovement();

	glDisable(GL_LIGHTING);
	glPushMatrix();                          //planar shadow of the ufo
		glMultMatrixf(shadowMat);
		glTranslatef(0, 5, -8);
		glScalef(0.5, 0.5, 0.5);
		glRotatef(rotation_angle*13, 0, 1, 0);
		glColor4f(0.2, 0.2, 0.2, 1.0);
		ufo(1);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glPushMatrix();                         //ufo
		glTranslatef(0, 5, -8);
		glScalef(0.5, 0.5, 0.5);
		glRotatef(rotation_angle*13, 0, 1, 0);
		ufo(0);
	glPopMatrix();

	glPushMatrix();
	  glRotatef(13, 0, 0, 1);
	  planets();
	glPopMatrix();
	
	glutSwapBuffers();									
}

//-------------------------Initialise Function--------------------------

void initialise()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    
	loadTexture();	
	glEnable(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);

	glClearColor(1, 1, 1, 1);
	
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat);
    glMaterialf(GL_FRONT, GL_SHININESS, 100);
    
    glEnable(GL_COLOR_MATERIAL);
    glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();
	gluPerspective(60, 1, 0.1, 300);
}

//----------------------------The Main Function-------------------------

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
   glutInitWindowSize (700, 700); 
   glutInitWindowPosition (30, 30);
   glutCreateWindow ("The Mesume");
   initialise();

   glutDisplayFunc(display); 
   glutSpecialFunc(special);
   glutTimerFunc(30, sceneTimer, 0);
   glutMainLoop();
   return 0;
}
