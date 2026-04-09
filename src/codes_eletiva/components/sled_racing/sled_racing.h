#ifndef SLEDRACING_H
#define SLEDRACING_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include "../../consts/consts.h"
#include "../../utils/Aux_Timeout.h"

typedef struct {
    SDL_Texture *textura;
    SDL_Rect rec;
} Sprite;

SDL_Texture * terrenos[25];

static inline void carregaImagens_sled(SDL_Renderer *renderizador) 
{
    terrenos[0]  = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_98.webp");
    terrenos[1]  = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[2]  = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[3]  = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[4]  = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[5]  = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[6]  = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[7]  = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[8]  = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[9]  = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[10] = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[11] = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[12] = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[13] = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[14] = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[15] = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[16] = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[17] = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[18] = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[19] = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[20] = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[21] = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[22] = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[23] = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
    terrenos[24] = IMG_LoadTexture(renderizador, "imgs/Sled_Racing/Sled_Racing_track_segment_0.webp");
}

int velocidade = 1;

static inline int RenderSledRacingScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event *evento, Uint32 *timeout, GameState *estadoJogo) 
{
    SDL_SetWindowFullscreen(janela, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_SetWindowPosition(janela, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    carregaImagens_sled(renderizador);

    int LARGURA, ALTURA;
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);

    SDL_Point mouse;
    double coef = -0.5;         // inclinação da linha diagonal
    int y_base = ALTURA;         // ponto inicial Y (canto inferior da tela)
    int x_base = LARGURA;        // ponto inicial X (canto direito da tela)
    char pistaLog1[20];
    char pistaLog2[20];
    char pistaLog3[20];
    char pistaLog4[20];
    
    Sprite pista[6];
    pista[0].rec.x = 0;
    pista[0].rec.y = (int)(coef * (x_base - pista[0].rec.x) + y_base);
    pista[0].rec.w = 1000;
    pista[0].rec.h = 506;
    pista[0].textura = terrenos[0];
    sprintf(pistaLog1, "%d", pista[0].rec.y ); 
    SDL_Log(pistaLog1);

    pista[1].rec.x = 620;
    pista[1].rec.y = (int)(coef * (x_base - pista[1].rec.x) + y_base);
    pista[1].rec.w = 691;
    pista[1].rec.h = 415;
    pista[1].textura = terrenos[1];
    sprintf(pistaLog2, "%d", pista[1].rec.y ); 
    SDL_Log(pistaLog2);
 
    pista[2].rec.x = 955;
    pista[2].rec.y = (int)(coef * (x_base - pista[2].rec.x) + y_base);
    pista[2].rec.w = 691;
    pista[2].rec.h = 415;
    pista[2].textura = terrenos[2];
    sprintf(pistaLog3, "%d", pista[2].rec.y ); 
    SDL_Log(pistaLog3);

    pista[3].rec.x = 1290;
    pista[3].rec.y = (int)(coef * (x_base - pista[3].rec.x) + y_base);
    pista[3].rec.w = 691;
    pista[3].rec.h = 415;
    pista[3].textura = terrenos[3];
    sprintf(pistaLog4, "%d", pista[3].rec.y ); 
    SDL_Log(pistaLog4);

    while (true) {
        SDL_GetMouseState(&mouse.x, &mouse.y);
        srand(time(NULL));

        SDL_SetRenderDrawColor(renderizador, 225, 225, 255, 0);
        SDL_RenderClear(renderizador);

        for (int i = 0; i < 4; i++) {
            // Move para a esquerda
            pista[i].rec.x -= velocidade;

            // Calcula Y seguindo a linha diagonal fixa
            pista[i].rec.y = (int)(coef * (x_base - pista[i].rec.x) + y_base);

            // Se saiu completamente da tela
            if (pista[i].rec.x + pista[i].rec.w < 0 || pista[i].rec.y + pista[i].rec.h/2 < 0) {
                // Encontra o maior X atual entre os blocos
                int max_x = pista[0].rec.x;
                int max_w = pista[0].rec.w;
                for (int j = 1; j < 4; j++) {
                    if (pista[j].rec.x > max_x) {
                        max_x = pista[j].rec.x;
                        max_w = pista[j].rec.w;
                    }
                }

                // Reposiciona logo após o último bloco
                pista[i].rec.x = max_x + max_w/2;  // alinhado perfeitamente
                pista[i].rec.y = (int)(coef * (x_base - pista[i].rec.x) + y_base);

                pista[i].rec.w = 691;
                pista[i].rec.h = 415;

                // Troca textura
                pista[i].textura = terrenos[rand() % 25];
            }

            // Renderiza o bloco
            SDL_RenderCopy(renderizador, pista[i].textura, NULL, &pista[i].rec);
        }

        if (AUX_WaitEventTimeout(evento, timeout)) {
            if (evento->type == SDL_KEYDOWN) {
                if (evento->key.keysym.sym == SDLK_ESCAPE) {
                    *estadoJogo = STATE_JOGANDO;

                    IMG_Quit();
                    return 1;
                }
            }
        }

        SDL_RenderPresent(renderizador);
    }

    return 1;
}

#endif
