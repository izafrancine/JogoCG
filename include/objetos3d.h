#ifndef OBJETOS3D_H
#define OBJETOS3D_H

#include <GL/freeglut.h>

void aplicarMaterial(GLfloat ambiente[4], GLfloat difusa[4], GLfloat especular[4], GLfloat brilho);
void desenharChao(void);
void desenharCenario();
void desenharObjetos(void);
void aplicarSombra(float alturaChao, float lx, float ly, float lz);

#endif