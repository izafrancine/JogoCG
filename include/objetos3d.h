#ifndef OBJETOS3D_H
#define OBJETOS3D_H

#include <GL/freeglut.h>

/*
 * Aplica um material completo à superfície do próximo objeto a ser
 * desenhado. A "cor" do objeto vem das componentes ambiente e difusa
 * do material (não de glColor); a componente especular e o expoente
 * de brilho (shininess) definem o quão polida/fosca é a superfície
 */
void aplicarMaterial(GLfloat ambiente[4], GLfloat difusa[4],
                      GLfloat especular[4], GLfloat brilho);

/* Desenha o chão/água da cena */
void desenharChao(void);

/*
 * Desenha os objetos 3D da cena "Aqua Thrills"
 * Cada objeto tem um material distinto (cor e propriedades ópticas
 * diferentes: plástico, borracha, pedra fosca e metal polido)
 */
void desenharObjetos(void);

#endif