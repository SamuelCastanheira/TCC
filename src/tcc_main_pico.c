#include "pico.h"
#include "../check.h"



int main(int args, char* argc[]) {

    pico_init(1);

    pico_set_window(NULL, 1, NULL);
    Pico_Abs_Dim window;
    pico_get_window(NULL, NULL, &window);
    Pico_Rel_Dim dim = { '!', {window.w, window.h}, NULL };
    pico_set_view(-1, &dim, NULL, NULL, NULL, NULL, NULL, NULL);
    
    pico_layer_empty("ui", (Pico_Abs_Dim){window.w, window.h});
    pico_layer_empty("quadro_cores", (Pico_Abs_Dim){window.w, window.h});
    
    pico_set_layer("ui");
    Pico_Rel_Rect background = { '%', {0, 0, 1,1}, PICO_ANCHOR_NW, NULL };
    Pico_Rel_Rect pinguim = { '%', {0.10, 0.10, 0.4,0.45}, PICO_ANCHOR_NW, NULL };
    Pico_Rel_Rect voltar = { '%', {0.142, 0.75, 0.292, 0.156}, PICO_ANCHOR_NW, NULL };
    Pico_Rel_Rect quadro = { '%', {0.55,0.22,0.45,0.56}, PICO_ANCHOR_NW, NULL };
    pico_output_clear();
    pico_output_draw_image("imgs/background_personalizar.png", &background);
    pico_output_draw_image("imgs/personalizar/pinguim_amarelo.png", &pinguim);
    pico_output_draw_image("imgs/botoes/b_voltar.png", &voltar);
    pico_output_draw_text("Escolha sua cor: ", &(Pico_Rel_Rect){'%', {0.60, 0.20, 0.38, 0.1}, PICO_ANCHOR_NW, NULL});
    pico_output_draw_image("imgs/personalizar/quadro.png", &quadro);

    //pico_set_layer("quadro_cores");
    //Pico_Rel_Rect quadro = { '%', {0, 0, 1,1}, PICO_ANCHOR_NW, NULL };
    //pico_set_color_clear((Pico_Color){0x00,0x00,0x00});
    //pico_set_alpha(0);
    //pico_output_clear();
    //pico_set_alpha(255);
    //pico_output_draw_image("imgs/personalizar/quadro.png", &quadro);
    
    pico_set_layer(NULL);
    pico_output_clear();
    pico_output_draw_layer("ui", &(Pico_Rel_Rect){ '%', {0, 0, 1, 1}, PICO_ANCHOR_NW, NULL });
    //pico_output_draw_layer("quadro_cores",  &(Pico_Rel_Rect){ '%', {0.55,0.22,0.45,0.56}, PICO_ANCHOR_NW, NULL });

    pico_input_delay(5000);
    pico_init(0);

    return 0;
}