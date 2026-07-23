#include "objetos3d.h"
#include <GL/gl.h>
#include "logica.h"
#define PI 3.14159265358979323846f

GLuint listaBarco = 0;
extern EstadoDoJogo jogo;

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
        glScalef(80.0f, 0.1f, 80.0f);
        glutSolidCube(1.0f);
    glPopMatrix();
}


void desenharObjetos(void) {
    // desenha jogador
    GLfloat ambJogador[] = {0.25f, 0.15f, 0.07f, 1.0f};
    GLfloat difJogador[] = {0.55f, 0.35f, 0.18f, 1.0f};
    GLfloat espJogador[] = {0.08f, 0.08f, 0.08f, 1.0f};
    glPushMatrix();
        glTranslatef(jogo.jogador.x, jogo.jogador.y, jogo.jogador.z);
        glRotatef(jogo.jogador.angulo * 180.0f / PI, 0.0f, 1.0f, 0.0f);
        glScalef(0.25f, 0.25f, 0.25f); 
        aplicarMaterial(ambJogador, difJogador, espJogador, 20.0f);
        glCallList(listaBarco);       
 
    glPopMatrix();

    // moeda (substitui boias) 
    GLfloat ambMoedas[] = {0.25f, 0.15f, 0.01f, 1.0f};
    GLfloat difMoedas[] = {0.95f, 0.6f,  0.05f, 1.0f};
    GLfloat espMoedas[] = {0.4f,  0.35f, 0.2f,  1.0f};
    for (int i = 0; i < MAX_MOEDAS; i++) {
        if (!jogo.moedas[i].ativo) continue;
        glPushMatrix();
            glTranslatef(jogo.moedas[i].x, jogo.moedas[i].y, jogo.moedas[i].z);
            aplicarMaterial(ambMoedas, difMoedas, espMoedas, 35.0f);
            glutSolidSphere(jogo.moedas[i].raio, 20, 20);
        glPopMatrix();
    }

    /* Obstáculo (rocha) - pedra fosca, quase sem brilho especular */
    GLfloat ambRocha[] = {0.1f,  0.09f, 0.08f, 1.0f};
    GLfloat difRocha[] = {0.35f, 0.32f, 0.30f, 1.0f};
    GLfloat espRocha[] = {0.05f, 0.05f, 0.05f, 1.0f};
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        glPushMatrix();
            glTranslatef(jogo.obstaculos[i].x, jogo.obstaculos[i].y, jogo.obstaculos[i].z);
            glRotatef(25.0f, 0.3f, 1.0f, 0.2f);
            aplicarMaterial(ambRocha, difRocha, espRocha, 3.0f);
            glutSolidCube(jogo.obstaculos[i].raio * 1.3f);
        glPopMatrix();
    }
}