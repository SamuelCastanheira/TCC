#ifndef PERSONALIZACAO_H
#define PERSONALIZACAO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "../../consts/consts.h"
#include "../../utils/Aux_Timeout.h"
#include "../../utils/objeto.c"
#include "../../texturas/globais.c"
#include "../../texturas/texturaid.c"

// ---------------- VARIÁVEIS GLOBAIS ----------------

// Declaração global da cor selecionada
extern Cor corSelecionada;

#ifdef PERSONALIZACAO_IMPLEMENTACAO
Cor corSelecionada = AMARELO; // Definição única aqui
#endif


// ---------------- FUNÇÕES ----------------

static inline int RenderPersonalizacaoScreen(
    SDL_Window *janela, 
    SDL_Renderer *renderizador, 
    SDL_Event *evento, 
    Uint32 *timeout,
    GameState *estadoJogo
) {

    bool editando = true;
    SDL_SetWindowFullscreen(janela, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_SetWindowPosition(janela, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    
    SDL_Texture *pinguim_img = lista_txt.inicio[TEX_PINGUIM_AMARELO].txt;
    corSelecionada = AMARELO;

    int LARGURA, ALTURA;
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);

    Objeto background;
    background.rect = (SDL_Rect){0, 0, LARGURA, ALTURA};
    background.txt = lista_txt.inicio[TEX_BACKGROUND_PERSONALIZAR].txt;

    Objeto quadro;
    quadro.rect = (SDL_Rect){LARGURA*0.55, ALTURA*0.20, LARGURA*0.45, ALTURA*0.56};
    quadro.txt = lista_txt.inicio[TEX_QUADRO].txt;

    
    int tam_botao_x = 400, tam_botao_y = 120;

    SDL_Rect pinguim= {200, 100, 400, 400};
    Objeto escolha;

    escolha.rect = (SDL_Rect){LARGURA*0.60, ALTURA*0.20, LARGURA*0.35, ALTURA*0.05};
    escolha.txt = lista_txt.inicio[TEX_ESCOLHA].txt;
    SDL_Rect voltar= {LARGURA/7, (4*ALTURA)/5, tam_botao_x, tam_botao_y};

    SDL_Rect rectCores[TOTAL];

    int startX = LARGURA*0.60;
    int startY = ALTURA*0.30;
    int espaco = 10;
    int largura = 90;
    int altura  = 90;

    for (int i = 0; i < TOTAL; i++) {
        rectCores[i].x = startX + (i % 5) * (largura + espaco);
        rectCores[i].y = startY + (i / 5) * (altura + espaco);
        rectCores[i].w = largura;
        rectCores[i].h = altura;
    }

    SDL_Point mouse;

    while (true) {
        SDL_GetMouseState(&mouse.x, &mouse.y);

        SDL_SetRenderDrawColor(renderizador,225, 225, 255, 0);
        SDL_RenderClear(renderizador);
        SDL_RenderCopy(renderizador, background.txt, NULL, &background.rect);
        SDL_RenderCopy(renderizador, quadro.txt, NULL, &quadro.rect);
        SDL_RenderCopy(renderizador, escolha.txt, NULL , &escolha.rect);
        SDL_RenderCopy(renderizador, SDL_PointInRect(&mouse, &voltar) ?
        lista_txt.inicio[TEX_ELEMENTO_VOLTAR_HOVER].txt : lista_txt.inicio[TEX_ELEMENTO_VOLTAR].txt,
          NULL, &voltar);

        if (AUX_WaitEventTimeout(evento, timeout)) {
            if (evento->type == SDL_MOUSEBUTTONDOWN) {
                if (SDL_PointInRect(&mouse, &voltar)) {
                    *estadoJogo = STATE_MENU;
                    return 1;
                }
                for (int i = 0; i < TOTAL; i++) {
                    if (SDL_PointInRect(&mouse, &rectCores[i])) {
                        pinguim_img = lista_txt.inicio[i+TEX_PINGUIM_AMARELO].txt;
                        corSelecionada = (Cor)i; // 🔹 Atualiza a cor global
                    }
                }
            }
            else if(evento->type == SDL_QUIT) {
                *estadoJogo = STATE_SAIR;
                return 0;
            }
        }

        SDL_RenderCopy(renderizador, pinguim_img, NULL , &pinguim);
        for (int i = 0; i < TOTAL; i++) {
            SDL_RenderCopy(renderizador, lista_txt.inicio[i + TEX_COR_AMARELO].txt, NULL , &rectCores[i]);
        }
        SDL_RenderPresent(renderizador);
    }

    SDL_DestroyTexture(pinguim_img);
    return 1;
}

#endif
