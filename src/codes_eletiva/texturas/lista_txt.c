#ifndef TXT_LISTA
#define TXT_LISTA

#include <SDL2/SDL.h>
#include <assert.h>

typedef struct {
    char nome[64];
    char caminho[256];
    SDL_Texture * txt;
} TexturaInfo;

typedef struct
{
    TexturaInfo  * inicio;
    int qnt_texturas;
    int texturas_lidas_sucesso;
    double porcentagem_txt_lidas;
} ListaTexturas;

void inicializa_lista_textura(ListaTexturas *lista,TexturaInfo * infos, int qnt)
{
    lista->inicio = infos;
    lista->qnt_texturas = qnt;
    lista->porcentagem_txt_lidas = 0;
    lista->texturas_lidas_sucesso = 0;
}

void le_prox_textura(ListaTexturas *lista, SDL_Renderer *renderizador)
{
    if (lista->texturas_lidas_sucesso >= lista->qnt_texturas)
        return;

    // --- 1. Pega o caminho relativo vindo do JSON ---
    const char *relativo = lista->inicio[lista->texturas_lidas_sucesso].caminho;

    // --- 2. Pega o caminho base do executável (SDL garante válido em Windows/Linux/macOS) ---
    char *base = SDL_GetBasePath();

    // --- 3. Monta o caminho completo ---
    char caminho_completo[512];
    snprintf(caminho_completo, sizeof(caminho_completo), "%s%s", base, relativo);
    printf("%s\n",caminho_completo);
    // SDL_GetBasePath retorna memória que precisa ser liberada
    SDL_free(base);

    // --- 4. Carrega textura com caminho completo ---
    lista->inicio[lista->texturas_lidas_sucesso].txt =
        IMG_LoadTexture(renderizador, caminho_completo);

    // Verificação de erro
    assert(lista->inicio[lista->texturas_lidas_sucesso].txt != NULL);

    // --- 5. Atualiza dados da lista ---
    lista->texturas_lidas_sucesso++;
    lista->porcentagem_txt_lidas =
        lista->texturas_lidas_sucesso / (double)lista->qnt_texturas;
}

void libera_lista_texturas(ListaTexturas *lista)
{
    TexturaInfo *c = lista->inicio;
    free(lista);
}

#endif
