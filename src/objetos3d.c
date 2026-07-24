#include "objetos3d.h"
#include <GL/gl.h>
#include "logica.h"
#define PI 3.14159265358979323846f

GLuint listaBarco = 0;
GLuint listaMoeda=0;
GLuint listasPedra[4]={0,0,0,0};

extern EstadoDoJogo jogo;

void aplicarMaterial(GLfloat ambiente[4], GLfloat difusa[4],
                      GLfloat especular[4], GLfloat brilho) {
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ambiente);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  especular);
    glMaterialf(GL_FRONT,  GL_SHININESS, brilho);
}


void desenharChao(void) {
    GLfloat ambChao[] = {0.05f, 0.12f, 0.18f, 1.0f};
    GLfloat difChao[] = {0.1f,  0.35f, 0.55f, 1.0f};
    GLfloat espChao[] = {0.9f,  0.9f,  0.9f,  1.0f};
    aplicarMaterial(ambChao, difChao, espChao, 100.0f);
 
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
        glRotatef(-jogo.jogador.angulo * 180.0f / PI, 0.0f, 1.0f, 0.0f); //barco roda com a camera
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f); //coloca frente que estava invertida
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
            glCallList(listaMoeda);
        glPopMatrix();
    }

    /* Obstáculo (rocha) - pedra fosca, quase sem brilho especular */
    GLfloat ambRocha[] = {0.1f,  0.09f, 0.08f, 1.0f};
    GLfloat difRocha[] = {0.35f, 0.32f, 0.30f, 1.0f};
    GLfloat espRocha[] = {0.05f, 0.05f, 0.05f, 1.0f};
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        glPushMatrix();
            glTranslatef(jogo.obstaculos[i].x, jogo.obstaculos[i].y, jogo.obstaculos[i].z);
            aplicarMaterial(ambRocha, difRocha, espRocha, 3.0f);
            glCallList(listasPedra[jogo.obstaculos[i].tipo]);  
        glPopMatrix();
    }
}