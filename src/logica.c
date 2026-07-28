#include "logica.h"
#include <math.h>

#define ACELERACAO 3.0f //unidades por seg^2
#define ATRITO 1.5f // desacelera (tecla solta)
#define VEL_MAXIMA 6.0f //unidades por seg
#define VEL_ROTACAO 2.0f //rad por segundos
#define RAIO_COLISAO 1.f // teste de atrito

EstadoDoJogo jogo;

void inicializarJogo() {
    jogo.jogador.x = 0;
    jogo.jogador.y = 0;
    jogo.jogador.z = 30;   
    jogo.jogador.angulo = 0;
    jogo.jogador.velocidade = 0;

    float posMoedas[MAX_MOEDAS][2] = {
        {   -10.54f,    29.88f },
        {    -0.79f,    21.58f },
        {   -24.15f,    21.58f },
        {   -12.88f,    15.04f },
        {   -20.10f,    34.71f },
        {    19.53f,    26.55f },
        {    22.71f,    18.99f },
        {     9.24f,    11.58f },
        {    26.50f,    -1.04f },
        {   -26.45f,     4.04f },
        {    35.82f,   -25.61f },
        {    -0.70f,   -25.61f },
        {     6.60f,   -10.55f },
        {   -30.64f,   -21.25f },
    };

    for (int i = 0; i < MAX_MOEDAS; i++) {
        jogo.moedas[i].x = posMoedas[i][0];
        jogo.moedas[i].y = 0.0f;
        jogo.moedas[i].z = posMoedas[i][1];
        jogo.moedas[i].raio = 0.6f;
        jogo.moedas[i].ativo = 1;
    }

    float posObstaculos[MAX_OBSTACULOS][3] = {
        {     0.00f,     0.00f, 0.0f },
        {    25.41f,   -18.27f, 0.0f },
        {    25.41f,    29.03f, 0.0f },
        {   -15.50f,    34.40f, 0.0f },
        {     2.90f,    25.00f, 1.0f },
        {    20.00f,     0.00f, 1.0f },
        {   -20.01f,     4.95f, 1.0f },
        {    -6.49f,   -27.88f, 1.0f },
        {    30.54f,   -27.88f, 1.0f },
        {   -23.19f,   -18.61f, 2.0f },
        {    12.49f,    35.74f, 2.0f },
        {    23.88f,    13.99f, 2.0f },
        {   -28.70f,    21.00f, 3.0f },
        {     3.13f,   -12.84f, 3.0f },
        {   -28.70f,   -32.71f, 3.0f },
    };

    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        jogo.obstaculos[i].x = posObstaculos[i][0];
        jogo.obstaculos[i].y = -2.5f;
        jogo.obstaculos[i].z = posObstaculos[i][1];
        jogo.obstaculos[i].raio = 3.0f;
        jogo.obstaculos[i].tipo = (int) posObstaculos[i][2]; 
    }

    jogo.moedasColetadas = 0;
    jogo.tempoDecorrido = 0;
    jogo.estado = JOGANDO;
}

int testarColisaoEsferas(float x1, float y1, float z1, float r1,float x2, float y2, float z2, float r2) {
    float dx = x1 - x2, dy = y1 - y2, dz = z1 - z2;
    float distQuadrada = dx * dx + dy * dy + dz * dz;
    float somaRaios = r1 + r2;
    return distQuadrada <= (somaRaios * somaRaios);
}

void atualizarJogo(float dt, int acelerando, int freando, float direcao) {
    // acelerando = 1 se a tecla de acelerar (w) esta pressionada
    // freando = 1 se a tecla de ré (s) esta pressionada
    // direcao = -1 (vira esquerda), 0 (reto), +1 (vira direita)

    if (jogo.estado != JOGANDO) return;

    jogo.tempoDecorrido += dt;

    //atualiza oangulo do barco: direcao (esq ou dir) rad por segundos e segundos
    jogo.jogador.angulo += direcao * VEL_ROTACAO * dt; 

    if (acelerando) {
        jogo.jogador.velocidade += ACELERACAO * dt;
    } else if (freando) {
        jogo.jogador.velocidade -= ACELERACAO * dt;
    } else { //teclas soltas
        if (jogo.jogador.velocidade > 0.0f) { 
            jogo.jogador.velocidade -= ATRITO * dt; // diminui velocidade ate parar
            if (jogo.jogador.velocidade < 0.0f) jogo.jogador.velocidade = 0.0f; //evita que ele ande de ré
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

    if (jogo.jogador.x > LIMITE_MAPA_X) {
        jogo.jogador.x = LIMITE_MAPA_X;
        jogo.jogador.velocidade *= -0.3f;
    } else if (jogo.jogador.x < -LIMITE_MAPA_X) {
        jogo.jogador.x = -LIMITE_MAPA_X;
        jogo.jogador.velocidade *= -0.3f;
    }

    if (jogo.jogador.z > LIMITE_MAPA_Z) {
        jogo.jogador.z = LIMITE_MAPA_Z;
        jogo.jogador.velocidade *= -0.3f;
    } else if (jogo.jogador.z < -LIMITE_MAPA_Z) {
        jogo.jogador.z = -LIMITE_MAPA_Z;
        jogo.jogador.velocidade *= -0.3f;
    }

    //colisao com moedas
    for (int i = 0; i < MAX_MOEDAS; i++) {
        if (jogo.moedas[i].ativo &&
            testarColisaoEsferas(jogo.jogador.x, jogo.jogador.y, jogo.jogador.z,RAIO_COLISAO ,
                jogo.moedas[i].x, jogo.moedas[i].y, jogo.moedas[i].z, jogo.moedas[i].raio)) {
            jogo.moedas[i].ativo = 0;
            jogo.moedasColetadas++;
        }
    }

    // colisao com obstaculos 
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        if (testarColisaoEsferas(jogo.jogador.x, jogo.jogador.y, jogo.jogador.z, RAIO_COLISAO,
            jogo.obstaculos[i].x, jogo.obstaculos[i].y, jogo.obstaculos[i].z,
            jogo.obstaculos[i].raio)) {

            float empurraoFixo = 0.4f; //unidades de distancia que recua por frame
            jogo.jogador.x -= sinf(jogo.jogador.angulo) * empurraoFixo; 
            jogo.jogador.z += cosf(jogo.jogador.angulo) * empurraoFixo;
        
            //velocidade negativa faz ele zerar com logica ja existente do atrito
            jogo.jogador.velocidade = -2.0f;
        }
    }

    if (jogo.moedasColetadas >= MAX_MOEDAS) {
        jogo.estado = VITORIA;
    } else if (jogo.tempoDecorrido >= TEMPO_LIMITE) {
        jogo.estado = DERROTA;
    }
}