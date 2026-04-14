#ifndef PEGAPUFFLE_H
#define PEGAPUFFLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../../utils/Aux_monitor.h"
#include "../../utils/Aux_Timeout.h"
#include "../../utils/Aux_movimentacao.h"
#include "../../consts/consts.h"

typedef struct {
    float x;
    float y;
} Velocidade ;

static inline int VerificaColisaoRect(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x &&
            a.y < b.y + b.h && a.y + a.h > b.y);
}

static inline int VerificaPuffleDentroCercado(SDL_Rect puffle, SDL_Rect cercado) {
    return (puffle.x >= cercado.x &&
            puffle.y >= cercado.y &&
            puffle.x + puffle.w <= cercado.x + cercado.w &&
            puffle.y + puffle.h <= cercado.y + cercado.h);
}

static inline int RenderPegaPuffleScreen(
    SDL_Window *janela, 
    SDL_Renderer *renderizador, 
    SDL_Event *evento, 
    Uint32 *timeout, 
    GameState *estadoJogo
) {
    int LARGURA, ALTURA;
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);

    IMG_Init(IMG_INIT_PNG);
    SDL_Texture *fundo_img = IMG_LoadTexture(renderizador, "../../assets/imgs/puffle_roundup/fundo.png");
    SDL_Texture *arvores_img = IMG_LoadTexture(renderizador, "../../assets/imgs/puffle_roundup/arvores.png");

    int num_puffles = 10;
    int altura_puffle = ALTURA/18;
    int largura_puffle = ALTURA/18;

    const char *puffle_paths[] = {
        "../../assets/imgs/puffle_roundup/puffle_amarelo.png",
        "../../assets/imgs/puffle_roundup/puffle_azul.png",
        "../../assets/imgs/puffle_roundup/puffle_branco.png",
        "../../assets/imgs/puffle_roundup/puffle_vermelho.png",
        "../../assets/imgs/puffle_roundup/puffle_verde.png",
        "../../assets/imgs/puffle_roundup/puffle_laranja.png",
        "../../assets/imgs/puffle_roundup/puffle_rosa.png",
        "../../assets/imgs/puffle_roundup/puffle_roxo.png",
        "../../assets/imgs/puffle_roundup/puffle_preto.png",
        "../../assets/imgs/puffle_roundup/puffle_marrom.png"
    };
    SDL_Texture *puffle_textures[num_puffles];
    for (int i = 0; i < num_puffles; i++) {
        puffle_textures[i] = IMG_LoadTexture(renderizador, puffle_paths[i]);
    }

    SDL_Rect hitboxes[3] = {
        {17*LARGURA/40, ALTURA/2, LARGURA/30, ALTURA/4},
        {10*LARGURA/24, 3*ALTURA/4, 2*LARGURA/11, ALTURA/15},
        {45*LARGURA/80, ALTURA/2, LARGURA/30, ALTURA/4}
    };
    SDL_Rect cercado = {17*LARGURA/40, 6*ALTURA/10, 2*LARGURA/13, 3*ALTURA/10};

    SDL_Rect cenario_rect = {0, 0, LARGURA, ALTURA};
    SDL_Point_Float posicoes_puffles[num_puffles];
    SDL_Point_Float destinos_puffles[num_puffles];
    SDL_Rect puffle_rects[num_puffles];
    float distancias[num_puffles], direcoes_rad[num_puffles];
    Velocidade velocidades[num_puffles];
    float escalar_velocidade = 1.0f;

    srand(time(NULL));
    for (int i = 0; i < num_puffles; i++) {     
        int x_random = (LARGURA / 3) + (rand() % ((LARGURA / 3) + 1));
        int y_random = (ALTURA / 10) + (rand() % ((3*ALTURA / 10) + 1));
        puffle_rects[i] = (SDL_Rect){x_random, y_random, altura_puffle, largura_puffle};
        posicoes_puffles[i] = (SDL_Point_Float){(float)puffle_rects[i].x, (float)puffle_rects[i].y};
        destinos_puffles[i] = (SDL_Point_Float){(float)puffle_rects[i].x, (float)puffle_rects[i].y};
        distancias[i] = 0;
        direcoes_rad[i] = 0;
        velocidades[i] = (Velocidade){0.0f, 0.0f};
    }
  
    while (true) {
        

        SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 0);
        SDL_RenderClear(renderizador);

        SDL_RenderCopy(renderizador, fundo_img, NULL, &cenario_rect);

        for (int i = 0; i < num_puffles; i++) {
            SDL_RenderCopy(renderizador, puffle_textures[i], NULL, &puffle_rects[i]);
        }
        SDL_RenderCopy(renderizador, arvores_img, NULL, &cenario_rect);


        if (AUX_WaitEventTimeout(evento, timeout)) {
            if (evento->type == SDL_KEYDOWN) {
                if (evento->key.keysym.sym == SDLK_ESCAPE) {
                    *estadoJogo = STATE_JOGANDO;

                    IMG_Quit();
                    return 1;
                }
            } else if (evento->type == SDL_MOUSEMOTION) {
                float ponto_inicial_x = evento->motion.x;
                float ponto_inicial_y = evento->motion.y;
                
                
                for (int i = 0; i < num_puffles; i++) {
                    //calcula ponto para inverter direcao do movimento do puffle
                    float dx = ponto_inicial_x - posicoes_puffles[i].x;
                    float dy = ponto_inicial_y - posicoes_puffles[i].y;
                    float ponto_invertido_x = ponto_inicial_x - (2 * dx);
                    float ponto_invertido_y = ponto_inicial_y - (2 * dy);
                    
                    destinos_puffles[i].x = ponto_invertido_x - largura_puffle/2;
                    destinos_puffles[i].y = ponto_invertido_y - altura_puffle/2;
                    
                    CalculaDistancia(
                        posicoes_puffles[i].x,
                        posicoes_puffles[i].y,
                        destinos_puffles[i].x,
                        destinos_puffles[i].y,
                        &distancias[i],
                        &direcoes_rad[i]
                    );
                    if (distancias[i] > 0.5f && distancias[i] <= 200) {
                        IniciaMovimentacao(
                            &posicoes_puffles[i].x,
                            &posicoes_puffles[i].y,
                            destinos_puffles[i].x,
                            destinos_puffles[i].y,
                            &distancias[i],
                            &direcoes_rad[i],
                            &velocidades[i].x,
                            &velocidades[i].y,
                            escalar_velocidade
                        );
                    }
                    
                }
            }
        }
        
        // Atualiza posições dos puffles continuamente
        for (int i = 0; i < num_puffles; i++) {
            if (distancias[i] > 0.5f && distancias[i] <= 200) {
                // Salva posição anterior para reverter em caso de colisão
                float pos_x_anterior = posicoes_puffles[i].x;
                float pos_y_anterior = posicoes_puffles[i].y;
                
                SDL_Point nova_posicao = AtualizaPosicao(
                    &posicoes_puffles[i].x, 
                    &posicoes_puffles[i].y, 
                    destinos_puffles[i].x, 
                    destinos_puffles[i].y, 
                    &velocidades[i].x, 
                    &velocidades[i].y, 
                    &distancias[i]
                );
    
                puffle_rects[i].x = nova_posicao.x;
                puffle_rects[i].y = nova_posicao.y;
                
                // Verifica colisão com todas as hitboxes
                int colidiu = 0;
                for (int j = 0; j < 3; j++) {
                    if (VerificaColisaoRect(puffle_rects[i], hitboxes[j])) {
                        colidiu = 1;
                        break;
                    }
                }
                
                // Se colidiu, reverte movimento e para o puffle
                if (colidiu) {
                    posicoes_puffles[i].x = pos_x_anterior;
                    posicoes_puffles[i].y = pos_y_anterior;
                    puffle_rects[i].x = (int)roundf(pos_x_anterior);
                    puffle_rects[i].y = (int)roundf(pos_y_anterior);
                    velocidades[i].x = 0;
                    velocidades[i].y = 0;
                    distancias[i] = 0;
                }
            }
        }

        // Verifica se todos os puffles estão dentro do cercado
        int todos_dentro = 1;
        for (int i = 0; i < num_puffles; i++) {
            if (!VerificaPuffleDentroCercado(puffle_rects[i], cercado)) {
                todos_dentro = 0;
                break;
            }
        }
        
        // Se todos os puffles estão dentro do cercado, termina o jogo
        if (todos_dentro) {
            *estadoJogo = STATE_JOGANDO;
            IMG_Quit();
            return 1;
        }

        SDL_RenderPresent(renderizador);
    }

    IMG_Quit();
    return 1;
}

#endif