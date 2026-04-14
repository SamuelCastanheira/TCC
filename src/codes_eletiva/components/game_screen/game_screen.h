#ifndef CENTER_H
#define CENTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../../utils/Aux_Timeout.h"
#include "../../utils/Aux_monitor.h"
#include "../../utils/Aux_movimentacao.h"
#include "../../consts/consts.h"
#include "../personalizacao/personalizacao.h" 
#include "../../utils/objeto.c"


static inline int RenderGameScreen(SDL_Window *janela, SDL_Renderer *renderizador, SDL_Event * evento, Uint32 *timeout, GameState *estadoJogo, char dinheiro[10]) {
    int LARGURA, ALTURA;
    int mouse_x, mouse_y;
    char caminho[100];

    obterTamanhoJanela(janela, &LARGURA, &ALTURA);
    SDL_GetMouseState(&mouse_x, &mouse_y);

    TTF_Init();
    TTF_Font *fonte = TTF_OpenFont("../../assets/fonts/Lovelo/lovelo_black.otf", 50);
    SDL_Color corTexto = {255, 255, 0, 255}; // Amarelo

    SDL_Surface* textoSurface = TTF_RenderText_Blended(fonte, dinheiro, corTexto);
    SDL_Texture* textoTexture = SDL_CreateTextureFromSurface(renderizador, textoSurface);

    SDL_Rect textoRect;
    textoRect.w = 100;
    textoRect.h = 100;
    textoRect.x = 100; 
    textoRect.y = 0;  

    SDL_FreeSurface(textoSurface);



    IMG_Init(IMG_INIT_PNG);
    SDL_Texture *centroIMG = IMG_LoadTexture(renderizador, "../../assets/imgs/centro.png");    
    SDL_Texture *textura_pinguim[TOTAL_DIRECOES];
    SDL_Texture *textura_atual;

    // Moeda
    SDL_Rect moeda = {0, 0, 50, 50};
    SDL_Texture* moeda_textura = lista_txt.inicio[TEX_MOEDA].txt;

    for (int i = 0; i < TOTAL_DIRECOES; i++) {
        sprintf(caminho, "../../assets/imgs/pinguim/%s/%s.png", nomes_cores[corSelecionada], nomes_direcoes[i]);
        textura_pinguim[i] = IMG_LoadTexture(renderizador, caminho);
    }
    textura_atual = textura_pinguim[SUL];
  
    
    SDL_Rect pinguimRect = {LARGURA/2, ALTURA/2, 100, 160};
    SDL_Rect centroRect = {0, 0, LARGURA, ALTURA};
    
    Objeto coffe;
    coffe.rect = (SDL_Rect){(LARGURA*0.10), (ALTURA*0.20), LARGURA*0.40, ALTURA*0.40};
    coffe.txt_nao_clicado = IMG_LoadTexture(renderizador, "../../assets/imgs/coffe.png");
    coffe.txt_clicado = IMG_LoadTexture(renderizador, "../../assets/imgs/coffe_hover.png");

    Objeto pega_puffle;
    pega_puffle.rect = (SDL_Rect){(LARGURA*0.40), (ALTURA*0.20), LARGURA*0.3, ALTURA*0.3};
    pega_puffle.txt_nao_clicado = IMG_LoadTexture(renderizador, "../../assets/imgs/pet_shop.png");
    pega_puffle.txt_clicado = IMG_LoadTexture(renderizador, "../../assets/imgs/pet_shop_hover.png");

    Objeto dojo;
    dojo.rect = (SDL_Rect){(LARGURA*0.60), (ALTURA*0.22), LARGURA*0.3, ALTURA*0.3};
    dojo.txt_nao_clicado = lista_txt.inicio[TEX_CENTRO_DOJO].txt;
    dojo.txt_clicado = lista_txt.inicio[TEX_CENTRO_DOJO_HOVER].txt;

    float escalar_velocidade = 1, velocidade_x = 0, velocidade_y = 0;
    SDL_Point_Float pinguim_posicao = {(LARGURA/2.0f), (ALTURA/2.0f)};
    SDL_Point_Float destino = {(LARGURA/2.0f), (ALTURA/2.0f)};
    float distancia = 0, direcao_rad = 0, dx = 0, dy = 0;

    while (true) 
    {
        SDL_Point mouse;
        
        if (AUX_WaitEventTimeout(evento, timeout)) {
            if (evento->type == SDL_KEYDOWN) {
                if (evento->key.keysym.sym == SDLK_ESCAPE) {
                    *estadoJogo = STATE_MENU;

                    IMG_Quit();
                    return 1;
                }
            } else if(evento->type == SDL_QUIT) {
                *estadoJogo = STATE_SAIR;

                IMG_Quit();
                return 0;
            } else if(evento->type == SDL_MOUSEBUTTONDOWN) {
                //define destino do pinguim
                destino.x = (float)evento->button.x - (pinguimRect.w/2);
                destino.y = (float)evento->button.y - (2*pinguimRect.h/3);

                IniciaMovimentacao(
                    &pinguim_posicao.x, 
                    &pinguim_posicao.y, 
                    destino.x, 
                    destino.y, 
                    &distancia,
                    &direcao_rad,
                    &velocidade_x, 
                    &velocidade_y,
                    escalar_velocidade
                );
                int direcao = DefineDirecaoCardinal(direcao_rad);

                textura_atual = textura_pinguim[direcao];
            }
        }

        SDL_GetMouseState(&mouse.x,&mouse.y);

        if (SDL_PointInRect(&mouse,&coffe.rect))
        {
            coffe.txt = coffe.txt_clicado;
        }
        else
        {
           coffe.txt = coffe.txt_nao_clicado; 
        }


        if (SDL_PointInRect(&mouse,&pega_puffle.rect))
        {
            pega_puffle.txt = pega_puffle.txt_clicado;
        }
        else
        {
           pega_puffle.txt = pega_puffle.txt_nao_clicado; 
        }

         if (SDL_PointInRect(&mouse,&dojo.rect))
        {
            dojo.txt = dojo.txt_clicado;
        }
        else
        {
           dojo.txt = dojo.txt_nao_clicado; 
        }

          if (
            SDL_PointInRect(
                &(SDL_Point){(int)pinguim_posicao.x + (pinguimRect.w/2), (int)pinguim_posicao.y + (2*pinguimRect.h/3)}, 
                &coffe.rect
            )
        ) {
            *estadoJogo = STATE_BEANS_COUNTERS;
            return 1;
        }    

        if (
            SDL_PointInRect(
                &(SDL_Point){(int)pinguim_posicao.x + (pinguimRect.w/2), (int)pinguim_posicao.y + (2*pinguimRect.h/3)}, 
                &pega_puffle.rect
            )
        ) {
            *estadoJogo = STATE_PEGA_PUFFLE;
            return 1;
        }

        if (
            SDL_PointInRect(
                &(SDL_Point){(int)pinguim_posicao.x + (pinguimRect.w/2), (int)pinguim_posicao.y + (2*pinguimRect.h/3)}, 
                &dojo.rect
            )
        ) {
            *estadoJogo = STATE_DOJO;
            return 1;
        }


        if (distancia > 0.5f) {
            SDL_Point nova_posicao = AtualizaPosicao(
                &pinguim_posicao.x, 
                &pinguim_posicao.y, 
                destino.x, 
                destino.y, 
                &velocidade_x, 
                &velocidade_y, 
                &distancia
            );

            pinguimRect.x = nova_posicao.x;
            pinguimRect.y = nova_posicao.y;
        }

        
        //SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 0);
        SDL_RenderClear(renderizador);
        SDL_RenderCopy(renderizador, centroIMG, NULL, &centroRect);

        SDL_RenderCopy(renderizador, pega_puffle.txt, NULL, &pega_puffle.rect);

        SDL_RenderCopy(renderizador, dojo.txt, NULL, &dojo.rect);
        //SDL_SetRenderDrawColor(renderizador, 255, 0, 0, 100);
         SDL_RenderCopy(renderizador, coffe.txt, NULL, &coffe.rect);
        //SDL_SetRenderDrawColor(renderizador, 0, 0, 255, 100);
        
        SDL_RenderCopy(renderizador, textura_atual, NULL, &pinguimRect);
        
        // Dinheiro
        SDL_RenderCopy(renderizador, moeda_textura, NULL, &moeda); 
        SDL_RenderCopy(renderizador, textoTexture, NULL, &textoRect);
        SDL_RenderPresent(renderizador);
    }

    IMG_Quit();
    return 1;
}

#endif