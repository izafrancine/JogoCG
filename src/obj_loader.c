#include "obj_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { float x, y, z; } Vec3;

#define LINHA_MAX 512
#define FACE_MAX_VERTICES 16

GLuint carregarOBJ(const char *caminho) {
    FILE *arq = fopen(caminho, "r");
    if (!arq) {
        fprintf(stderr, "[obj_loader] Erro: nao foi possivel abrir '%s'\n", caminho);
        return 0;
    }

    int capV = 2048, capN = 2048;
    Vec3 *vertices = (Vec3 *) malloc(sizeof(Vec3) * capV);
    Vec3 *normais  = (Vec3 *) malloc(sizeof(Vec3) * capN);
    int nV = 0, nN = 0;

    GLuint lista = glGenLists(1);
    glNewList(lista, GL_COMPILE);

    char linha[LINHA_MAX];
    while (fgets(linha, LINHA_MAX, arq)) {

        if (linha[0] == 'v' && linha[1] == ' ') {
            if (nV >= capV) { capV *= 2; vertices = (Vec3 *) realloc(vertices, sizeof(Vec3) * capV); }
            sscanf(linha + 2, "%f %f %f", &vertices[nV].x, &vertices[nV].y, &vertices[nV].z);
            nV++;

        } else if (linha[0] == 'v' && linha[1] == 'n') {
            if (nN >= capN) { capN *= 2; normais = (Vec3 *) realloc(normais, sizeof(Vec3) * capN); }
            sscanf(linha + 3, "%f %f %f", &normais[nN].x, &normais[nN].y, &normais[nN].z);
            nN++;

        } else if (linha[0] == 'f' && linha[1] == ' ') {
            int idxV[FACE_MAX_VERTICES], idxN[FACE_MAX_VERTICES];
            int total = 0;

            char *tok = strtok(linha + 2, " \t\r\n");
            while (tok && total < FACE_MAX_VERTICES) {
                int vi = 0, ti = 0, ni = 0;
                if (sscanf(tok, "%d/%d/%d", &vi, &ti, &ni) == 3) {
                    idxV[total] = vi; idxN[total] = ni;
                } else if (sscanf(tok, "%d//%d", &vi, &ni) == 2) {
                    idxV[total] = vi; idxN[total] = ni;
                } else if (sscanf(tok, "%d/%d", &vi, &ti) == 2) {
                    idxV[total] = vi; idxN[total] = 0;
                } else if (sscanf(tok, "%d", &vi) == 1) {
                    idxV[total] = vi; idxN[total] = 0;
                }
                total++;
                tok = strtok(NULL, " \t\r\n");
            }

            /* Triangulacao em leque: funciona para faces triangulares e
               quadrangulares (a maioria dos exportadores gera uma dessas duas) */
            glBegin(GL_TRIANGLES);
            for (int i = 1; i < total - 1; i++) {
                int seq[3] = {0, i, i + 1};
                for (int k = 0; k < 3; k++) {
                    int vidx = idxV[seq[k]];
                    int nidx = idxN[seq[k]];
                    if (vidx < 0) vidx = nV + vidx + 1;   /* indices negativos = relativos ao final */
                    if (nidx != 0) {
                        int realN = (nidx < 0) ? (nN + nidx + 1) : nidx;
                        glNormal3f(normais[realN - 1].x, normais[realN - 1].y, normais[realN - 1].z);
                    }
                    glVertex3f(vertices[vidx - 1].x, vertices[vidx - 1].y, vertices[vidx - 1].z);
                }
            }
            glEnd();
        }
        /* linhas comecando com 'o', 'g', 'usemtl', 'mtllib', 'vt', '#' sao ignoradas */
    }

    glEndList();
    fclose(arq);
    free(vertices);
    free(normais);

    printf("[obj_loader] '%s' carregado: %d vertices, %d normais\n", caminho, nV, nN);
    return lista;
}