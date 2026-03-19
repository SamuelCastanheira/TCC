#define PERSONALIZACAO_IMPLEMENTACAO
#include <SDL2/SDL.h>
#include "./components/game_screen/game_screen.h"
#include "texturas/leitura_arquivos.c"
#include "texturas/globais.c"
#include "texturas/texturaid.c"


int main(int args, char* argc[]) {
    SDL_Event *evento;

      // Dimensões da tela
    int LARGURA = LARGURA_PADRAO;
    int ALTURA = ALTURA_PADRAO;

    // Inicialização SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Erro ao iniciar SDL: %s", SDL_GetError());
        return -1;
    }
  
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
    SDL_Log("%d,%d",ALTURA, LARGURA);
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

     while (carregando)
    {
        if (lista_txt.texturas_lidas_sucesso < lista_txt.qnt_texturas)
        {
            le_prox_textura(&lista_txt,renderizador);
        }
        else
        {
            carregando = false;
        }
    }

    bool editando = true;
    
    SDL_Texture *pinguim_img = lista_txt.inicio[TEX_PINGUIM_AMARELO].txt;
    corSelecionada = AMARELO;

    Objeto background;
    background.rect = (SDL_Rect){0, 0, LARGURA, ALTURA};
    background.txt = lista_txt.inicio[TEX_BACKGROUND_PERSONALIZAR].txt;

    Objeto quadro;
    quadro.rect = (SDL_Rect){LARGURA*0.55, ALTURA*0.20, LARGURA*0.45, ALTURA*0.56};
    quadro.txt = lista_txt.inicio[TEX_QUADRO].txt;

    
    int tam_botao_x = LARGURA*0.292, tam_botao_y = ALTURA*0.156;
    
    SDL_Rect pinguim= { LARGURA*0.146, ALTURA*0.13, ALTURA*0.52, ALTURA*0.52};
    Objeto escolha;

    escolha.rect = (SDL_Rect){LARGURA*0.60, ALTURA*0.20, LARGURA*0.35, ALTURA*0.05};
    escolha.txt = lista_txt.inicio[TEX_ESCOLHA].txt;
    SDL_Rect voltar= {LARGURA*0.142, ALTURA*0.75, LARGURA*0.292, ALTURA*0.156};

    SDL_Rect rectCores[TOTAL];

    int startX = LARGURA*0.60;
    int startY = ALTURA*0.30;
    int espaco = 10;
    int largura = 0.065*LARGURA;
    int altura  = 0.117*ALTURA;

    for (int i = 0; i < TOTAL; i++) {
        rectCores[i].x = startX + (i % 5) * (largura + espaco);
        rectCores[i].y = startY + (i / 5) * (altura + espaco);
        rectCores[i].w = largura;
        rectCores[i].h = altura;
    }

    SDL_Point mouse;

    SDL_GetMouseState(&mouse.x, &mouse.y);

    SDL_SetRenderDrawColor(renderizador,225, 225, 255, 0);
    SDL_RenderClear(renderizador);
    SDL_RenderCopy(renderizador, background.txt, NULL, &background.rect);
    SDL_RenderCopy(renderizador, quadro.txt, NULL, &quadro.rect);
    SDL_RenderCopy(renderizador, escolha.txt, NULL , &escolha.rect);
    SDL_RenderCopy(renderizador, SDL_PointInRect(&mouse, &voltar) ?
    lista_txt.inicio[TEX_ELEMENTO_VOLTAR_HOVER].txt : lista_txt.inicio[TEX_ELEMENTO_VOLTAR].txt,
        NULL, &voltar);

    SDL_RenderCopy(renderizador, pinguim_img, NULL , &pinguim);
    for (int i = 0; i < TOTAL; i++) {
        SDL_RenderCopy(renderizador, lista_txt.inicio[i + TEX_COR_AMARELO].txt, NULL , &rectCores[i]);
    }
    SDL_RenderPresent(renderizador);
    SDL_Delay(5000);

      // Libera recursos
    SDL_DestroyTexture(pinguim_img);
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(janela);
    SDL_Quit();

    return 0;
}