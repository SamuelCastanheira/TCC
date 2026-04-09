#ifndef BEAN_GLOBAIS
#define BEAN_GLOBAIS

#include <SDL2/SDL.h>

//Dimensões da Tela
int LARGURA, ALTURA;

// Gravidade da carga
double gravidade;
double k = 1.045;

// Marcadores de tempo
Uint32 tempo_anterior_carga = 0;

//limites de movimentao do pinguim
int limite_esq;
int limite_dir;

#endif
