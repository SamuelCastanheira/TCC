#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.c"
#include "lista_txt.c"

TexturaInfo* carregar_texturas(const char *arquivo, int *quantidade) {
    FILE *f = fopen(arquivo, "r");
    if (!f) {
        printf("Erro ao abrir %s\n", arquivo);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    rewind(f);

    char *buffer = malloc(tamanho + 1);
    fread(buffer, 1, tamanho, f);
    buffer[tamanho] = '\0';
    fclose(f);

    cJSON *json = cJSON_Parse(buffer);
    free(buffer);

    if (!json) {
        printf("Erro no JSON\n");
        return NULL;
    }

    cJSON *lista = cJSON_GetObjectItem(json, "texturas");
    if (!cJSON_IsArray(lista)) {
        printf("Campo 'texturas' não é array\n");
        cJSON_Delete(json);
        return NULL;
    }

    int n = cJSON_GetArraySize(lista);
    *quantidade = n;

    TexturaInfo *vetor = malloc(sizeof(TexturaInfo) * n);

    for (int i = 0; i < n; i++) {
        cJSON *item = cJSON_GetArrayItem(lista, i);
        
        cJSON *nome = cJSON_GetObjectItem(item, "nome");
        cJSON *caminho = cJSON_GetObjectItem(item, "caminho");

        strcpy(vetor[i].nome, nome->valuestring);
        strcpy(vetor[i].caminho, caminho->valuestring);
    }

    cJSON_Delete(json);
    return vetor;
}

#endif