#ifndef BEAN_PILHA
#define BEAN_PILHA

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "globais.c"
#include "enums.c"
#include "carga.c"

typedef struct {
    int limite;
    Carga * data;
    int topo;  // índice do elemento do topo
    int pos_x;
    int pos_y;
} Pilha;

void inicializa_pilha(Pilha *p) {
    p->topo = -1;
    p->limite = 10;
    p->data = malloc(10*sizeof(Carga));
}

// Verifica se a pilha está cheia
int pilha_cheia(Pilha *p) {
    return p->topo == p->limite - 1;
}

int push(SDL_Renderer * renderizador, Pilha *p) {
    if (pilha_cheia(p)) {
        return 0; // falhou
    }
    Carga carga;
    carga.rect = (SDL_Rect){p->pos_x, p->pos_y, LARGURA*0.07, LARGURA*0.07 };
    carga.tipo = GRAOS;
    carga.txt = lista_txt.inicio[TEX_BEAN_SACO].txt;
    p->pos_y -= LARGURA*0.02;
    p->data[++p->topo] = carga;
    return 1;
}

void draw_pilha(SDL_Renderer * renderizador,Pilha * p)
{
    for (int i =0; i<=p->topo; i++)
    {
        SDL_RenderCopy(renderizador, p->data[i].txt, NULL, &p->data[i].rect);
    }
}
#endif