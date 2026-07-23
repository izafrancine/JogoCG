/*
 * Jogo 3D: Aqua Thrills (Esqueleto)
 *  -> Objetos 3D com cores/materiais distintos (ver objetos3d.c/.h)
 *  -> Iluminação ambiente, difusa e especular
 *  -> Câmera navegável (WASD + setas)
 *  -> Projeção perspectiva
 *  -> Remoção de superfícies ocultas (depth test)
 */

#include <GL/freeglut.h>
#include <math.h>
#include "objetos3d.h"


float camX = 0.0f, camY = 3.0f, camZ = 8.0f; 
float camAngle = 0.0f;                        
float camSpeed = 0.2f;


int winWidth = 800, winHeight = 600;


void init(void) {
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST); 

  
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // A cor de cada objeto vem do seu material (glMaterialfv em objetos3d.c), então GL_COLOR_MATERIAL não é usado aqui 
    GLfloat luzAmbiente[]  = {0.25f, 0.25f, 0.25f, 1.0f};
    GLfloat luzDifusa[]    = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat luzEspecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat posicaoLuz[]   = {5.0f, 8.0f, 5.0f, 1.0f}; 

    glLightfv(GL_LIGHT0, GL_AMBIENT,  luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

  
    GLfloat luzAmbienteGlobal[] = {0.15f, 0.15f, 0.15f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbienteGlobal);

    glShadeModel(GL_SMOOTH);
}


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

   
    float lookX = camX + sinf(camAngle);
    float lookZ = camZ - cosf(camAngle);
    gluLookAt(camX, camY, camZ,  
              lookX, camY, lookZ, 
              0.0f, 1.0f, 0.0f);  

    desenharChao();
    desenharObjetos();

    glutSwapBuffers();
}


void reshape(int w, int h) {
    if (h == 0) h = 1;
    winWidth = w; winHeight = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w / (double)h, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}


void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': camX += camSpeed * sinf(camAngle); camZ -= camSpeed * cosf(camAngle); break;
        case 's': camX -= camSpeed * sinf(camAngle); camZ += camSpeed * cosf(camAngle); break;
        case 'a': camAngle -= 0.05f; break;
        case 'd': camAngle += 0.05f; break;
        case 27: exit(0); break; // ESC sai 
    }
    glutPostRedisplay();
}


void teclasEspeciais(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:    camY += camSpeed; break;
        case GLUT_KEY_DOWN:  camY -= camSpeed; break;
        case GLUT_KEY_LEFT:  camAngle -= 0.05f; break;
        case GLUT_KEY_RIGHT: camAngle += 0.05f; break;
    }
    glutPostRedisplay();
}


void idle(void) {
    glutPostRedisplay();
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 50);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Jogo 3D - Aqua Thrills - Computacao Grafica UFCA");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(teclasEspeciais);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}