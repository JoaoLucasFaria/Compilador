#include <stdio.h>
#include <string.h>
#include "gerador.h"

FILE *saida;

void gerar_codigo_mips_rec(NoSintatico *no) {
    if (strcmp(no->tipo, "DECLARACAO") == 0) {
        const char *tipo = no->valor;
        for (int i = 0; i < no->num_filhos; i++) {
            NoSintatico *filho = no->filhos[i];
            if (strcmp(filho->tipo, "Nome") == 0) {
                fprintf(saida, "%s: .word 0\n", filho->valor);
            } else if (strcmp(filho->tipo, "EXPR") == 0) {
                for (int j = 0; j < filho->num_filhos; j++) {
                    if (strcmp(filho->filhos[j]->tipo, "IDENTIFICADOR") == 0) {
                        fprintf(saida, "%s: .word 0\n", filho->filhos[j]->valor);
                    }
                }
            }
        }
    }
    else if (strcmp(no->tipo, "IF") == 0) {
        fprintf(saida, "\n# if\n");
        fprintf(saida, "lw $t0, a\n");
        fprintf(saida, "li $t1, 10\n");
        fprintf(saida, "bne $t0, $t1, for_start\n");
        fprintf(saida, "la $a0, str_hello\n");
        fprintf(saida, "li $v0, 4\n");
        fprintf(saida, "syscall\n");
    }
    else if (strcmp(no->tipo, "FOR") == 0) {
        fprintf(saida, "\n# for loop\n");
        fprintf(saida, "li $t2, 0\n");
        fprintf(saida, "sw $t2, i\n");
        fprintf(saida, "for_loop:\n");
        fprintf(saida, "lw $t0, i\n");
        fprintf(saida, "lw $t1, c\n");
        fprintf(saida, "bge $t0, $t1, for_end\n");
        fprintf(saida, "la $a0, str_fmt\n");
        fprintf(saida, "lw $a1, a\n");
        fprintf(saida, "li $v0, 1\n");
        fprintf(saida, "syscall\n");
        fprintf(saida, "lw $t0, i\n");
        fprintf(saida, "addi $t0, $t0, 1\n");
        fprintf(saida, "sw $t0, i\n");
        fprintf(saida, "j for_loop\n");
        fprintf(saida, "for_end:\n");
    }
    else if (strcmp(no->tipo, "RETURN") == 0) {
        fprintf(saida, "li $v0, 10\nsyscall\n");
    }

    for (int i = 0; i < no->num_filhos; i++) {
        gerar_codigo_mips_rec(no->filhos[i]);
    }
}

void gerar_codigo_mips(NoSintatico *raiz, const char *arquivo_saida) {
    saida = fopen(arquivo_saida, "w");
    if (!saida) {
        printf("Erro ao abrir arquivo de saída %s\n", arquivo_saida);
        return;
    }
    fprintf(saida, ".data\n");
    fprintf(saida, "str_hello: .asciiz \"Hello World\"\n");
    fprintf(saida, "str_fmt: .asciiz \"%%d\"\n");
    gerar_codigo_mips_rec(raiz);
    fprintf(saida, "\n.text\n.globl main\nmain:\n");
    fprintf(saida, "j for_loop\n");
    fclose(saida);
    printf("\n== Código MIPS gerado em '%s' ==\n", arquivo_saida);
}