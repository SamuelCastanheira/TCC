#include "pico.h"
#include "../check.h"

int CORES = 15;

int main(int args, char* argc[]) {

    pico_init(1);

    Pico_Rel_Dim phy = {'!', {1280, 720}};
    pico_set_window("Personalização",-1, NULL);
    pico_set_dim(&phy);
    pico_set_expert(1,60);
    
    Pico_Rel_Rect background = { '%', {0.5, 0.5, 1,1}, PICO_ANCHOR_C, NULL };
    Pico_Rel_Rect pinguim = { '%', {0.3, 0.35, 0.3, 0}, PICO_ANCHOR_C, NULL };
    Pico_Rel_Rect voltar = { '%', {0.3, 0.8, 0.3, 0}, PICO_ANCHOR_C, NULL };
    Pico_Rel_Rect texto_escolha = {'%', {0.60, 0.20, 0.38, 0.1}, PICO_ANCHOR_NW, NULL};

    pico_output_clear();
    pico_output_draw_image("../../imgs/background_personalizar.png", &background);
    pico_output_draw_image("../../imgs/personalizar/pinguim_amarelo.png", &pinguim);
    pico_output_draw_image("../../imgs/botoes/b_voltar.png", &voltar);
    pico_output_draw_text("Escolha sua cor: ", &texto_escolha);
   
    pico_layer_empty("quadro", (Pico_Abs_Dim){600, 400});
    pico_set_layer("quadro");
    pico_set_view(-1,&(Pico_Rel_Dim){'#',{7,5}},&(Pico_Abs_Dim){85,80}, NULL, NULL,NULL,NULL,NULL);
    Pico_Rel_Rect quadro_gelo = {'#', {1, 1, 7, 5} , PICO_ANCHOR_NW, NULL};
    pico_set_color_clear_alpha((Pico_Color_A){0x00,0x00,0x00,0x00});
    pico_output_clear();
    pico_output_draw_image("../../imgs/personalizar/quadro.png",
                            &quadro_gelo);

    char path[100];
    char base[] = "../../imgs/personalizar"; 

    for (int i = 0; i < CORES; i++) {
            snprintf(path, sizeof(path), "%s/%d.png", base, i);

            pico_output_draw_image(
            path,
            &(Pico_Rel_Rect){
                '#',
                {
                (i % 5) + 2,
                2 + (i / 5),
                1,
                1
                },
                PICO_ANCHOR_C,
                NULL
            }
        );  
    }
    
    pico_set_layer(NULL);
    pico_output_draw_layer("quadro", &(Pico_Rel_Rect){ '%', {0.55, 0.25, 0, 0}, PICO_ANCHOR_NW, NULL });
    pico_output_present();
    pico_input_delay(5000);
    pico_init(0);

    return 0;
}