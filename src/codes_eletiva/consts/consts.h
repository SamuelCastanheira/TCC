#ifndef CONSTS_H
#define CONSTS_H
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../utils/Aux_monitor.h"

/*
Caso haja constantes de outro domínio, adicione um comentário 
para entitular o domínio e liste as constantes seguindo o padrão abaixo
*/

//CONSTANTES DE TELA
const int LARGURA_PADRAO = 800;
const int ALTURA_PADRAO = 600;


//CONSTANTES DE SISTEMA 
Uint32 timeout = 200;
bool rodando = true;

typedef enum {
    STATE_LOADING,
    STATE_MENU,
    STATE_PERSONALIZACAO,
    STATE_JOGANDO,
    STATE_BEANS_COUNTERS,
    STATE_PEGA_PUFFLE,
    STATE_SAIR,
    STATE_DOJO
} GameState;

//CONSTANTES PERSONALIZACAO
typedef enum {
    AMARELO,
    AVERMELHADO,
    AZUL_FORTE,
    AZUL_FRACO,
    CIANO,
    CINZA,
    LARANJA,
    MARROM,
    PRETO,
    ROSA_MEDIO,
    ROXO,
    VERDE_FORTE,
    VERDE_MEDIO,
    VERDE_VOMITO,
    VERMELHO,
    TOTAL 
} Cor;

const char *nomes_cores[TOTAL] = {
    "amarelo",        
    "rosa",           
    "azul",           
    "azul_claro",     
    "ciano",          
    "cinza",          
    "laranja",        
    "marrom",         
    "preto",          
    "rosa",           
    "roxo",           
    "verdeMusgo",     
    "verdeEscuro",    
    "verde",          
    "vermelho"        
};

//CONSTANTES DIREÇÃO
typedef struct {
    float x;
    float y;
} SDL_Point_Float;

typedef enum {
    SUL,
    SUDESTE,
    LESTE,
    NORDESTE,
    NORTE,
    NOROESTE,
    OESTE,
    SUDOESTE,
    TOTAL_DIRECOES
} Direcao;

const char *nomes_direcoes[8] = {
    "000",
    "045",
    "090",
    "135",
    "180",
    "225",
    "270",
    "315",
};

#endif