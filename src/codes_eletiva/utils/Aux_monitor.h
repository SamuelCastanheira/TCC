#ifndef AUX_MONITOR_H
#define AUX_MONITOR_H

#include <SDL2/SDL.h>


static inline void obterTamanhoMonitor(int* largura, int* altura) {
    SDL_DisplayMode displayMode;

    if (SDL_GetDesktopDisplayMode(0, &displayMode) == 0) {
        *largura = displayMode.w;
        *altura = displayMode.h;
    } else {
        // Fallback se não conseguir obter
        *largura = 1920;
        *altura = 1080;
    }
}

static inline void obterTamanhoJanela(SDL_Window* janela, int* largura, int* altura) {
    SDL_GetWindowSize(janela, largura, altura);
}

static inline void obterInfoMonitor(int indice_monitor, int* largura, int* altura, int* refresh_rate) {
    SDL_DisplayMode displayMode;
    if (SDL_GetDesktopDisplayMode(indice_monitor, &displayMode) == 0) {
        *largura = displayMode.w;
        *altura = displayMode.h;
        *refresh_rate = displayMode.refresh_rate;
    } else {
        // Fallback
        *largura = 1920;
        *altura = 1080;
        *refresh_rate = 60;
    }
}

#endif