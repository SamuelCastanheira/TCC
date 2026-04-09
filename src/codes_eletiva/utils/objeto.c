#ifndef OBJETO
#define OBJETO

#include <SDL2/SDL.h>

typedef struct 
{
    SDL_Rect rect;
    SDL_Texture * txt;
    SDL_Texture * txt_nao_clicado;
    SDL_Texture * txt_clicado;

} Objeto;


#endif