#ifndef LEXICO_H
#define LEXICO_H

typedef struct {
    char tipo[30];
    char valor[100];
    int linha;
    int coluna;
} Token;

extern Token tokens[1000];
extern int num_tokens;

void analisar_lexico(const char *codigo);

#endif
