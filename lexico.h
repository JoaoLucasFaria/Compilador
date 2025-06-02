#ifndef LEXICO_H
#define LEXICO_H

typedef struct {
    int linha;
    int coluna;
    char tipo[20];
    char valor[100];
} Token;

extern Token tokens[1000];
extern int num_tokens;

void analisar_lexico(const char *nome_arquivo);

#endif
