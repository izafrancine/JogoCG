#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include "logica.h"
#include "hud.h"

extern int winWidth;
extern int winHeight;
extern EstadoDoJogo jogo;

// texto 2D simples na tela
void desenharTexto(float x, float y, const char *texto) {
    glRasterPos2f(x, y);
    for (const char *c = texto; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}
// soma a largura que o texto vai ocupar com a fonte
int larguraTexto(const char *texto) {
    int total = 0;
    for (const char *c = texto; *c; c++) {
        total += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c);
    }
    return total;
}
// desenha um painel genérico
void desenharPainelArredondado(float x, float y, float largura, float altura, float r, float g, float b, float a) {
    float raio = altura / 2.0f;
 
    if (raio > largura / 2.0f) {
        raio = largura / 2.0f;
    }
 
    float cxEsq = x + raio;
    float cxDir = x + largura - raio;
    float cy = y + raio;
    int segmentos = 20;
    int i;
 
    glColor4f(r, g, b, a);
 
    glBegin(GL_QUADS);
        glVertex2f(cxEsq, y);
        glVertex2f(cxDir, y);
        glVertex2f(cxDir, y + altura);
        glVertex2f(cxEsq, y + altura);
    glEnd();
 
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cxEsq, cy);
        for (i = 0; i <= segmentos; i++) {
            float ang = (PI / 2.0f) + (float)i / segmentos * PI;
            glVertex2f(cxEsq + cosf(ang) * raio, cy + sinf(ang) * raio);
        }
    glEnd();
 
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cxDir, cy);
        for (i = 0; i <= segmentos; i++) {
            float ang = (-PI / 2.0f) + (float)i / segmentos * PI;
            glVertex2f(cxDir + cosf(ang) * raio, cy + sinf(ang) * raio);
        }
    glEnd();
}
void desenharPainelHUD(float x, float y, float largura, float altura, const char *texto, float corTxtR, float corTxtG, float corTxtB) {

    desenharPainelArredondado(x + 3.0f, y - 3.0f, largura, altura, 0.0f, 0.0f, 0.0f, 0.25f);
    desenharPainelArredondado(x - 2.0f, y - 2.0f, largura + 4.0f, altura + 4.0f,0.70f, 0.50f, 0.05f, 1.0f);
    desenharPainelArredondado(x, y, largura, altura, 0.98f, 0.78f, 0.18f, 1.0f);

    int larguraTxt = larguraTexto(texto);
    float tx = x + (largura - larguraTxt) / 2.0f;
    float ty = y + altura / 2.0f - 6.0f;
    glColor3f(corTxtR, corTxtG, corTxtB);
    desenharTexto(tx, ty, texto);
}
void desenharPainelMensagem(const char *texto, float corTxtR, float corTxtG, float corTxtB) {
    float paddingH = 40.0f;
    float altura = 56.0f;
    float largura = larguraTexto(texto) + paddingH;
    float x = winWidth / 2.0f - largura / 2.0f;
    float y = winHeight / 2.0f - altura / 2.0f;
    desenharPainelHUD(x, y, largura, altura, texto, corTxtR, corTxtG, corTxtB);
}
// desenha o texto 2d na tela
void desenharHUD(void) {
    glDisable(GL_LIGHTING); //texto nao sofre efeitos da iluminacao
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, winWidth, 0, winHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    char bufMoedas[64];
    char bufTempo[64];

    float tempoRestante = TEMPO_LIMITE - jogo.tempoDecorrido;
    if (tempoRestante < 0.0f) tempoRestante = 0.0f;

    snprintf(bufMoedas, sizeof(bufMoedas), "MOEDAS: %d/%d", jogo.moedasColetadas, MAX_MOEDAS);
    snprintf(bufTempo, sizeof(bufTempo), "TEMPO: %.1fs", tempoRestante);

    float altura = 42.0f;
    float margem = 18.0f;
    float yPainel = winHeight - altura - margem;
    float largMoedas = 170.0f;
    float largTempo = 170.0f;

    // painel de moedas, no canto superior esquerdo
    desenharPainelHUD(margem, yPainel, largMoedas, altura, bufMoedas, 0.05f, 0.20f, 0.45f);

    // painel de tempo, no canto superior direito 
    desenharPainelHUD(winWidth - largTempo - margem, yPainel, largTempo, altura, bufTempo,0.05f, 0.20f, 0.45f);

    if (jogo.estado == VITORIA) {
        desenharPainelMensagem("VOCE VENCEU!", 0.10f, 0.45f, 0.10f);
    } else if (jogo.estado == DERROTA) {
        desenharPainelMensagem("TEMPO ESGOTADO!", 0.80f, 0.05f, 0.05f);
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST); // ativa novamente o que foi desativado
    glEnable(GL_LIGHTING);
}