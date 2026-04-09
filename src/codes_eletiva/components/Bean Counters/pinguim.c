#ifndef BEAN_PINGUIM
#define BEAN_PINGUIM

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "globais.c"
#include "enums.c"

typedef struct 
{
    SDL_Texture  * txt;
    SDL_Rect rect;
    Estados_pinguim_carga estado_carga;
    Estados_pinguim_movimento estado_movimento;
    double velocidade;
    double velocidade_inicial;
    double posx;
    int incremento;
    int destino;
    Uint32 now;
    Uint32 last;
    int sacos;
    SDL_Texture ** texturas;
    double porcentagem_peso;
    Uint32 ultima_entrega;
    int intervalo_entrega;
    int score;
    int vidas;
    bool atingido;
    Uint32 momento_queda;
    int posicao_inicial;
   

} Pinguim;


void inicializa_pinguim(SDL_Renderer * renderizador, Pinguim * pinguim)
{
    int comprimento_andavel = limite_dir - limite_esq;
    pinguim->posicao_inicial = limite_esq + comprimento_andavel / 2;
    printf("%d\n",ALTURA);
    printf("%d\n",LARGURA);

    pinguim->score = 0;
    pinguim->vidas = 3;
    pinguim->atingido = false;
    
    pinguim->rect = (SDL_Rect){ pinguim->posicao_inicial, ALTURA*0.72, LARGURA*0.12, LARGURA*0.12};
    pinguim->estado_movimento = PARADO;
    pinguim->estado_carga = NAO_CARREGA;
    pinguim->velocidade_inicial = LARGURA/50;
    pinguim->velocidade = 0;
    pinguim->posx = pinguim->rect.x;
    pinguim->destino = pinguim->posx;
    pinguim->incremento = 0;
    pinguim->last = SDL_GetTicks();
    pinguim->sacos = 0;
    pinguim->porcentagem_peso = 1;
    pinguim->ultima_entrega = 0;
    pinguim->intervalo_entrega = 250;

    pinguim->texturas = malloc(10*sizeof(SDL_Texture *));
    pinguim->texturas[0] = lista_txt.inicio[TEX_BEAN_PINGUIM0].txt;
    pinguim->texturas[1] = lista_txt.inicio[TEX_BEAN_PINGUIM1].txt;
    pinguim->texturas[2] = lista_txt.inicio[TEX_BEAN_PINGUIM2].txt;
    pinguim->texturas[3] = lista_txt.inicio[TEX_BEAN_PINGUIM3].txt;
    pinguim->texturas[4] = lista_txt.inicio[TEX_BEAN_PINGUIM4].txt;
    pinguim->texturas[5] = lista_txt.inicio[TEX_BEAN_PINGUIM5].txt;
    pinguim->texturas[6] = lista_txt.inicio[TEX_BEAN_PINGUIM6].txt;
    pinguim->texturas[7] = lista_txt.inicio[TEX_BEAN_PINGUIM_BIGORNA].txt;
    pinguim->texturas[8] = lista_txt.inicio[TEX_BEAN_PINGUIM_PEIXE].txt;
    pinguim->texturas[9] = lista_txt.inicio[TEX_BEAN_PINGUIM_VASO].txt;


    pinguim->txt = pinguim->texturas[0];

}

void calcula_movimento_pinguim(Pinguim *pinguim)
{
    if (pinguim->atingido)
        return;

    const Uint8* teclas = SDL_GetKeyboardState(NULL);

    // MOVER ESQUERDA
    if (teclas[SDL_SCANCODE_LEFT])
    {
        int destino_aux = (int)pinguim->rect.x - 
                          (pinguim->velocidade_inicial * pinguim->porcentagem_peso);

        if (destino_aux > limite_esq)
        {
            pinguim->destino = destino_aux;
            pinguim->estado_movimento = MOVENDO;
            pinguim->rect.x = pinguim->destino;
        }
    }

    // MOVER DIREITA
    if (teclas[SDL_SCANCODE_RIGHT])
    {
        int destino_aux = (int)pinguim->rect.x + 
                          (pinguim->velocidade_inicial * pinguim->porcentagem_peso);

        if (destino_aux + pinguim->rect.w < limite_dir)
        {
            pinguim->destino = destino_aux;
            pinguim->estado_movimento = MOVENDO;
            pinguim->rect.x = pinguim->destino;
        }
    }
}

#endif