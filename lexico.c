#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexico.h"

Token tokens[1000];
int num_tokens = 0;

void adicionar_token(int linha, int coluna, const char *tipo, const char *valor) {
    Token t;
    t.linha = linha;
    t.coluna = coluna;
    strcpy(t.tipo, tipo);
    strcpy(t.valor, valor);
    tokens[num_tokens++] = t;
}

int eh_palavra_chave(const char *str) {
    const char *palavras[] = {"int", "float", "char", "if", "else", "while", "for", "return"};
    for (int i = 0; i < 8; i++) {
        if (strcmp(str, palavras[i]) == 0) return 1;
    }
    return 0;
}

void analisar_lexico(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    int c, linha = 1, coluna = 1;
    while ((c = fgetc(arquivo)) != EOF) {
        if (isspace(c)) {
            if (c == '\n') { linha++; coluna = 1; }
            else coluna++;
            continue;
        }

        if (isalpha(c) || c == '_') {
            char buffer[100];
            int i = 0, start_col = coluna;
            do {
                buffer[i++] = c;
                c = fgetc(arquivo); coluna++;
            } while (isalnum(c) || c == '_');
            buffer[i] = '\0';
            ungetc(c, arquivo); coluna--;

            if (eh_palavra_chave(buffer))
                adicionar_token(linha, start_col, "PALAVRA-CHAVE", buffer);
            else
                adicionar_token(linha, start_col, "IDENTIFICADOR", buffer);
        }
        else if (isdigit(c)) {
            char buffer[100];
            int i = 0, start_col = coluna;
            do {
                buffer[i++] = c;
                c = fgetc(arquivo); coluna++;
            } while (isdigit(c));
            buffer[i] = '\0';
            ungetc(c, arquivo); coluna--;
            adicionar_token(linha, start_col, "NÚMERO", buffer);
        }
        else if (c == '"') {
            char buffer[100];
            int i = 0, start_col = coluna++;
            buffer[i++] = '"';
            while ((c = fgetc(arquivo)) != '"' && c != EOF) {
                buffer[i++] = c; coluna++;
            }
            buffer[i++] = '"';
            buffer[i] = '\0';
            adicionar_token(linha, start_col, "STRING", buffer);
        }
        else if (strchr("(){};,", c)) {
            char str[2] = {c, '\0'};
            adicionar_token(linha, coluna, "DELIMITADOR", str);
            coluna++;
        }
        else if (strchr("+-*/%=!<>", c)) {
            char str[3] = {c, '\0'};
            int next = fgetc(arquivo);
            if (next == '=' || (c == '+' && next == '+') || (c == '-' && next == '-')) {
                str[1] = next; str[2] = '\0'; coluna++;
            } else ungetc(next, arquivo);
            adicionar_token(linha, coluna, "OPERADOR", str);
            coluna++;
        }
        else {
            char str[2] = {c, '\0'};
            adicionar_token(linha, coluna, "ERRO", str);
            coluna++;
        }
    }

    fclose(arquivo);
}
