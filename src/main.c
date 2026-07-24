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


void init(void) {
    glClearColor(0.74f, 0.87f, 0.95f, 1.0f);
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND); //para tranpareencia da agua
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // A cor de cada objeto vem do seu material (glMaterialfv em objetos3d.c), então GL_COLOR_MATERIAL não é usado aqui 
    GLfloat luzDifusa[]    = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat luzEspecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat posicaoLuz[]   = {5.0f, 8.0f, 5.0f, 1.0f}; 

    glLightfv(GL_LIGHT0, GL_DIFFUSE,  luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

    //teste, luz do sol:
    GLfloat posSol[] = {0.3f, 0.9f, 0.2f, 0.0f};   // w=0.0 -> direção, não posição
    GLfloat corSol[] = {1.0f, 0.95f, 0.85f, 1.0f}; // luz branca/amarelada

    glLightfv(GL_LIGHT1, GL_POSITION, posSol);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, corSol);
    glLightfv(GL_LIGHT1, GL_SPECULAR, corSol);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

  
    GLfloat luzAmbienteGlobal[] = {0.15f, 0.15f, 0.15f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbienteGlobal);

    glShadeModel(GL_SMOOTH);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    inicializarJogo();
    ultimoTempo = glutGet(GLUT_ELAPSED_TIME);

    listaMoeda = carregarOBJ("moedas.obj");
    listaCenario = carregarOBJ("cenario.obj");
    listasPedra[0] = carregarOBJ("pedra1.obj");
    listasPedra[1] = carregarOBJ("pedra2.obj");
    listasPedra[2] = carregarOBJ("pedra3.obj");
    listasPedra[3] = carregarOBJ("pedra4.obj");
    listaBarco = carregarOBJ("barco.obj");
    texturaBarco = carregarTextura("barco_textura.png");
    glEnable(GL_TEXTURE_2D);
}

// texto 2D simples na tela 
void desenharTexto(float x, float y, const char *texto) {
    glRasterPos2f(x, y);
    for (const char *c = texto; *c; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
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

    char buffer[128];
    glColor3f(1.0f, 1.0f, 1.0f);

    float tempoRestante = TEMPO_LIMITE - jogo.tempoDecorrido;
    if (tempoRestante < 0.0f) tempoRestante = 0.0f;

    snprintf(buffer, sizeof(buffer), "Moedas: %d/%d   Tempo restante: %.1fs",jogo.moedasColetadas, MAX_MOEDAS, tempoRestante);
    desenharTexto(20, winHeight - 30, buffer);

    if (jogo.estado == VITORIA) {
        glColor3f(0.2f, 1.0f, 0.2f);
        desenharTexto(winWidth / 2 - 70, winHeight / 2, "VOCE VENCEU!");
    } else if (jogo.estado == DERROTA) {
        glColor3f(1.0f, 0.2f, 0.2f);
        desenharTexto(winWidth / 2 - 90, winHeight / 2, "TEMPO ESGOTADO!");
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