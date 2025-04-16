#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexico.h"

#define MAX_TOKEN 100

// Lista de palavras-chave da linguagem
const char *palavras_chave[] = {
    "int", "float", "if", "else", "while", "for", "return", "char", "double", "void"};

// Verifica se uma string é palavra-chave
int eh_palavra_chave(const char *str) {
    for (int i = 0; i < sizeof(palavras_chave) / sizeof(palavras_chave[0]); i++) {
        if (strcmp(str, palavras_chave[i]) == 0)
            return 1;
    }
    return 0;
}

// Verifica se um caractere é delimitador
int eh_delimitador(char c) {
    return c == ' ' || c == '+' || c == '-' || c == '*' || c == '/' ||
           c == ',' || c == ';' || c == '%' || c == '>' || c == '<' ||
           c == '=' || c == '(' || c == ')' || c == '[' || c == ']' ||
           c == '{' || c == '}';
}

// Verifica se é um operador simples
int eh_operador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '>' ||
           c == '<' || c == '=' || c == '&' || c == '|';
}

// Verifica se é um operador duplo (como ==, <=, ++)
int eh_operador_duplo(char a, char b) {
    return (a == '=' && b == '=') || (a == '<' && b == '=') ||
           (a == '>' && b == '=') || (a == '+' && b == '+') ||
           (a == '-' && b == '-') || (a == '&' && b == '&') ||
           (a == '|' && b == '|');
}

void analisar_lexico(const char *codigo) {
    int i = 0, linha = 1, coluna = 1;
    int parenteses = 0, colchetes = 0, chaves = 0;
    int aspas_abertas = 0;
    int linha_paren = -1, col_paren = -1;
    int linha_colchete = -1, col_colchete = -1;
    int linha_chave = -1, col_chave = -1;
    int linha_aspas = -1, col_aspas = -1;

    while (codigo[i] != '\0') {
        if (isspace(codigo[i])) {
            if (codigo[i] == '\n') {
                linha++;
                coluna = 1;
            } else {
                coluna++;
            }
            i++;
            continue;
        }

        if (codigo[i] == '#') {
            char buffer[MAX_TOKEN];
            int j = 0;
            while (codigo[i] != '\n' && codigo[i] != '\0') {
                buffer[j++] = codigo[i++];
            }
            buffer[j] = '\0';
            printf("l%d.c%d DIRETIVA: %s\n", linha, coluna, buffer);
            coluna += strlen(buffer);
            continue;
        }

        if (codigo[i] == '"') {
            char buffer[MAX_TOKEN];
            int j = 0;
            linha_aspas = linha;
            col_aspas = coluna;
            buffer[j++] = codigo[i++];
            coluna++;
            aspas_abertas++;

            while (codigo[i] != '"' && codigo[i] != '\0') {
                if (codigo[i] == '\n') {
                    linha++;
                    coluna = 1;
                } else {
                    coluna++;
                }
                buffer[j++] = codigo[i++];
            }

            if (codigo[i] == '"') {
                buffer[j++] = codigo[i++];
                coluna++;
                aspas_abertas--;
            }

            buffer[j] = '\0';
            printf("l%d.c%d STRING: %s\n", linha_aspas, col_aspas, buffer);
            continue;
        }

        if (isalpha(codigo[i]) || codigo[i] == '_') {
            char buffer[MAX_TOKEN];
            int j = 0;
            while (isalnum(codigo[i]) || codigo[i] == '_') {
                buffer[j++] = codigo[i++];
                coluna++;
            }
            buffer[j] = '\0';
            if (eh_palavra_chave(buffer)) {
                printf("l%d.c%d PALAVRA-CHAVE: %s\n", linha, coluna - j, buffer);
            } else {
                printf("l%d.c%d IDENTIFICADOR: %s\n", linha, coluna - j, buffer);
            }
            continue;
        }

        if (isdigit(codigo[i])) {
            char buffer[MAX_TOKEN];
            int j = 0;
            while (isdigit(codigo[i])) {
                buffer[j++] = codigo[i++];
                coluna++;
            }
            buffer[j] = '\0';
            printf("l%d.c%d NÚMERO: %s\n", linha, coluna - j, buffer);
            continue;
        }

        if (eh_operador(codigo[i]) && eh_operador_duplo(codigo[i], codigo[i + 1])) {
            printf("l%d.c%d OPERADOR: %c%c\n", linha, coluna, codigo[i], codigo[i + 1]);
            i += 2;
            coluna += 2;
            continue;
        }

        if (eh_operador(codigo[i])) {
            printf("l%d.c%d OPERADOR: %c\n", linha, coluna, codigo[i]);
            i++;
            coluna++;
            continue;
        }

        if (eh_delimitador(codigo[i])) {
            char c = codigo[i];
            printf("l%d.c%d DELIMITADOR: %c\n", linha, coluna, c);

            if (c == '(') { parenteses++; linha_paren = linha; col_paren = coluna; }
            if (c == ')') parenteses--;
            if (c == '[') { colchetes++; linha_colchete = linha; col_colchete = coluna; }
            if (c == ']') colchetes--;
            if (c == '{') { chaves++; linha_chave = linha; col_chave = coluna; }
            if (c == '}') chaves--;

            i++;
            coluna++;
            continue;
        }

        printf("l%d.c%d ERRO LÉXICO: caractere inválido '%c'\n", linha, coluna, codigo[i]);
        i++;
        coluna++;
    }

    // Verificações finais
    if (aspas_abertas > 0)
        printf("ERRO: %d string(s) com aspas sem fechamento correspondente em l%d.c%d.\n", aspas_abertas, linha_aspas, col_aspas);
    if (parenteses > 0)
        printf("ERRO: %d parêntese(s) '(' sem fechamento correspondente em l%d.c%d.\n", parenteses, linha_paren, col_paren);
    if (colchetes > 0)
        printf("ERRO: %d colchete(s) '[' sem fechamento correspondente em l%d.c%d.\n", colchetes, linha_colchete, col_colchete);
    if (chaves > 0)
        printf("ERRO: %d chave(s) '{' sem fechamento correspondente em l%d.c%d.\n", chaves, linha_chave, col_chave);
}