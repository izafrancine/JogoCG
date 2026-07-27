#include "objetos3d.h"
#include <GL/gl.h>
#include "logica.h"
#define PI 3.14159265358979323846f

GLuint listaBarco = 0;
GLuint listaMoeda=0;
GLuint listaCenario = 0;
GLuint texturaCenario = 0;
GLuint listasPedra[4]={0,0,0,0};
GLuint texturaBarco = 0;
GLuint listaPersonagem = 0;
GLuint texturaPersonagem = 0;
GLuint listaTronco = 0;
GLuint listaFolhagem = 0;

extern EstadoDoJogo jogo;

void aplicarMaterial(GLfloat ambiente[4], GLfloat difusa[4],
                      GLfloat especular[4], GLfloat brilho) {
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ambiente);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   difusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  especular);
    glMaterialf(GL_FRONT,  GL_SHININESS, brilho);
}

void aplicarSombra(float alturaChao, float lx, float ly, float lz) {
    float d = -alturaChao;
    float dot = ly;
    GLfloat m[16];
    m[0]  = dot;         m[4]  = -lx;   m[8]  = 0.0f;  m[12] = lx * alturaChao;
    m[1]  = 0.0f;        m[5]  = 0.0f;  m[9]  = 0.0f;  m[13] = ly * alturaChao;
    m[2]  = 0.0f;        m[6]  = -lz;   m[10] = dot;   m[14] = lz * alturaChao;
    m[3]  = 0.0f;        m[7]  = 0.0f;  m[11] = 0.0f;  m[15] = dot;

    glMultMatrixf(m);
}


void desenharChao(void) {
    GLfloat ambChao[] = {0.05f, 0.10f, 0.15f, 1.0f};
    GLfloat difChao[] = {0.1f,  0.35f, 0.55f, 0.65f};
    GLfloat espChao[] = {0.9f,  0.9f,  0.9f,  1.0f};
    aplicarMaterial(ambChao, difChao, espChao, 100.0f);
 
    glPushMatrix();
        glTranslatef(0.0f, -1.0f, 0.0f);
        glScalef(80.0f, 0.1f, 100.0f);
        glutSolidCube(1.0f);
    glPopMatrix();
}

void desenharCenario(){
    //piso
    GLfloat ambGrama[] = {0.03f, 0.08f, 0.02f, 1.0f};
    GLfloat difGrama[] = {0.10f, 0.32f, 0.08f, 1.0f};
    GLfloat espGrama[] = {0.04f, 0.04f, 0.04f, 1.0f};
    aplicarMaterial(ambGrama, difGrama, espGrama, 8.0f);

    glPushMatrix();
        glTranslatef(0.0f, 1.22f, 0.0f);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturaCenario);
        glCallList(listaCenario);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //folhas
    GLfloat ambFolhas[] = {0.22f, 0.32f, 0.24f, 1.0f};
    GLfloat difFolhas[] = {0.15f, 0.59f,0.75f, 1.0f};
    GLfloat espFolhas[] = {0.01f, 0.03f, 0.01f, 1.0f};
    aplicarMaterial(ambFolhas, difFolhas, espFolhas, 8.0f);
    glPushMatrix();
        glTranslatef(0, -1, 0);
        glCallList(listaFolhagem);
    glPopMatrix();

    //troncos
    GLfloat ambTronco[] = {0.10f, 0.04f, 0.015f, 1.0f};
    GLfloat difTronco[] = {0.45f, 0.18f, 0.05f, 1.0f};
    GLfloat espTronco[] = {0.02f, 0.015f, 0.01f, 1.0f};
    aplicarMaterial(ambTronco, difTronco, espTronco, 4.0f);
    glPushMatrix();
        glTranslatef(0, -1, 0);
        glCallList(listaTronco);
    glPopMatrix();
}


void desenharObjetos(void) {
    // desenha jogador
    GLfloat ambJogador[] = {0.35f, 0.35f, 0.35f, 1.0f};
    GLfloat difJogador[] = {1.0f, 1.0f, 1.0f, 1.0f}; //branco por causa da textura
    GLfloat espJogador[] = {0.5f, 0.5f, 0.5f, 1.0f};
    glPushMatrix();
        glTranslatef(jogo.jogador.x, jogo.jogador.y, jogo.jogador.z);
        glRotatef(-jogo.jogador.angulo * 180.0f / PI, 0.0f, 1.0f, 0.0f); //barco roda com a camera
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f); //coloca frente que estava invertida
        glScalef(0.25f, 0.25f, 0.25f); 
        aplicarMaterial(ambJogador, difJogador, espJogador, 20.0f);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturaBarco);
        glCallList(listaBarco);
        glBindTexture(GL_TEXTURE_2D, 0);      
        glDisable(GL_TEXTURE_2D);

        // personagem em cima do barco:
        glPushMatrix();
            glTranslatef(0.0f, 1.5f, -0.3f); 
            glScalef(4.00f, 4.00f, 4.00f);
            GLfloat ambPers[] = {1.0f, 1.0f, 1.0f, 1.0f};
            GLfloat difPers[] = {1.0f, 1.0f, 1.0f, 1.0f};
            GLfloat espPers[] = {0.3f, 0.3f, 0.3f, 1.0f};
            aplicarMaterial(ambPers, difPers, espPers, 15.0f);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texturaPersonagem);
            glCallList(listaPersonagem);
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    glPopMatrix();

    // sombra do barco
    glPushMatrix();
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glDepthMask(GL_FALSE);
        glColor4f(0.0f, 0.0f, 0.0f, 0.35f);
        aplicarSombra(-0.94f, 0.3f, 0.9f, 0.2f); // mesmos valores de posSol[] no main.c
        glTranslatef(jogo.jogador.x, jogo.jogador.y, jogo.jogador.z);
        glRotatef(-jogo.jogador.angulo * 180.0f / PI, 0.0f, 1.0f, 0.0f);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glScalef(0.25f, 0.25f, 0.25f);
        glCallList(listaBarco);
        glDepthMask(GL_TRUE);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
    glPopMatrix();


    // moeda
    GLfloat ambMoedas[] = {0.30f, 0.20f, 0.01f, 1.0f};
    GLfloat difMoedas[] = {0.90f, 0.55f,  0.1f, 1.0f};
    GLfloat espMoedas[] = {1.f, 0.95f, 0.80f, 1.0f};
    for (int i = 0; i < MAX_MOEDAS; i++) {
        if (!jogo.moedas[i].ativo) continue;
        glPushMatrix();
            glTranslatef(jogo.moedas[i].x, jogo.moedas[i].y, jogo.moedas[i].z);
            aplicarMaterial(ambMoedas, difMoedas, espMoedas, 128.f);
            glCallList(listaMoeda);
        glPopMatrix();

         // sombra moeda
        glPushMatrix();
            glDisable(GL_LIGHTING);
            glDepthMask(GL_FALSE);
            glColor4f(0.0f, 0.0f, 0.0f, 0.3f);
            aplicarSombra(-0.94f, 0.3f, 0.9f, 0.2f);
            glTranslatef(jogo.moedas[i].x, jogo.moedas[i].y, jogo.moedas[i].z);
            glCallList(listaMoeda);
            glDepthMask(GL_TRUE);
            glEnable(GL_LIGHTING);
        glPopMatrix();
    }

    
    // obstáculo (rocha)fosca, quase sem brilho especular 
    GLfloat ambRocha[] = {0.19f, 0.17f, 0.14f, 1.0f};
    GLfloat difRocha[] = {0.35f, 0.32f, 0.30f, 1.0f};
    GLfloat espRocha[] = {0.05f, 0.05f, 0.05f, 1.0f};
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        glPushMatrix();
            glTranslatef(jogo.obstaculos[i].x, jogo.obstaculos[i].y, jogo.obstaculos[i].z);
            glScalef(1.5, 1.80, 1.5); 
            aplicarMaterial(ambRocha, difRocha, espRocha, 3.0f);
            glCallList(listasPedra[jogo.obstaculos[i].tipo]);  
        glPopMatrix();

        // sombra da pedra
        glPushMatrix();
            glDisable(GL_LIGHTING);
            glDepthMask(GL_FALSE);
            glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
            aplicarSombra(-0.94f, 0.3f, 0.9f, 0.2f);
            glTranslatef(jogo.obstaculos[i].x, jogo.obstaculos[i].y, jogo.obstaculos[i].z);
            glScalef(1.5f, 1.80f, 1.5f);
            glCallList(listasPedra[jogo.obstaculos[i].tipo]);
            glDepthMask(GL_TRUE);
            glEnable(GL_LIGHTING);
        glPopMatrix();
    }
}