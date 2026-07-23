#include "objetos3d.h"
#include <GL/gl.h>


void aplicarMaterial(GLfloat ambiente[4], GLfloat difusa[4],
                      GLfloat especular[4], GLfloat brilho) {
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ambiente);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  especular);
    glMaterialf(GL_FRONT,  GL_SHININESS, brilho);
}


void desenharChao(void) {
    /* Chão/água - cor acinzentada, opaco, sem brilho especular */
    GLfloat ambChao[] = {0.15f, 0.15f, 0.17f, 1.0f};
    GLfloat difChao[] = {0.3f,  0.3f,  0.35f, 1.0f};
    GLfloat espChao[] = {0.0f,  0.0f,  0.0f,  1.0f};
    aplicarMaterial(ambChao, difChao, espChao, 0.0f);

    glPushMatrix();
        glTranslatef(0.0f, -1.0f, 0.0f);
        glScalef(20.0f, 0.1f, 20.0f);
        glutSolidCube(1.0f);
    glPopMatrix();
}


void desenharObjetos(void) {
    /* Jogador (jet ski) - plástico vermelho, brilho baixo/médio */
    GLfloat ambJogador[] = {0.2f, 0.02f, 0.02f, 1.0f};
    GLfloat difJogador[] = {0.8f, 0.1f,  0.1f,  1.0f};
    GLfloat espJogador[] = {0.3f, 0.3f,  0.3f,  1.0f};
    glPushMatrix();
        glTranslatef(-2.0f, 0.0f, 0.0f);
        aplicarMaterial(ambJogador, difJogador, espJogador, 20.0f);
        glutSolidCube(1.0f);
    glPopMatrix();

    /* Boia - plástico/borracha amarelo-laranja, brilho médio */
    GLfloat ambBoia[] = {0.25f, 0.15f, 0.01f, 1.0f};
    GLfloat difBoia[] = {0.95f, 0.6f,  0.05f, 1.0f};
    GLfloat espBoia[] = {0.4f,  0.35f, 0.2f,  1.0f};
    glPushMatrix();
        glTranslatef(2.0f, -0.5f, -2.0f);
        aplicarMaterial(ambBoia, difBoia, espBoia, 35.0f);
        glutSolidSphere(0.6, 20, 20);
    glPopMatrix();

    /* Obstáculo (rocha) - pedra fosca, quase sem brilho especular */
    GLfloat ambRocha[] = {0.1f,  0.09f, 0.08f, 1.0f};
    GLfloat difRocha[] = {0.35f, 0.32f, 0.30f, 1.0f};
    GLfloat espRocha[] = {0.05f, 0.05f, 0.05f, 1.0f};
    glPushMatrix();
        glTranslatef(1.0f, -0.6f, 2.0f);
        glRotatef(25.0f, 0.3f, 1.0f, 0.2f);
        aplicarMaterial(ambRocha, difRocha, espRocha, 3.0f);
        glutSolidCube(0.9f);
    glPopMatrix();

    /* Poste de chegada - metal dourado bem polido, brilho alto */
    GLfloat ambMetal[] = {0.25f, 0.2f, 0.05f, 1.0f};
    GLfloat difMetal[] = {0.85f, 0.7f, 0.15f, 1.0f};
    GLfloat espMetal[] = {1.0f,  0.9f, 0.6f,  1.0f};
    glPushMatrix();
        glTranslatef(-4.0f, -1.0f, -3.0f);
        aplicarMaterial(ambMetal, difMetal, espMetal, 90.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glutSolidCone(0.3, 2.0, 16, 4);
    glPopMatrix();
}