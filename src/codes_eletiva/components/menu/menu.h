#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "../../consts/consts.h"
#include "../../utils/Aux_Timeout.h"
#include "../personalizacao/personalizacao.h"
#include "../../utils/objeto.c"
#include "../../texturas/globais.c"
#include "../../texturas/texturaid.c"




static inline int RenderMenuScreen(
    SDL_Window *janela, 
    SDL_Renderer *renderizador, 
    SDL_Event * evento, 
    Uint32 *timeout,
    GameState *estadoJogo  
    )
{

    SDL_SetWindowFullscreen(janela, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_SetWindowPosition(janela, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    IMG_Init(IMG_INIT_PNG);

    int LARGURA, ALTURA;
    obterTamanhoJanela(janela, &LARGURA, &ALTURA);

    SDL_Point mouse;

    Objeto background;
    background.rect = (SDL_Rect){0, 0, LARGURA, ALTURA};
    background.txt = lista_txt.inicio[TEX_BACKGROUND_MENU].txt;



    Objeto logo;
    logo.rect = (SDL_Rect){LARGURA*0.325, ALTURA*0.05, LARGURA*0.40, ALTURA*0.40};
    logo.txt = lista_txt.inicio[TEX_LOGO].txt;

    Objeto jogar;
    jogar.rect = (SDL_Rect){LARGURA*0.25, ALTURA*0.40, LARGURA*0.55, ALTURA*0.20};
    jogar.txt_nao_clicado = lista_txt.inicio[TEX_JOGAR].txt;
    jogar.txt_clicado = lista_txt.inicio[TEX_JOGAR_HOVER].txt;

    Objeto personalizar;
    personalizar.rect = (SDL_Rect){LARGURA*0.25, ALTURA*0.60, LARGURA*0.55, ALTURA*0.20};
    personalizar.txt_nao_clicado = lista_txt.inicio[TEX_PERSONALIZAR].txt;
    personalizar.txt_clicado = lista_txt.inicio[TEX_PERSONALIZAR_HOVER].txt;
    Objeto sair;
    sair.rect = (SDL_Rect){LARGURA*0.25, ALTURA*0.80, LARGURA*0.55, ALTURA*0.20};
    sair.txt_nao_clicado = lista_txt.inicio[TEX_SAIR].txt;
    sair.txt_clicado = lista_txt.inicio[TEX_SAIR_HOVER].txt;
  
    while(true){
        SDL_GetMouseState(&mouse.x, &mouse.y);
        if (SDL_PointInRect(&mouse,&jogar.rect))
        {
            jogar.txt = jogar.txt_clicado;
        }
        else
        {
            jogar.txt = jogar.txt_nao_clicado;
        }

        if (SDL_PointInRect(&mouse,&personalizar.rect))
        {
            personalizar.txt = personalizar.txt_clicado;
        }
        else
        {
            personalizar.txt = personalizar.txt_nao_clicado;
        }

        if (SDL_PointInRect(&mouse,&sair.rect))
        {
            sair.txt = sair.txt_clicado;
        }
        else
        {
            sair.txt = sair.txt_nao_clicado;
        }

        if(AUX_WaitEventTimeout(evento, timeout)){
            switch(evento->type) {
                case SDL_MOUSEBUTTONDOWN:
                    if(evento->button.button == SDL_BUTTON_LEFT){
                        if(SDL_PointInRect(&mouse,&jogar.rect)){
                            //iniciar jogo
                            *estadoJogo = STATE_JOGANDO;
                            IMG_Quit();
                            return 1;
                        }
                        else if(SDL_PointInRect(&mouse,&personalizar.rect)){
                            //abrir tela de personalizar personagem
                            *estadoJogo = STATE_PERSONALIZACAO;
                            IMG_Quit();
                            return 1;
                        }
                        else if(SDL_PointInRect(&mouse,&sair.rect)){
                            *estadoJogo = STATE_SAIR;
                            IMG_Quit();
                            return 0;
                        }
                    }
                    break;
                case SDL_QUIT:
                    *estadoJogo = STATE_SAIR;
                    libera_lista_texturas(&lista_txt);
                    IMG_Quit();
                    return 0;
                    break;
            }
        }

        SDL_RenderClear(renderizador);
        SDL_SetRenderDrawColor(renderizador, 255, 0, 0, 255);

        //Renderiza botões com texturas atualizadas
        SDL_RenderCopy(renderizador, background.txt, NULL, &background.rect);
        SDL_RenderCopy(renderizador, logo.txt, NULL, &logo.rect);
        SDL_RenderCopy(renderizador, jogar.txt, NULL, &jogar.rect);
        SDL_RenderCopy(renderizador, personalizar.txt, NULL, &personalizar.rect);
        SDL_RenderCopy(renderizador, sair.txt, NULL, &sair.rect);
        SDL_RenderPresent(renderizador);
    }
}

#endif
