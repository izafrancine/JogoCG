#include "obj_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glu.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct { float x, y, z; } Vec3;
typedef struct { float u, v; } Vec2;

#define LINHA_MAX 512
#define FACE_MAX_VERTICES 16

GLuint carregarOBJ(const char *caminho) {
    FILE *arq = fopen(caminho, "r");
    if (!arq) {
        fprintf(stderr, "[obj_loader] Erro: nao foi possivel abrir '%s'\n", caminho);
        return 0;
    }

    int capV = 2048, capN = 2048, capT = 2048;
    Vec3 *vertices = (Vec3 *) malloc(sizeof(Vec3) * capV);
    Vec3 *normais  = (Vec3 *) malloc(sizeof(Vec3) * capN);
    Vec2 *texcoord = (Vec2 *) malloc(sizeof(Vec2) * capT);
    int nV = 0, nN = 0, nT = 0;

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

        } else if (linha[0] == 'v' && linha[1] == 't') {
            if (nT >= capT) { capT *= 2; texcoord = (Vec2 *) realloc(texcoord, sizeof(Vec2) * capT); }
            sscanf(linha + 3, "%f %f", &texcoord[nT].u, &texcoord[nT].v);
            nT++;

        } else if (linha[0] == 'f' && linha[1] == ' ') {
            int idxV[FACE_MAX_VERTICES], idxN[FACE_MAX_VERTICES], idxT[FACE_MAX_VERTICES];
            int total = 0;

            char *tok = strtok(linha + 2, " \t\r\n");
            while (tok && total < FACE_MAX_VERTICES) {
                int vi = 0, ti = 0, ni = 0;
                if (sscanf(tok, "%d/%d/%d", &vi, &ti, &ni) == 3) {
                    idxV[total] = vi; idxT[total] = ti; idxN[total] = ni;
                } else if (sscanf(tok, "%d//%d", &vi, &ni) == 2) {
                    idxV[total] = vi; idxT[total] = 0;  idxN[total] = ni;
                } else if (sscanf(tok, "%d/%d", &vi, &ti) == 2) {
                    idxV[total] = vi; idxT[total] = ti; idxN[total] = 0;
                } else if (sscanf(tok, "%d", &vi) == 1) {
                    idxV[total] = vi; idxT[total] = 0;  idxN[total] = 0;
                }
                total++;
                tok = strtok(NULL, " \t\r\n");
            }

            /* Triangulacao em leque */
            glBegin(GL_TRIANGLES);
            for (int i = 1; i < total - 1; i++) {
                int seq[3] = {0, i, i + 1};
                for (int k = 0; k < 3; k++) {
                    int vidx = idxV[seq[k]];
                    int nidx = idxN[seq[k]];
                    int tidx = idxT[seq[k]];
                    if (vidx < 0) vidx = nV + vidx + 1;

                    if (nidx != 0) {
                        int realN = (nidx < 0) ? (nN + nidx + 1) : nidx;
                        glNormal3f(normais[realN - 1].x, normais[realN - 1].y, normais[realN - 1].z);
                    }
                    if (tidx != 0) {
                        int realT = (tidx < 0) ? (nT + tidx + 1) : tidx;
                        /* obj usa v crescendo de baixo pra cima; a maioria das texturas
                           carregadas com stb_image vem de cima pra baixo, entao invertemos o V */
                        glTexCoord2f(texcoord[realT - 1].u, 1.0f - texcoord[realT - 1].v);
                    }
                    glVertex3f(vertices[vidx - 1].x, vertices[vidx - 1].y, vertices[vidx - 1].z);
                }
            }
            glEnd();
        }
    }

    glEndList();
    fclose(arq);
    free(vertices);
    free(normais);
    free(texcoord);

    printf("[obj_loader] '%s' carregado: %d vertices, %d normais, %d coords de textura\n",
           caminho, nV, nN, nT);
    return lista;
}

GLuint carregarTextura(const char *caminho) {
    int largura, altura, canais;
    stbi_set_flip_vertically_on_load(0);
    unsigned char *dados = stbi_load(caminho, &largura, &altura, &canais, 0);
    if (!dados) {
        fprintf(stderr, "[obj_loader] Erro ao carregar textura '%s'\n", caminho);
        return 0;
    }

    GLenum formato = (canais == 4) ? GL_RGBA : GL_RGB;

    GLuint textura;
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* gluBuild2DMipmaps faz o papel de glTexImage2D + glGenerateMipmap juntos,
       mas usando GLU (OpenGL 1.1), que funciona em qualquer compilador/SO
       sem precisar carregar extensoes (diferente de glGenerateMipmap, que e
       OpenGL 3.0+ e nao existe direto no MinGW/Windows) */
    gluBuild2DMipmaps(GL_TEXTURE_2D, formato, largura, altura, formato, GL_UNSIGNED_BYTE, dados);

    stbi_image_free(dados);

    printf("[obj_loader] Textura '%s' carregada (%dx%d, %d canais)\n", caminho, largura, altura, canais);
    return textura;
}