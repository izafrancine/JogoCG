#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include "objetos3d.h"
#include "logica.h"
#include "obj_loader.h"
#define PI 3.14159265358979323846f

float camDistancia = 6.0f; //camera segue o jogador em 3a pessoa
float camAltura = 3.0f;


int winWidth = 800, winHeight = 600;

extern EstadoDoJogo jogo;
int teclaW = 0, teclaS = 0, teclaA = 0, teclaD = 0;
int ultimoTempo = 0;

//importacao de objeto que sao definidos em objetos#d.c
extern GLuint listaBarco;
extern GLuint listaMoeda;
extern GLuint listaCenario;
extern GLuint listasPedra[4];
extern GLuint texturaBarco;
extern GLuint listaPersonagem;
extern GLuint texturaPersonagem;
extern GLuint listaTronco;
extern GLuint listaFolhagem;


void init(void) {
    glClearColor(0.74f, 0.87f, 0.95f, 1.0f);
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND); //para tranpareencia da agua
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //teste, luz do sol:
    GLfloat posSol[] = {0.3f, 0.9f, 0.2f, 0.0f};   // w=0.0 -> direção, não posição
    GLfloat corSol[] = {1.0f, 0.95f, 0.85f, 1.0f}; // luz branca/amarelada

    glLightfv(GL_LIGHT1, GL_POSITION, posSol);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, corSol);
    glLightfv(GL_LIGHT1, GL_SPECULAR, corSol);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);

  
    GLfloat luzAmbienteGlobal[] = {0.5f, 0.5f, 0.55f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbienteGlobal);

    glShadeModel(GL_SMOOTH);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    inicializarJogo();
    ultimoTempo = glutGet(GLUT_ELAPSED_TIME);

    listaMoeda = carregarOBJ("moedas.obj");
    listaCenario = carregarOBJ("cenario.obj");
    listaFolhagem = carregarOBJ ("folhagem.obj");
    listaTronco = carregarOBJ ("troncos.obj");
    listasPedra[0] = carregarOBJ("pedra1.obj");
    listasPedra[1] = carregarOBJ("pedra2.obj");
    listasPedra[2] = carregarOBJ("pedra3.obj");
    listasPedra[3] = carregarOBJ("pedra4.obj");
    listaBarco = carregarOBJ("barco.obj");
    texturaBarco = carregarTextura("barco_textura.png");
    listaPersonagem = carregarOBJ("personagem.obj");
    texturaPersonagem = carregarTextura("colormap.png");
    
}

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
void desenharPainelArredondado(float x, float y, float largura, float altura,
                                float r, float g, float b, float a) {
    float raio = altura / 2.0f;
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


void desenharPainelHUD(float x, float y, float largura, float altura, const char *texto,
                        float corTxtR, float corTxtG, float corTxtB) {

    desenharPainelArredondado(x + 3.0f, y - 3.0f, largura, altura, 0.0f, 0.0f, 0.0f, 0.25f);
    desenharPainelArredondado(x - 2.0f, y - 2.0f, largura + 4.0f, altura + 4.0f,
                               0.70f, 0.50f, 0.05f, 1.0f);
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

    /* obs: o blend ja foi habilitado em init() (usado pela agua), entao os
       paineis com alpha < 1.0 (sombra) ja aparecem translucidos sem
       precisar mexer no estado do GL_BLEND aqui */

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
    desenharPainelHUD(winWidth - largTempo - margem, yPainel, largTempo, altura, bufTempo,
                       0.05f, 0.20f, 0.45f);

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

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float camX = jogo.jogador.x - sinf(jogo.jogador.angulo) * camDistancia;
    float camZ = jogo.jogador.z + cosf(jogo.jogador.angulo) * camDistancia;
    float camY = jogo.jogador.y + camAltura;
    gluLookAt(camX, camY, camZ, jogo.jogador.x, jogo.jogador.y + 0.5f, jogo.jogador.z, 0.0f, 1.0f, 0.0f);   
    desenharCenario();
    desenharObjetos();
    desenharHUD();
    desenharChao();
    glutSwapBuffers();
}


void reshape(int w, int h) {
    if (h == 0) h = 1;
    winWidth = w; winHeight = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w / (double)h, 0.1, 200.0);

    glMatrixMode(GL_MODELVIEW);
}


void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': teclaW = 1; break;
        case 's': teclaS = 1; break;
        case 'a': teclaA = 1; break;
        case 'd': teclaD = 1; break;
        case 27: exit(0); break; 
    }
}
void tecladoSolto(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': teclaW = 0; break;
        case 's': teclaS = 0; break;
        case 'a': teclaA = 0; break;
        case 'd': teclaD = 0; break;
    }
}


void idle(void) {
    int tempoAtual = glutGet(GLUT_ELAPSED_TIME);
    float dt = (tempoAtual - ultimoTempo) / 1000.0f; // ms -> s 
    ultimoTempo = tempoAtual;
    if (dt > 0.1f) dt = 0.1f; // evita "saltos" grandes se a janela travar 

    float direcao = 0.0f;
    if (teclaA) direcao -= 1.0f;
    if (teclaD) direcao += 1.0f;

    atualizarJogo(dt, teclaW, teclaS, direcao);

    glutPostRedisplay();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 50);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Jogo 3D");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(teclado);
    glutKeyboardUpFunc(tecladoSolto);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}