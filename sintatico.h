#ifndef SINTATICO_H
#define SINTATICO_H

typedef struct NoSintatico {
    char tipo[50];
    char valor[100];
    struct NoSintatico *filho;
    struct NoSintatico *irmao;
} NoSintatico;

NoSintatico *analisar_sintatico();
void imprimir_arvore(NoSintatico *no, int nivel);

#endif
