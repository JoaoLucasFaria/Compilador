#include <stdio.h>
#include <stdlib.h>
#include "lexico.h"
#include "sintatico.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo_fonte.c>\n", argv[0]);
        return 1;
    }

    char *arquivo = argv[1];
    analisar_lexico(arquivo);

    int opcao;
    printf("Imprimir:\n");
    printf("1 - Lexico\n");
    printf("2 - Sintatico\n");
    printf("3 - Ambos\n");
    scanf("%d", &opcao);

    if (opcao == 1 || opcao == 3) {
        printf("== Tokens encontrados no arquivo '%s' ==\n\n", arquivo);
        for (int i = 0; i < num_tokens; i++) {
            printf("l%d.c%d %s: %s\n", tokens[i].linha, tokens[i].coluna, tokens[i].tipo, tokens[i].valor);
        }
    }

    if (opcao == 2 || opcao == 3) {
        printf("\n== \u00c1rvore Sint\u00e1tica ==\n\n");
        analisar_sintatico();
    }

    return 0;
}
