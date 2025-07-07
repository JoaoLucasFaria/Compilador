#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gerador.h"

FILE *saida;
int str_count = 0;

void gerar_codigo_expr(NoSintatico *no);

void gerar_codigo_rec(NoSintatico *no) {
    if (strcmp(no->tipo, "DECLARACAO") == 0) {
        const char *tipo = no->valor;
        for (int i = 0; i < no->num_filhos; i++) {
            NoSintatico *filho = no->filhos[i];
            if (strcmp(filho->tipo, "Nome") == 0) {
                fprintf(saida, "%s: .word 0\n", filho->valor);
            }
            else if (strcmp(filho->tipo, "EXPR") == 0) {
                for (int j = 0; j < filho->num_filhos; j++) {
                    if (strcmp(filho->filhos[j]->tipo, "IDENTIFICADOR") == 0) {
                        fprintf(saida, "%s: .word 0\n", filho->filhos[j]->valor);
                    }
                }
            }
        }
    }
    if (strcmp(no->tipo, "IF") == 0) {
        fprintf(saida, "\n# IF\n");
        fprintf(saida, "lw $t0, %s\n", no->filhos[0]->filhos[0]->valor);
        fprintf(saida, "beqz $t0, endif%d\n", str_count);
        gerar_codigo_expr(no->filhos[1]);
        fprintf(saida, "endif%d:\n", str_count++);
    }
    else if (strcmp(no->tipo, "FOR") == 0) {
        fprintf(saida, "\n# FOR\n");
        fprintf(saida, "li $t2, 0\n");
        fprintf(saida, "sw $t2, %s\n", no->filhos[0]->filhos[1]->valor);
        fprintf(saida, "for_loop%d:\n", str_count);
        fprintf(saida, "lw $t0, %s\n", no->filhos[1]->filhos[0]->valor);
        fprintf(saida, "lw $t1, %s\n", no->filhos[1]->filhos[2]->valor);
        fprintf(saida, "bge $t0, $t1, endfor%d\n", str_count);
        gerar_codigo_expr(no->filhos[3]);
        fprintf(saida, "lw $t0, %s\n", no->filhos[2]->filhos[0]->valor);
        fprintf(saida, "addi $t0, $t0, 1\n");
        fprintf(saida, "sw $t0, %s\n", no->filhos[2]->filhos[0]->valor);
        fprintf(saida, "j for_loop%d\n", str_count);
        fprintf(saida, "endfor%d:\n", str_count++);
    }
    else if (strcmp(no->tipo, "RETURN") == 0) {
        fprintf(saida, "li $v0, 10\nsyscall\n");
    }

    for (int i = 0; i < no->num_filhos; i++) {
        gerar_codigo_rec(no->filhos[i]);
    }
}

void gerar_codigo_expr(NoSintatico *no) {
    if (strcmp(no->tipo, "BLOCO") == 0 || strcmp(no->tipo, "COMANDO") == 0) {
        for (int i = 0; i < no->num_filhos; i++) {
            gerar_codigo_expr(no->filhos[i]);
        }
    }
    else if (strcmp(no->tipo, "EXPR") == 0) {
        for (int i = 0; i < no->num_filhos; i++) {
            if (strcmp(no->filhos[i]->tipo, "STRING") == 0) {
                fprintf(saida, "str%d: .asciiz %s\n", str_count, no->filhos[i]->valor);
                fprintf(saida, "la $a0, str%d\n", str_count);
                fprintf(saida, "li $v0, 4\nsyscall\n");
                str_count++;
            }
        }
    }
}

void gerar_codigo(NoSintatico *raiz, const char *arquivo_saida) {
    saida = fopen(arquivo_saida, "w");
    if (!saida) {
        printf("Erro ao abrir arquivo de saída %s\n", arquivo_saida);
        return;
    }
    fprintf(saida, ".data\n");
    gerar_codigo_rec(raiz);
    fprintf(saida, "\n.text\n.globl main\nmain:\n");

    // Inicializar a árvore principal (reprocessar para instruções)
    gerar_codigo_expr(raiz);

    fprintf(saida, "li $v0, 10\nsyscall\n");
    fclose(saida);
    printf("\n== Código MIPS gerado em '%s' ==\n", arquivo_saida);
}
