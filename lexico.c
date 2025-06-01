#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexico.h"

#define MAX_TOKEN 1000

Token tokens[MAX_TOKEN];
int num_tokens = 0;

const char *palavras_chave[] = {
    "int", "float", "char", "void", "if", "else", "while", "for", "return"};

int eh_palavra_chave(const char *str) {
    for (int i = 0; i < sizeof(palavras_chave) / sizeof(char *); i++)
        if (strcmp(str, palavras_chave[i]) == 0) return 1;
    return 0;
}

void adicionar_token(const char *tipo, const char *valor, int linha, int coluna) {
    if (num_tokens >= MAX_TOKEN) return;
    strcpy(tokens[num_tokens].tipo, tipo);
    strcpy(tokens[num_tokens].valor, valor);
    tokens[num_tokens].linha = linha;
    tokens[num_tokens].coluna = coluna;
    num_tokens++;
}

void analisar_lexico(const char *codigo) {
    int i = 0, linha = 1, coluna = 1;
    while (codigo[i] != '\0') {
        if (isspace(codigo[i])) {
            if (codigo[i] == '\n') { linha++; coluna = 1; }
            else coluna++;
            i++;
            continue;
        }

        if (codigo[i] == '/' && codigo[i + 1] == '/') {
            while (codigo[i] != '\n' && codigo[i] != '\0') i++;
            continue;
        }

        if (isalpha(codigo[i]) || codigo[i] == '_') {
            char buffer[100];
            int j = 0;
            while (isalnum(codigo[i]) || codigo[i] == '_') buffer[j++] = codigo[i++];
            buffer[j] = '\0';
            adicionar_token(eh_palavra_chave(buffer) ? "PALAVRA-CHAVE" : "IDENTIFICADOR", buffer, linha, coluna);
            coluna += j;
            continue;
        }

        if (isdigit(codigo[i])) {
            char buffer[100];
            int j = 0;
            while (isdigit(codigo[i])) buffer[j++] = codigo[i++];
            if (codigo[i] == '.') {
                buffer[j++] = codigo[i++];
                while (isdigit(codigo[i])) buffer[j++] = codigo[i++];
                buffer[j] = '\0';
                adicionar_token("FLOAT", buffer, linha, coluna);
            } else {
                buffer[j] = '\0';
                adicionar_token("NÚMERO", buffer, linha, coluna);
            }
            coluna += j;
            continue;
        }

        if (codigo[i] == '\'' && isprint(codigo[i + 1]) && codigo[i + 2] == '\'') {
            char buffer[4] = {codigo[i], codigo[i + 1], codigo[i + 2], '\0'};
            adicionar_token("CHAR", buffer, linha, coluna);
            i += 3;
            coluna += 3;
            continue;
        }

        if (codigo[i] == '"') {
            char buffer[100];
            int j = 0;
            buffer[j++] = codigo[i++];
            while (codigo[i] != '"' && codigo[i] != '\0') buffer[j++] = codigo[i++];
            if (codigo[i] == '"') buffer[j++] = codigo[i++];
            buffer[j] = '\0';
            adicionar_token("STRING", buffer, linha, coluna);
            coluna += j;
            continue;
        }

        // operadores compostos
        if ((codigo[i] == '=' && codigo[i + 1] == '=') ||
            (codigo[i] == '!' && codigo[i + 1] == '=') ||
            (codigo[i] == '<' && codigo[i + 1] == '=') ||
            (codigo[i] == '>' && codigo[i + 1] == '=') ||
            (codigo[i] == '&' && codigo[i + 1] == '&') ||
            (codigo[i] == '|' && codigo[i + 1] == '|')) {
            char op[3] = {codigo[i], codigo[i + 1], '\0'};
            adicionar_token("OPERADOR", op, linha, coluna);
            i += 2;
            coluna += 2;
            continue;
        }

        // operadores simples
        if (strchr("+-*/=<>!&|", codigo[i])) {
            char op[2] = {codigo[i], '\0'};
            adicionar_token("OPERADOR", op, linha, coluna);
            i++;
            coluna++;
            continue;
        }

        // delimitadores
        if (strchr("(){}[];,", codigo[i])) {
            char d[2] = {codigo[i], '\0'};
            adicionar_token("DELIMITADOR", d, linha, coluna);
            i++;
            coluna++;
            continue;
        }

        // caractere desconhecido
        char erro[2] = {codigo[i], '\0'};
        adicionar_token("ERRO", erro, linha, coluna);
        i++;
        coluna++;
    }

    // imprimir tokens
    for (int t = 0; t < num_tokens; t++) {
        printf("l%d.c%d %s: %s\n", tokens[t].linha, tokens[t].coluna, tokens[t].tipo, tokens[t].valor);
    }
}
