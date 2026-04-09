#ifndef BEAN_COUNTERS_H
#define BEAN_COUNTERS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "../../utils/Aux_Timeout.h"
#include "../../utils/Aux_monitor.h"
#include "../personalizacao/personalizacao.h"
#include "globais.c"
#include "pinguim.c"
#include "carga.c"
#include "pilha.c"
#include "enums.c"
#include "../../texturas/leitura_arquivos.c"
#include "../../texturas/globais.c"

// ==================================================
// TELA PRINCIPAL — PINGUIM MANTIDO, CARGA CORRIGIDA
// ==================================================
static inline int RenderBeanCountersScreen(
    SDL_Window *janela,
    SDL_Renderer *renderizador,
    SDL_Event *evento,
    Uint32 *timeout,
    GameState *estadoJogo)
{

    srand(time(NULL));

    SDL_Point inicial_mouse;
    SDL_Point atual_mouse;

    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    SDL_GetMouseState(&inicial_mouse.x, &inicial_mouse.y);

    gravidade = ALTURA * k;

    limite_esq = LARGURA * 0.25;
    limite_dir = LARGURA * 0.75;

    IMG_Init(IMG_INIT_PNG);

    SDL_Rect chao = {0, ALTURA * 0.90, LARGURA, ALTURA * 0.10};
    SDL_Rect background = {0, 0, LARGURA, ALTURA};
    SDL_Texture *txt_background = lista_txt.inicio[TEX_BEAN_BACKGROUND].txt;

    SDL_Rect neve = {0, ALTURA * 0.37, LARGURA, ALTURA * 0.70};
    SDL_Texture *txt_neve = lista_txt.inicio[TEX_BEAN_NEVE].txt;

    SDL_Rect plataforma = {0, ALTURA * 0.80, LARGURA * 0.30, ALTURA * 0.15};
    SDL_Texture *txt_plataforma = lista_txt.inicio[TEX_BEAN_PLATAFORMA].txt;

    // Pinguim
    Pinguim pinguim;
    inicializa_pinguim(renderizador, &pinguim);

    // Caminhão
    SDL_Texture *caminhao_txt = lista_txt.inicio[TEX_BEAN_TRUCKER].txt;
    SDL_Rect caminhao = {LARGURA * 0.80, ALTURA * 0.15, LARGURA * 0.20, ALTURA * 0.85};

    // CARGA
    ListaCarga cargas_jogadas;
    inicializa_lista_carga(&cargas_jogadas);

    // POSIÇÃO EM DOUBLE PARA PINGUIM

    Pilha pilha_1, pilha_2;
    inicializa_pilha(&pilha_1);
    inicializa_pilha(&pilha_2);
    pilha_1.pos_x = LARGURA * 0.1;
    pilha_1.pos_y = ALTURA * 0.80;

    pilha_2.pos_x = LARGURA * 0.15;
    pilha_2.pos_y = ALTURA * 0.80;

    TTF_Init();
    TTF_Font *fnt = TTF_OpenFont("../fonts/Lovelo/lovelo_black.otf", 50);
    assert(fnt != NULL);
    SDL_Color clr = {0xFF, 0xFF, 0xFF, 0xFF};

    SDL_Surface *sfc_life = TTF_RenderText_Blended(fnt, "LIFE:  ", clr);
    assert(sfc_life != NULL);
    SDL_Texture *txt_life = SDL_CreateTextureFromSurface(renderizador, sfc_life);
    assert(txt_life != NULL);
    SDL_FreeSurface(sfc_life);

    SDL_Surface *sfc_score = TTF_RenderText_Blended(fnt, "SCORE: ", clr);
    assert(sfc_score != NULL);
    SDL_Texture *txt_score = SDL_CreateTextureFromSurface(renderizador, sfc_score);
    assert(txt_score != NULL);
    SDL_FreeSurface(sfc_score);

    SDL_Rect life = {LARGURA * 0.07, ALTURA * 0.025, LARGURA * 0.05, ALTURA * 0.04};
    SDL_Rect score = {LARGURA * 0.15, ALTURA * 0.025, LARGURA * 0.05, ALTURA * 0.04};

    SDL_ShowCursor(SDL_DISABLE);

    while (true)
    {
        SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 255);
        SDL_RenderClear(renderizador);

        if (pinguim.atingido && (SDL_GetTicks() - pinguim.momento_queda) >= 3000)
        {
            pinguim.atingido = false;
            pinguim.rect.x = pinguim.posicao_inicial;
            pinguim.destino = pinguim.posicao_inicial;
            pinguim.incremento = 0;
            pinguim.txt = pinguim.texturas[0];
            pinguim.last = SDL_GetTicks();
        }

        // =====================
        //    ENTRADAS
        // =====================
        if (AUX_WaitEventTimeout(evento, timeout))
        {
            if (evento->type == SDL_KEYDOWN && evento->key.keysym.sym == SDLK_ESCAPE)
            {
                *estadoJogo = STATE_JOGANDO;
                SDL_ShowCursor(SDL_ENABLE);
                libera_lista_carga(&cargas_jogadas);
                IMG_Quit();
                return 1;
            }
            if (evento->type == SDL_QUIT)
            {
                *estadoJogo = STATE_SAIR;
                SDL_ShowCursor(SDL_ENABLE);
                IMG_Quit();
                return 0;
            }
        }

        // ============================
        //  MOVIMENTO DA CARGA (FÍSICA)
        // ============================

        if (!pinguim.atingido)
        {
            sorteia_carga(renderizador, &cargas_jogadas);
            calcula_movimento_pinguim(&pinguim);
        }
        calcula_movimento_cargas(&cargas_jogadas);

        if (!pinguim.atingido)
        {
            Carga *c = cargas_jogadas.inicio;
            while (c != NULL)
            {
                if (c->ativo && !c->fade)
                {
                    if (SDL_HasIntersection(&pinguim.rect, &c->rect))
                    {
                        c->ativo = false;

                        if (c->tipo == PEIXE)
                        {
                            pinguim.txt = pinguim.texturas[8];
                            pinguim.vidas--;
                            pinguim.atingido = true;
                            pinguim.momento_queda = SDL_GetTicks();
                            pinguim.sacos = 0;
                            pinguim.porcentagem_peso = 1;
                        }
                        else if (c->tipo == VASO)
                        {
                            pinguim.txt = pinguim.texturas[9];
                            pinguim.vidas--;
                            pinguim.atingido = true;
                            pinguim.momento_queda = SDL_GetTicks();
                            pinguim.sacos = 0;
                            pinguim.porcentagem_peso = 1;
                        }
                        else if (c->tipo == BIGORNA)
                        {
                            pinguim.txt = pinguim.texturas[7];
                            pinguim.vidas--;
                            pinguim.atingido = true;
                            pinguim.momento_queda = SDL_GetTicks();
                            pinguim.sacos = 0;
                            pinguim.porcentagem_peso = 1;
                        }
                        else if (c->tipo == GRAOS)
                        {
                            if (pinguim.sacos < 5)
                            {
                                pinguim.sacos++;
                                pinguim.score += 8;
                                pinguim.porcentagem_peso -= 0.10;
                                pinguim.ultima_entrega = SDL_GetTicks();
                                pinguim.estado_carga = (Estados_pinguim_carga)pinguim.sacos;
                                pinguim.txt = pinguim.texturas[pinguim.sacos];
                                c->ativo = false;
                            }
                            else
                            {
                                pinguim.vidas--;
                                pinguim.atingido = true;
                                pinguim.momento_queda = SDL_GetTicks();
                                pinguim.sacos = 0;
                                pinguim.porcentagem_peso = 1;
                                pinguim.txt = pinguim.texturas[6];
                            }
                        }
                    }
                }

                c = c->prox;
            }

            if (pinguim.sacos >= 1 && SDL_HasIntersection(&pinguim.rect,&plataforma) && (SDL_GetTicks() - pinguim.ultima_entrega) >= pinguim.intervalo_entrega)
                {
                    pinguim.ultima_entrega = SDL_GetTicks();
                    pinguim.sacos--;
                    pinguim.txt = pinguim.texturas[pinguim.sacos];
                    pinguim.porcentagem_peso +=0.10;
                    pinguim.score +=12;
                    if(!pilha_cheia(&pilha_1))
                    {
                        push(renderizador,&pilha_1);
                    }
                    else if (!pilha_cheia(&pilha_2))
                    {
                        push(renderizador,&pilha_2);
                    }
                }
        }

        Carga *c2 = cargas_jogadas.inicio;
        while (c2 != NULL)
        {
            if (c2->ativo && !c2->fade)
            {
                if (SDL_HasIntersection(&chao, &c2->rect))
                {
                    c2->fade = true;
                    c2->tempo_queda = SDL_GetTicks();
                }
            }

            c2 = c2->prox;
        }


        // ============================
        //        DESENHO
        // ============================
        char buffer[32];                            // string que vamos montar
        sprintf(buffer, "LIFE: %d", pinguim.vidas); // monta a string

        sfc_life = TTF_RenderText_Blended(fnt, buffer, clr);
        assert(sfc_life != NULL);
        txt_life = SDL_CreateTextureFromSurface(renderizador, sfc_life);
        assert(txt_life != NULL);

        sprintf(buffer, "SCORE: %d", pinguim.score); // monta a string
        sfc_score = TTF_RenderText_Blended(fnt, buffer, clr);
        assert(sfc_score != NULL);
        txt_score = SDL_CreateTextureFromSurface(renderizador, sfc_score);
        assert(txt_score != NULL);

        SDL_RenderCopy(renderizador, txt_background, NULL, &background);

        SDL_RenderCopy(renderizador, txt_plataforma, NULL, &plataforma);
        draw_pilha(renderizador, &pilha_1);
        draw_pilha(renderizador, &pilha_2);

        draw_cargas(renderizador, &cargas_jogadas);

        SDL_RenderCopy(renderizador, caminhao_txt, NULL, &caminhao);

        SDL_RenderCopy(renderizador, pinguim.txt, NULL, &pinguim.rect);

        SDL_RenderCopy(renderizador, txt_neve, NULL, &neve);

        SDL_RenderCopy(renderizador, txt_life, NULL, &life);
        SDL_RenderCopy(renderizador, txt_score, NULL, &score);

        SDL_RenderPresent(renderizador);
    }

    SDL_ShowCursor(SDL_ENABLE);
    IMG_Quit();
    return 1;
}

#endif
