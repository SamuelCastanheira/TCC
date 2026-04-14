#ifndef LOADING_H
#define LOADING_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../../consts/consts.h"
#include "../../utils/Aux_Timeout.h"
#include "../../texturas/leitura_arquivos.c"
#include "../../texturas/globais.c"

static inline int RenderLoadingScreen(
    SDL_Window *janela,
    SDL_Renderer *renderizador,
    SDL_Event *evento,
    Uint32 *timeout,
    GameState *estadoJogo)
{

    SDL_Texture *capa = IMG_LoadTexture(renderizador, "../../assets/imgs/capa.png");
    assert(capa != NULL);
    int LARGURA, ALTURA;
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);

    SDL_Rect img_capa = {0, 0, LARGURA, ALTURA};
    SDL_Rect carregamento = {0, 0, 1, 20};
    bool carregando = true;
    int qnt;

    const char *relativo = "texturas/menu.json";

    // --- 2. Pega o caminho base do executável (SDL garante válido em Windows/Linux/macOS) ---
    char *base = SDL_GetBasePath();

    // --- 3. Monta o caminho completo ---
    char caminho_completo[512];
    snprintf(caminho_completo, sizeof(caminho_completo), "%s%s", base, relativo);
    TexturaInfo *info_txt = carregar_texturas(caminho_completo, &qnt);

    inicializa_lista_textura(&lista_txt, info_txt, qnt);

    carregamento.w = LARGURA * lista_txt.porcentagem_txt_lidas;

    while (carregando)
    {
        SDL_PumpEvents();
        if (AUX_WaitEventTimeout(evento, timeout))
        {
        }

        if (lista_txt.texturas_lidas_sucesso < lista_txt.qnt_texturas)
        {
            le_prox_textura(&lista_txt,renderizador);
            carregamento.w = LARGURA*lista_txt.porcentagem_txt_lidas;
        }
        else
        {
            carregando = false;
        }
        SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 0);
        SDL_RenderClear(renderizador);
        SDL_RenderCopy(renderizador, capa, NULL, &img_capa);
        SDL_SetRenderDrawColor(renderizador, 0, 255, 0, 0);
        SDL_RenderFillRect(renderizador, &carregamento);
        SDL_RenderPresent(renderizador);
    }
    *estadoJogo = STATE_MENU;
    // Liberando recursos
    SDL_DestroyTexture(capa);
    return 1;
}

#endif
