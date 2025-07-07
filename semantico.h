#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "sintatico.h"

typedef struct {
    char nome[100];
    char tipo[20];
    int escopo;
    int usada; // <- aqui
} Simbolo;

extern Simbolo tabela[1000];
extern int num_simbolos;
extern int escopo_atual;

int analisar_semantico(NoSintatico *no);

#endif
