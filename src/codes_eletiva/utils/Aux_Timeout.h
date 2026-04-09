#ifndef AUX_TIMEOUT_H
#define AUX_TIMEOUT_H

#include <SDL2/SDL.h>

static inline int AUX_WaitEventTimeout(SDL_Event *evt, Uint32 *ms) {
    if (ms == NULL) {
        return 0; // segurança
    }

    Uint32 start = SDL_GetTicks();
    int result = SDL_WaitEventTimeout(evt, *ms);
    Uint32 end = SDL_GetTicks();
    Uint32 elapsed = end - start;

    if (elapsed >= *ms) {
        *ms = 0;
    } else {
        *ms -= elapsed;
    }

    return result;
}



#endif
