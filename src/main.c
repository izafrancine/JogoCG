#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include "objetos3d.h"
#include "logica.h"
#include "obj_loader.h"
#include "hud.h"

float camDistancia = 6.0f; //camera segue o jogador em 3a pessoa
float camAltura = 3.0f;

int winWidth = 800, winHeight = 600;

extern EstadoDoJogo jogo;
int teclaW = 0, teclaS = 0, teclaA = 0, teclaD = 0;
int ultimoTempo = 0;

//importacao de objeto que sao definidos em objetos3d.c
extern GLuint listaBarco;
extern GLuint listaMoeda;
extern GLuint listaCenario;
extern GLuint texturaCenario;
extern GLuint listasPedra[4];  
extern GLuint texturaBarco;
extern GLuint listaPersonagem;
extern GLuint texturaPersonagem;
extern GLuint listaTronco;
extern GLuint listaFolhagem;

void init() {
    glClearColor(0.74f, 0.87f, 0.95f, 1.0f); //ceu
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    glEnable(GL_BLEND); //para transparencia da agua
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glShadeModel(GL_SMOOTH);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    // luz do sol infinita (w=0) 
    GLfloat posSol[] = {0.3f, 0.9f, 0.2f, 0.0f};  //vetor de direção 
    GLfloat corSol[] = {1.0f, 0.95f, 0.85f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, posSol);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, corSol);
    glLightfv(GL_LIGHT0, GL_SPECULAR, corSol);

    glEnable(GL_LIGHT0);

    //luz do ceu (w=0) 
    GLfloat posPreenchimento[] = {-0.3f, 0.5f, -0.2f, 0.0f}; // oposta ao sol
    GLfloat corPreenchimento[] = {0.2f, 0.25f, 0.53f, 1.0f}; // azulada 

    glLightfv(GL_LIGHT1, GL_POSITION, posPreenchimento);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, corPreenchimento);
    glEnable(GL_LIGHT1);

    // luz do barco (w=1) 
    GLfloat corLuzBarco[] = {1.0f, 0.9f, 0.6f, 1.0f};  // tom amarelado
    glLightfv(GL_LIGHT2, GL_DIFFUSE, corLuzBarco);
    glLightfv(GL_LIGHT2, GL_SPECULAR, corLuzBarco);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.80f);   // atenuação
    glEnable(GL_LIGHT2);

    //iluminação global da cena
    GLfloat luzAmbienteGlobal[] = {0.5f, 0.5f, 0.55f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbienteGlobal);


    inicializarJogo();
    ultimoTempo = glutGet(GLUT_ELAPSED_TIME);

    listaMoeda = carregarOBJ("assets/moedas.obj");
    listaCenario = carregarOBJ("assets/cenario.obj");
    texturaCenario = carregarTextura("assets/grama_textura.png"); 
    listaFolhagem = carregarOBJ ("assets/folhagem.obj");
    listaTronco = carregarOBJ ("assets/troncos.obj");
    listasPedra[0] = carregarOBJ("assets/pedra1.obj");
    listasPedra[1] = carregarOBJ("assets/pedra2.obj");
    listasPedra[2] = carregarOBJ("assets/pedra3.obj");
    listasPedra[3] = carregarOBJ("assets/pedra4.obj");
    listaBarco = carregarOBJ("assets/barco.obj");
    texturaBarco = carregarTextura("assets/barco_textura.png");
    listaPersonagem = carregarOBJ("assets/personagem.obj");
    texturaPersonagem = carregarTextura("assets/colormap.png");
    
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float camX = jogo.jogador.x - sinf(jogo.jogador.angulo) * camDistancia;
    float camZ = jogo.jogador.z + cosf(jogo.jogador.angulo) * camDistancia;
    float camY = jogo.jogador.y + camAltura;
    gluLookAt(camX, camY, camZ, jogo.jogador.x, jogo.jogador.y + 0.5f, jogo.jogador.z, 0.0f, 1.0f, 0.0f);
    
    GLfloat posLuzBarco[] = {jogo.jogador.x, jogo.jogador.y + 1, jogo.jogador.z, 1.0f};
    glLightfv(GL_LIGHT2, GL_POSITION, posLuzBarco);

    desenharCenario();
    desenharObjetos();
    desenharChao();
    desenharHUD();
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


void idle() {
    int tempoAtual = glutGet(GLUT_ELAPSED_TIME);
    float dt = (tempoAtual - ultimoTempo) / 1000.0f; // ms para s 
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