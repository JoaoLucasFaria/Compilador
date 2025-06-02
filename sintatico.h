#ifndef SINTATICO_H
#define SINTATICO_H

#define MAX_FILHOS 10

typedef struct NoSintatico {
    char tipo[50];
    char valor[100];
    struct NoSintatico *filhos[MAX_FILHOS];
    int num_filhos;
} NoSintatico;

NoSintatico *criar_no(const char *tipo, const char *valor);
void adicionar_filho(NoSintatico *pai, NoSintatico *filho);
void imprimir_arvore(NoSintatico *no, int nivel);
void analisar_sintatico();

#endif
