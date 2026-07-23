#ifndef LOGICA_H
#define LOGICA_H

typedef enum { JOGANDO, VITORIA } EstadoJogo;

typedef struct {
    float x, y, z;
    float angulo;       // direcao para onde o jogador aponta (radianos) 
    float velocidade;   // pode ser negativa (re) 
} Jogador;

typedef struct {
    float x, y, z;
    float raio;
    int ativo;          // 0 = ja foi coletada
} Moeda;

typedef struct {
    float x, y, z;
    float raio;
} Obstaculo;

#define MAX_MOEDAS 10
#define MAX_OBSTACULOS 6

typedef struct {
    Jogador jogador;
    Moeda moedas[MAX_MOEDAS];
    Obstaculo obstaculos[MAX_OBSTACULOS];
    int moedasColetadas;
    float tempoDecorrido;
    EstadoJogo estado;
} EstadoDoJogo;

// preenche o estado inicial (posicoes) 
void inicializarJogo();


// avanca a simulacao em dt segundos.
// acelerando = 1 se a tecla de acelerar esta pressionada
// freando = 1 se a tecla de frear/re esta pressionada
// direcao = -1 (vira esquerda), 0 (reto), +1 (vira direita)
void atualizarJogo(float dt, int acelerando, int freando, float direcao);

// teste generico de colisao esfera-esfera, reutilizavel para qualquer par de objetos */
int testarColisaoEsferas(float x1, float y1, float z1, float r1, float x2, float y2, float z2, float r2);

#endif