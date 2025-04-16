#include <stdio.h>
#include <stdlib.h>
#include "lexico.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <arquivo.c>\n", argv[0]);
        return 1;
    }

    FILE *arquivo = fopen(argv[1], "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    char *codigo = malloc(tamanho + 1);
    fread(codigo, 1, tamanho, arquivo);
    codigo[tamanho] = '\0';
    fclose(arquivo);

    printf("== Tokens encontrados no arquivo '%s' ==\n\n", argv[1]);
    analisar_lexico(codigo);
    free(codigo);
    return 0;
}
