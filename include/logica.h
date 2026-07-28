#ifndef LOGICA_H
#define LOGICA_H

typedef enum { JOGANDO, VITORIA, DERROTA } EstadoJogo;

typedef struct {
    float x, y, z;
    float angulo; // direcao para onde o jogador aponta (rad) 
    float velocidade;  // pode ser negativa (re) 
} Jogador;

typedef struct {
    float x, y, z;
    float raio;
    int ativo;        
} Moeda;

typedef struct {
    float x, y, z;
    float raio;
    int tipo;    // 0, 1, 2 ou 3 indica qual .obj usar
} Obstaculo;

#define MAX_MOEDAS 14
#define MAX_OBSTACULOS 15
#define TEMPO_LIMITE 70.0f
#define LIMITE_MAPA_X 39.0f
#define LIMITE_MAPA_Z 49.0f
typedef struct {
    Jogador jogador;
    Moeda moedas[MAX_MOEDAS];
    Obstaculo obstaculos[MAX_OBSTACULOS];
    int moedasColetadas;
    float tempoDecorrido;
    EstadoJogo estado;
} EstadoDoJogo;

void inicializarJogo();

// avanca a simulacao em dt segundos
void atualizarJogo(float dt, int acelerando, int freando, float direcao);

int testarColisaoEsferas(float x1, float y1, float z1, float r1, float x2, float y2, float z2, float r2);

#endif