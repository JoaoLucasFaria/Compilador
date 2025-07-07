#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "semantico.h"

Simbolo tabela[1000];
int num_simbolos = 0;
int escopo_atual = 0;

void inserir_simbolo(const char *nome, const char *tipo) {
    for (int i = 0; i < num_simbolos; i++) {
        if (strcmp(tabela[i].nome, nome) == 0 && tabela[i].escopo == escopo_atual) {
            printf("ERRO SEMÂNTICO: variável '%s' já declarada no mesmo escopo.\n", nome);
            exit(0);
        }
    }
    strcpy(tabela[num_simbolos].nome, nome);
    strcpy(tabela[num_simbolos].tipo, tipo);
    tabela[num_simbolos].escopo = escopo_atual;
    tabela[num_simbolos].usada = 0;
    num_simbolos++;
}

const char *buscar_tipo(const char *nome) {
    for (int i = num_simbolos - 1; i >= 0; i--) {
        if (strcmp(tabela[i].nome, nome) == 0) {
            tabela[i].usada = 1;
            return tabela[i].tipo;
        }
    }
    return NULL;
}

void verificar_identificadores(NoSintatico *no, int em_declaracao) {
    if (strcmp(no->tipo, "EXPR") == 0) {
        for (int i = 0; i < no->num_filhos; i++) {
            NoSintatico *filho = no->filhos[i];
            if (strcmp(filho->tipo, "IDENTIFICADOR") == 0) {
                if (strcmp(filho->valor, "printf") == 0)
                    continue;
                if (!em_declaracao) {
                    const char *tipo = buscar_tipo(filho->valor);
                    if (tipo == NULL) {
                        printf("ERRO SEMÂNTICO: variável '%s' usada mas não declarada.\n", filho->valor);
                    }
                }
            }
        }
    }
}

void analisar_semantico_rec(NoSintatico *no) {
    int contexto_decl = 0;

    if (strcmp(no->tipo, "DECLARACAO") == 0) {
        contexto_decl = 1;
        const char *tipo = no->valor;
        for (int i = 0; i < no->num_filhos; i++) {
            NoSintatico *filho = no->filhos[i];
            if (strcmp(filho->tipo, "Nome") == 0) {
                inserir_simbolo(filho->valor, tipo);
            }
            else if (strcmp(filho->tipo, "EXPR") == 0) {
                // declarações múltiplas: int a = 10, c = 20, d = 5;
                for (int j = 0; j < filho->num_filhos; j++) {
                    if (strcmp(filho->filhos[j]->tipo, "IDENTIFICADOR") == 0) {
                        inserir_simbolo(filho->filhos[j]->valor, tipo);
                    }
                }
            }
        }
    }

    // reconhecer declaração inline dentro do for
    if (strcmp(no->tipo, "EXPR") == 0 && no->num_filhos > 1 &&
        strcmp(no->filhos[0]->tipo, "PALAVRA-CHAVE") == 0) {
        contexto_decl = 1;
        const char *tipo = no->filhos[0]->valor;
        for (int i = 1; i < no->num_filhos; i++) {
            if (strcmp(no->filhos[i]->tipo, "IDENTIFICADOR") == 0) {
                inserir_simbolo(no->filhos[i]->valor, tipo);
            }
        }
    }

    if (strcmp(no->tipo, "BLOCO") == 0) {
        escopo_atual++;
        for (int i = 0; i < no->num_filhos; i++) {
            analisar_semantico_rec(no->filhos[i]);
        }
        escopo_atual--;
        return;
    }

    verificar_identificadores(no, contexto_decl);

    for (int i = 0; i < no->num_filhos; i++) {
        analisar_semantico_rec(no->filhos[i]);
    }
}

int analisar_semantico(NoSintatico *no) {
    int pass;
    printf("\n== Análise Semântica ==\n\n");
    analisar_semantico_rec(no);

    printf("\n== Checando variáveis não usadas ==\n");
    for (int i = 0; i < num_simbolos; i++) {
        if (!tabela[i].usada) {
            printf("AVISO SEMÂNTICO: variável '%s' declarada mas não utilizada.\n", tabela[i].nome);
            pass = 1;
        }
        if (pass) return 0;
        else return 1;
    }
}
