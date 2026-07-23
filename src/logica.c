#include "logica.h"
#include <math.h>

#define ACELERACAO 3.0f
#define ATRITO 1.5f
#define VEL_MAXIMA 6.0f
#define VEL_ROTACAO 2.0f
#define RAIO_COLISAO 0.10f

EstadoDoJogo jogo;

void inicializarJogo() {
    jogo.jogador.x = 0.0f;
    jogo.jogador.y = 0.0f;
    jogo.jogador.z = 30.0f;   
    jogo.jogador.angulo = 3.14159265f;
    jogo.jogador.velocidade = 0.0f;

    float posMoedas[MAX_MOEDAS][2] = {
        { 10.0f,  15.0f},
        {-20.0f,   8.0f},
        { 25.0f, -10.0f},
        {-15.0f, -25.0f},
        {  5.0f,  30.0f},
        {-30.0f,  -5.0f},
        { 18.0f,  22.0f},
        { -8.0f, -18.0f},
        { 30.0f,   5.0f},
        {  0.0f, -30.0f}
    };

    for (int i = 0; i < MAX_MOEDAS; i++) {
        jogo.moedas[i].x = posMoedas[i][0];
        jogo.moedas[i].y = 0.0f;
        jogo.moedas[i].z = posMoedas[i][1];
        jogo.moedas[i].raio = 0.6f;
        jogo.moedas[i].ativo = 1;
    }

    float posObstaculos[MAX_OBSTACULOS][2] = {
        { 15.0f,  10.0f},
        {-10.0f,  20.0f},
        { 20.0f, -20.0f},
        {-25.0f,  10.0f},
        {  5.0f, -15.0f},
        {-15.0f,  -8.0f}
    };

    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        jogo.obstaculos[i].x = posObstaculos[i][0];
        jogo.obstaculos[i].y = 0.0f;
        jogo.obstaculos[i].z = posObstaculos[i][1];
        jogo.obstaculos[i].raio = 1.0f;
    }

    jogo.moedasColetadas = 0;
    jogo.tempoDecorrido = 0.0f;
    jogo.estado = JOGANDO;
}

int testarColisaoEsferas(float x1, float y1, float z1, float r1,float x2, float y2, float z2, float r2) {
    float dx = x1 - x2, dy = y1 - y2, dz = z1 - z2;
    float distQuadrada = dx * dx + dy * dy + dz * dz;
    float somaRaios = r1 + r2;
    return distQuadrada <= (somaRaios * somaRaios);
}

void atualizarJogo(float dt, int acelerando, int freando, float direcao) {
    if (jogo.estado != JOGANDO) return;

    jogo.tempoDecorrido += dt;

    jogo.jogador.angulo += direcao * VEL_ROTACAO * dt;

    if (acelerando) {
        jogo.jogador.velocidade += ACELERACAO * dt;
    } else if (freando) {
        jogo.jogador.velocidade -= ACELERACAO * dt;
    } else {
        if (jogo.jogador.velocidade > 0.0f) {
            jogo.jogador.velocidade -= ATRITO * dt;
            if (jogo.jogador.velocidade < 0.0f) jogo.jogador.velocidade = 0.0f;
        } else if (jogo.jogador.velocidade < 0.0f) {
            jogo.jogador.velocidade += ATRITO * dt;
            if (jogo.jogador.velocidade > 0.0f) jogo.jogador.velocidade = 0.0f;
        }
    }

    if (jogo.jogador.velocidade > VEL_MAXIMA) jogo.jogador.velocidade = VEL_MAXIMA;
    if (jogo.jogador.velocidade < -VEL_MAXIMA / 2.0f) jogo.jogador.velocidade = -VEL_MAXIMA / 2.0f;

    //atualiza pos do jogador
    jogo.jogador.x += sinf(jogo.jogador.angulo) * jogo.jogador.velocidade * dt;
    jogo.jogador.z -= cosf(jogo.jogador.angulo) * jogo.jogador.velocidade * dt;


    //colisao com moedas
    for (int i = 0; i < MAX_MOEDAS; i++) {
        if (jogo.moedas[i].ativo &&
            testarColisaoEsferas(jogo.jogador.x, jogo.jogador.y, jogo.jogador.z,RAIO_COLISAO ,
                jogo.moedas[i].x, jogo.moedas[i].y, jogo.moedas[i].z, jogo.moedas[i].raio)) {
            jogo.moedas[i].ativo = 0;
            jogo.moedasColetadas++;
        }
    }

    // colisao com obstaculos, o jogador "quica" para tras 
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        if (testarColisaoEsferas(jogo.jogador.x, jogo.jogador.y, jogo.jogador.z, RAIO_COLISAO,
            jogo.obstaculos[i].x, jogo.obstaculos[i].y, jogo.obstaculos[i].z,
            jogo.obstaculos[i].raio)) {

            jogo.jogador.velocidade *= -0.4f;
        }
    }

    if (jogo.moedasColetadas >= MAX_MOEDAS) {
        jogo.estado = VITORIA;
    }
}