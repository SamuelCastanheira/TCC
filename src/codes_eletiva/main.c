#define PERSONALIZACAO_IMPLEMENTACAO
#include <SDL2/SDL.h>
#include "./consts/consts.h"
#include "./components/loading/loading.h"
#include "./components/menu/menu.h"
#include "./components/personalizacao/personalizacao.h"
#include "./components/sled_racing/sled_racing.h"
#include "./components/minigames/pega_puffle.h"
#include "./components/game_screen/game_screen.h"
#include "./components/Bean Counters/bean_counters.h"
#include "./components/dojo/dojo.h"
#include "texturas/globais.c"
#include "texturas/texturaid.c"
#include "./components/dojo/dojo.h"

int main(int args, char* argc[]) {
    SDL_Event evento;
    char dinheiro[10] = "0";

    // Inicialização SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Erro ao iniciar SDL: %s", SDL_GetError());
        return -1;
    }

    // Dimensões da tela
    int LARGURA = LARGURA_PADRAO;
    int ALTURA = ALTURA_PADRAO;
    obterTamanhoMonitor(&LARGURA, &ALTURA);

    // Criação da janela e renderizador
    SDL_Window *janela = SDL_CreateWindow(
        "Ilha Geladeira",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        LARGURA,
        ALTURA,
        SDL_WINDOW_SHOWN
    );

    if (!janela) {
        SDL_Log("Erro ao criar janela: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *renderizador = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);
    if (!renderizador) {
        SDL_Log("Erro ao criar renderizador: %s", SDL_GetError());
        SDL_DestroyWindow(janela);
        SDL_Quit();
        return -1;
    }

    // Estado inicial
    GameState estadoJogo = STATE_LOADING;
    // Loop principal da FSM
    while (rodando) {
                
        switch (estadoJogo) {
            case STATE_LOADING:
                RenderLoadingScreen(janela, renderizador, &evento, &timeout, &estadoJogo);
                break;

            case STATE_MENU:
                RenderMenuScreen(janela, renderizador, &evento, &timeout, &estadoJogo);
                break;

            case STATE_PERSONALIZACAO:
                RenderPersonalizacaoScreen(janela, renderizador, &evento, &timeout, &estadoJogo);
                break;
            
            case STATE_JOGANDO:
                RenderGameScreen(janela, renderizador, &evento, &timeout, &estadoJogo, dinheiro);
                break;

            case STATE_BEANS_COUNTERS:
               RenderBeanCountersScreen(janela, renderizador, &evento, &timeout, &estadoJogo);
	      	break;

            case STATE_PEGA_PUFFLE:
                RenderPegaPuffleScreen(janela, renderizador, &evento, &timeout, &estadoJogo);
                break;

            case STATE_DOJO:
                RenderDojoScreen(janela, renderizador, &evento, &timeout, &estadoJogo,dinheiro);
                break;

            case STATE_SAIR:
                rodando = false;
                break;          

            default:
                rodando = false;
                break;
        }
    }

    // Libera recursos
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(janela);
    SDL_Quit();

    return 0;
}