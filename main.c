#include <stdio.h>
#include <stdlib.h>
#include "lexico.h"
#include "sintatico.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s <arquivo.c>\n", argv[0]);
        return 1;
    }

    FILE *arquivo = fopen(argv[1], "r");
    if (!arquivo)
    {
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

    int choice = 0;
    printf("Imprimir:\n");
    printf("1 - Lexico\n");
    printf("2 - Sintatico\n");
    printf("3 - Ambos\n");
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("== Tokens encontrados no arquivo '%s' ==\n\n", argv[1]);
        analisar_lexico(codigo);
    }
    else if (choice == 2)
    {
        analisar_lexico(codigo);
        printf("\n== Árvore Sintática ==\n\n");
        analisar_sintatico();
    }
    else if (choice == 3)
    {
        printf("== Tokens encontrados no arquivo '%s' ==\n\n", argv[1]);
        analisar_lexico(codigo);
        printf("\n== Árvore Sintática ==\n\n");
        analisar_sintatico();
    }
    else
    {
        free(codigo);
        exit(0);
    }
    free(codigo);
    return 0;
}
