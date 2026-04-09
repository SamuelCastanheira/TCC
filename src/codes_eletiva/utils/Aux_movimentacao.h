#ifndef AUX_MOVIMENTACAO_H
#define AUX_MOVIMENTACAO_H

#include <math.h>
#include "../consts/consts.h"

static inline void CalculaDistancia(
    float origem_x,
    float origem_y,
    float destino_x,
    float destino_y,
    float *distancia,
    float *direcao_rad
) { 
    float dx = 0, dy = 0;

    dx = destino_x - origem_x;
    dy = destino_y - origem_y;
    
    
    *distancia = sqrt(dx*dx + dy*dy);
    *direcao_rad = atan2(dy, dx);
}

static inline void IniciaMovimentacao(
    float *origem_x, 
    float *origem_y, 
    float destino_x,
    float destino_y,
    float *distancia,
    float *direcao_rad,
    float *velocidade_x, 
    float *velocidade_y,
    float escalar_velocidade
) {
    CalculaDistancia(
        *origem_x,
        *origem_y,
        destino_x,
        destino_y,
        distancia,
        direcao_rad
    );
    float dx = 0, dy = 0;
    dx = destino_x - *origem_x;
    dy = destino_y - *origem_y;   
    
    if (*distancia > 0) {
        *velocidade_x = (dx / *distancia) * escalar_velocidade;
        *velocidade_y = (dy / *distancia) * escalar_velocidade;
    }
}

static inline SDL_Point AtualizaPosicao(
    float *origem_x, 
    float *origem_y, 
    float destino_x, 
    float destino_y, 
    float *velocidade_x, 
    float *velocidade_y, 
    float *distancia
) {
    // Calcula vetor de direção antes do movimento
    float dx_antes = destino_x - *origem_x;
    float dy_antes = destino_y - *origem_y;
    float dist_antes = sqrt(dx_antes*dx_antes + dy_antes*dy_antes);
    
    // Atualiza posição usando variáveis float
    *origem_x += *velocidade_x;
    *origem_y += *velocidade_y;
        
    // Recalcula distância após movimento
    float dx = destino_x - *origem_x;
    float dy = destino_y - *origem_y;
    float nova_distancia = sqrt(dx*dx + dy*dy);
    
    // Calcula produto escalar para verificar se passou do destino
    float produto_escalar = dx_antes * dx + dy_antes * dy;
        
    // Se chegou perto o suficiente OU passou do destino (produto escalar negativo)
    if (nova_distancia < 0.5f || produto_escalar < 0) {
        *origem_x = destino_x;
        *origem_y = destino_y;
        *velocidade_x = 0;
        *velocidade_y = 0;
        *distancia = 0;
    } else {
        // Atualiza a distância armazenada
        *distancia = nova_distancia;
    }
        
    SDL_Point p = { (int)roundf(*origem_x), (int)roundf(*origem_y) };
    return p;
}

static inline int DefineDirecaoCardinal(
    float direcao_rad
){
    if (direcao_rad < 5*M_PI/8 && direcao_rad >= 3*M_PI/8) {
        //sul
        return SUL;
    } else if (direcao_rad < 3*M_PI/8 && direcao_rad >= M_PI/8) {
        //sudeste
        return SUDESTE;
    } else if (direcao_rad < M_PI/8 && direcao_rad >= -M_PI/8) {
        //leste
        return LESTE;
    } else if (direcao_rad < -M_PI/8 && direcao_rad >= -3*M_PI/8) {
        //nordeste
        return NORDESTE;
    } else if (direcao_rad < -3*M_PI/8 && direcao_rad >= -5*M_PI/8) {
        //norte
        return NORTE;
    } else if (direcao_rad < -5*M_PI/8 && direcao_rad >= -7*M_PI/8) {
        //noroeste
        return NOROESTE;
    } else if (direcao_rad < -7*M_PI/8 || direcao_rad >= 7*M_PI/8) {
        //oeste
        return OESTE;
    } else if (direcao_rad < 7*M_PI/8 && direcao_rad >= 5*M_PI/8) {
        //sudoeste
        return SUDOESTE;
    }
    return SUL;
}

#endif