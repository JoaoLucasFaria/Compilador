#include <stdio.h>
#include <stdlib.h>
#include "lexico.h"
#include "sintatico.h"
#include "semantico.h"
#include "gerador.h"

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
        printf("\n== Arvore Sintatica ==\n\n");
        NoSintatico *raiz = analisar_sintatico();
        analisar_semantico(raiz);
        gerar_codigo_mips(raiz, "saida_mips.asm");

    }

    return 0;
}
