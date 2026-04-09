#ifndef DOJO_H
#define DOJO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>  // necessário para atoi() e sprintf()
#include "../../utils/Aux_Timeout.h"
#include "../../utils/Aux_monitor.h"
#include "../personalizacao/personalizacao.h"
#include "../../texturas/leitura_arquivos.c"
#include "../../texturas/globais.c"

#define NUM_CARTAS_PLAYER 3
#define NUM_CARTAS_POOL 5

static int cartaSelecionada = -1;
static int cartaNPCSelecionada = -1;

typedef enum { GELO, AGUA, FOGO } Elemento;

typedef struct {
    Elemento elemento;
    int pontuacao;
    SDL_Texture* textura;
} Carta;

typedef enum {
    DUEL_SELECIONANDO,
    DUEL_COMPARANDO,
    DUEL_FINALIZADO
} DuelState;

// Sorteio de cartas sem repetição
static inline void sortearCartas(Carta* cartas_pool, Carta* cartas_destino) {
    bool usados[NUM_CARTAS_POOL] = {false};
    for (int i = 0; i < NUM_CARTAS_PLAYER; i++) {
        int idx;
        do { idx = rand() % NUM_CARTAS_POOL; } while (usados[idx]);
        usados[idx] = true;
        cartas_destino[i] = cartas_pool[idx];
    }
}

// Comparar cartas por elemento e desempate por pontuação
static inline int compararCartas(Carta jogador, Carta npc) {
    if (jogador.elemento == npc.elemento) {
        if (jogador.pontuacao > npc.pontuacao) return 1;
        else if (jogador.pontuacao < npc.pontuacao) return -1;
        else return 0;
    }

    // Atributos: gelo > agua > fogo > gelo
    if ((jogador.elemento == GELO && npc.elemento == AGUA) ||
        (jogador.elemento == AGUA && npc.elemento == FOGO) ||
        (jogador.elemento == FOGO && npc.elemento == GELO)) {
        return 1; // jogador vence
    } else {
        return -1; // NPC vence
    }
}

// Remove carta do jogador
static inline void removerCartaJogador(Carta* array, int index, int* tamanho) {
    for (int i = index; i < (*tamanho) - 1; i++) {
        array[i] = array[i + 1];
    }
    (*tamanho)--;
}

// Remove carta do NPC
static inline void removerCartaNPC(Carta* array, int index, int* tamanho) {
    for (int i = index; i < (*tamanho) - 1; i++) {
        array[i] = array[i + 1];
    }
    (*tamanho)--;
}

static inline int RenderDojoScreen(
    SDL_Window *janela,
    SDL_Renderer *renderizador,
    SDL_Event *evento,
    Uint32 *timeout,
    GameState *estadoJogo,
    char dinheiro[10]  // agora é string
) {
    srand(time(NULL));
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    IMG_Init(IMG_INIT_PNG);

    SDL_Rect background = {0, 0, LARGURA, ALTURA};
    SDL_Texture* background_textura = lista_txt.inicio[TEX_FUNDO_DOJO].txt;

    // Pool de cartas
    Carta cartas_pool[NUM_CARTAS_POOL] = {
        {AGUA, 3, lista_txt.inicio[TEX_AGUA_3].txt},
        {FOGO, 4, lista_txt.inicio[TEX_FOGO_4].txt},
        {FOGO, 7, lista_txt.inicio[TEX_FOGO_7].txt},
        {GELO, 5, lista_txt.inicio[TEX_GELO_5].txt},
        {GELO, 6, lista_txt.inicio[TEX_GELO_6].txt}
    };

    // Sorteio das cartas do jogador
    Carta cartasJogador[NUM_CARTAS_PLAYER];
    sortearCartas(cartas_pool, cartasJogador);

    // Sorteio das cartas do NPC
    Carta cartasNPC[NUM_CARTAS_PLAYER];
    sortearCartas(cartas_pool, cartasNPC);

    SDL_Texture* carta_azul_textura = lista_txt.inicio[TEX_CARTA_AZUL].txt;
    SDL_Texture* ganhou_textura = lista_txt.inicio[TEX_GANHOU].txt;
    SDL_Texture* perdeu_textura = lista_txt.inicio[TEX_PERDEU].txt;

    SDL_Rect cartas_player[NUM_CARTAS_PLAYER];
    SDL_Rect cartas_azuis[NUM_CARTAS_PLAYER];

    int card_w = 200, card_h = 200;
    int pos_y = ALTURA - card_h;
    for (int i = 0; i < NUM_CARTAS_PLAYER; i++) {
        cartas_azuis[i].x = i * (card_w / 2);
        cartas_azuis[i].y = pos_y;
        cartas_azuis[i].w = card_w;
        cartas_azuis[i].h = card_h;

        cartas_player[i].x = LARGURA - card_w - i * card_w;
        cartas_player[i].y = pos_y;
        cartas_player[i].w = card_w;
        cartas_player[i].h = card_h;
    }

    SDL_Rect carta_grande = {LARGURA - 350 - 30, 40, 350, 350};
    SDL_Rect cartaNPC_rect = {40, 40, 350, 350};

    DuelState dueloEstado = DUEL_SELECIONANDO;
    int cartasRestantesJogador = NUM_CARTAS_PLAYER;
    int cartasRestantesNPC = NUM_CARTAS_PLAYER;
    int jogadorScore = 0, npcScore = 0;

    while (true) {
        if (AUX_WaitEventTimeout(evento, timeout)) {
            if (evento->type == SDL_MOUSEBUTTONDOWN) {
                int mx = evento->button.x;
                int my = evento->button.y;
                bool clicouNaCarta = false;

                for (int i = 0; i < cartasRestantesJogador; i++) {
                    if(mx >= cartas_player[i].x && mx <= cartas_player[i].x + cartas_player[i].w &&
                       my >= cartas_player[i].y && my <= cartas_player[i].y + cartas_player[i].h) 
                    {
                        cartaSelecionada = i;
                        clicouNaCarta = true;
                        if(cartaNPCSelecionada == -1)
                            cartaNPCSelecionada = rand() % cartasRestantesNPC;
                        break;
                    }
                }
                if(!clicouNaCarta) {
                    cartaSelecionada = -1;
                    cartaNPCSelecionada = -1;
                }
            }

            if (evento->type == SDL_KEYDOWN && evento->key.keysym.sym == SDLK_ESCAPE) {
                *estadoJogo = STATE_JOGANDO;
                IMG_Quit();
                return 1;
            }
            if (evento->type == SDL_QUIT) {
                *estadoJogo = STATE_SAIR;
                IMG_Quit();
                return 0;
            }
        }

        switch (dueloEstado) {
            case DUEL_SELECIONANDO:
                if (cartaSelecionada != -1 && cartaNPCSelecionada != -1)
                    dueloEstado = DUEL_COMPARANDO;
                break;

            case DUEL_COMPARANDO: {
                Carta cJog = cartasJogador[cartaSelecionada];
                Carta cNPC = cartasNPC[cartaNPCSelecionada];
                int resultado = compararCartas(cJog, cNPC);

                // Atualiza pontuação apenas
                if (resultado == 1) jogadorScore += 3;
                else if (resultado == -1) npcScore += 3;
                else { jogadorScore += 1; npcScore += 1; }

                // Render do duelo
                SDL_RenderClear(renderizador);
                SDL_RenderCopy(renderizador, background_textura, NULL, &background);

                for (int i = 0; i < cartasRestantesJogador; i++) {
                    SDL_RenderCopy(renderizador, carta_azul_textura, NULL, &cartas_azuis[i]);
                    SDL_RenderCopy(renderizador, cartasJogador[i].textura, NULL, &cartas_player[i]);
                }
                SDL_RenderCopy(renderizador, cartasNPC[cartaNPCSelecionada].textura, NULL, &cartaNPC_rect);
                SDL_RenderCopy(renderizador, cartasJogador[cartaSelecionada].textura, NULL, &carta_grande);

                SDL_Rect txt_rect = {0,0,150,150};
                if (resultado == 1) {
                    txt_rect.x = carta_grande.x + carta_grande.w/2 - txt_rect.w/2;
                    txt_rect.y = carta_grande.y + carta_grande.h/2 - txt_rect.h/2;
                    SDL_RenderCopy(renderizador, ganhou_textura, NULL, &txt_rect);

                    txt_rect.x = cartaNPC_rect.x + cartaNPC_rect.w/2 - txt_rect.w/2;
                    txt_rect.y = cartaNPC_rect.y + cartaNPC_rect.h/2 - txt_rect.h/2;
                    SDL_RenderCopy(renderizador, perdeu_textura, NULL, &txt_rect);
                } else if (resultado == -1) {
                    txt_rect.x = carta_grande.x + carta_grande.w/2 - txt_rect.w/2;
                    txt_rect.y = carta_grande.y + carta_grande.h/2 - txt_rect.h/2;
                    SDL_RenderCopy(renderizador, perdeu_textura, NULL, &txt_rect);

                    txt_rect.x = cartaNPC_rect.x + cartaNPC_rect.w/2 - txt_rect.w/2;
                    txt_rect.y = cartaNPC_rect.y + cartaNPC_rect.h/2 - txt_rect.h/2;
                    SDL_RenderCopy(renderizador, ganhou_textura, NULL, &txt_rect);
                } else {
                    txt_rect.x = carta_grande.x + carta_grande.w/2 - txt_rect.w/2;
                    txt_rect.y = carta_grande.y + carta_grande.h/2 - txt_rect.h/2;
                    SDL_RenderCopy(renderizador, ganhou_textura, NULL, &txt_rect);

                    txt_rect.x = cartaNPC_rect.x + cartaNPC_rect.w/2 - txt_rect.w/2;
                    txt_rect.y = cartaNPC_rect.y + cartaNPC_rect.h/2 - txt_rect.h/2;
                    SDL_RenderCopy(renderizador, ganhou_textura, NULL, &txt_rect);
                }

                SDL_RenderPresent(renderizador);
                SDL_Delay(3000); 

                removerCartaJogador(cartasJogador, cartaSelecionada, &cartasRestantesJogador);
                removerCartaNPC(cartasNPC, cartaNPCSelecionada, &cartasRestantesNPC);

                cartaSelecionada = -1;
                cartaNPCSelecionada = -1;
                dueloEstado = (cartasRestantesJogador > 0 && cartasRestantesNPC > 0) ? DUEL_SELECIONANDO : DUEL_FINALIZADO;
                break;
            }

            case DUEL_FINALIZADO:
                // Premiação apenas no resultado final
                {
                    int dinheiro_int = atoi(dinheiro); // converte string para int
                    if (jogadorScore > npcScore) dinheiro_int += 100;
                    else if (jogadorScore == npcScore) dinheiro_int += 50;
                    sprintf(dinheiro, "%d", dinheiro_int); // atualiza string
                }

                *estadoJogo = STATE_JOGANDO;
                IMG_Quit();
                return 1;
        }

        SDL_RenderClear(renderizador);
        SDL_RenderCopy(renderizador, background_textura, NULL, &background);
        for (int i = 0; i < cartasRestantesJogador; i++) {
            SDL_RenderCopy(renderizador, carta_azul_textura, NULL, &cartas_azuis[i]);
            SDL_RenderCopy(renderizador, cartasJogador[i].textura, NULL, &cartas_player[i]);
        }
        if(cartaNPCSelecionada != -1)
            SDL_RenderCopy(renderizador, cartasNPC[cartaNPCSelecionada].textura, NULL, &cartaNPC_rect);
        if(cartaSelecionada >= 0)
            SDL_RenderCopy(renderizador, cartasJogador[cartaSelecionada].textura, NULL, &carta_grande);
        SDL_RenderPresent(renderizador);
    }

    IMG_Quit();
    return 1;
}

#endif
