#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sintatico.h"
#include "lexico.h"

int pos = 0;

NoSintatico *criar_no(const char *tipo, const char *valor) {
    NoSintatico *no = malloc(sizeof(NoSintatico));
    strcpy(no->tipo, tipo);
    strcpy(no->valor, valor ? valor : "");
    no->filho = NULL;
    no->irmao = NULL;
    return no;
}

void adicionar_filho(NoSintatico *pai, NoSintatico *filho) {
    if (!pai->filho) pai->filho = filho;
    else {
        NoSintatico *atual = pai->filho;
        while (atual->irmao) atual = atual->irmao;
        atual->irmao = filho;
    }
}

Token *token_atual() {
    return &tokens[pos];
}

int aceitar(const char *tipo, const char *valor) {
    if (pos >= num_tokens) return 0;
    if ((tipo == NULL || strcmp(tokens[pos].tipo, tipo) == 0) &&
        (valor == NULL || strcmp(tokens[pos].valor, valor) == 0)) {
        pos++;
        return 1;
    }
    return 0;
}

int esperar(const char *tipo, const char *valor) {
    if (aceitar(tipo, valor)) return 1;
    printf("ERRO SINTÁTICO: Esperado %s '%s' na linha %d\n", tipo, valor, token_atual()->linha);
    exit(1);
}

NoSintatico *parse_expressao();
NoSintatico *parse_comando();
NoSintatico *parse_bloco();

NoSintatico *parse_expressao() {
    Token *tk = token_atual();
    NoSintatico *no = criar_no("Expressao", "");

    if (tk->tipo[0] == 'N' || tk->tipo[0] == 'F' || tk->tipo[0] == 'C' || tk->tipo[0] == 'I') {
        adicionar_filho(no, criar_no(tk->tipo, tk->valor));
        pos++;
    }

    while (strcmp(token_atual()->tipo, "OPERADOR") == 0) {
        Token *op = token_atual();
        pos++;
        Token *dir = token_atual();
        NoSintatico *n_op = criar_no("Operacao", op->valor);
        pos++;
        adicionar_filho(n_op, criar_no(dir->tipo, dir->valor));
        adicionar_filho(no, n_op);
    }

    return no;
}

NoSintatico *parse_declaracao() {
    Token *tipo = token_atual();
    pos++;
    Token *id = token_atual();
    esperar("IDENTIFICADOR", NULL);
    NoSintatico *no = criar_no("Declaracao", tipo->valor);
    adicionar_filho(no, criar_no("Identificador", id->valor));

    if (aceitar("OPERADOR", "=")) {
        adicionar_filho(no, parse_expressao());
    }

    esperar("DELIMITADOR", ";");
    return no;
}

NoSintatico *parse_retorno() {
    esperar("PALAVRA-CHAVE", "return");
    NoSintatico *no = criar_no("Return", "");
    adicionar_filho(no, parse_expressao());
    esperar("DELIMITADOR", ";");
    return no;
}

NoSintatico *parse_if() {
    esperar("PALAVRA-CHAVE", "if");
    esperar("DELIMITADOR", "(");
    NoSintatico *no = criar_no("If", "");
    adicionar_filho(no, parse_expressao());
    esperar("DELIMITADOR", ")");
    adicionar_filho(no, parse_comando());
    if (aceitar("PALAVRA-CHAVE", "else")) {
        adicionar_filho(no, parse_comando());
    }
    return no;
}

NoSintatico *parse_while() {
    esperar("PALAVRA-CHAVE", "while");
    esperar("DELIMITADOR", "(");
    NoSintatico *no = criar_no("While", "");
    adicionar_filho(no, parse_expressao());
    esperar("DELIMITADOR", ")");
    adicionar_filho(no, parse_comando());
    return no;
}

NoSintatico *parse_for() {
    esperar("PALAVRA-CHAVE", "for");
    esperar("DELIMITADOR", "(");
    NoSintatico *no = criar_no("For", "");
    adicionar_filho(no, parse_expressao());
    esperar("DELIMITADOR", ";");
    adicionar_filho(no, parse_expressao());
    esperar("DELIMITADOR", ";");
    adicionar_filho(no, parse_expressao());
    esperar("DELIMITADOR", ")");
    adicionar_filho(no, parse_comando());
    return no;
}

NoSintatico *parse_comando() {
    if (strcmp(token_atual()->tipo, "PALAVRA-CHAVE") == 0) {
        if (strcmp(token_atual()->valor, "if") == 0) return parse_if();
        if (strcmp(token_atual()->valor, "while") == 0) return parse_while();
        if (strcmp(token_atual()->valor, "for") == 0) return parse_for();
        if (strcmp(token_atual()->valor, "return") == 0) return parse_retorno();
    }

    if (strcmp(token_atual()->tipo, "DELIMITADOR") == 0 && strcmp(token_atual()->valor, "{") == 0) {
        return parse_bloco();
    }

    NoSintatico *cmd = criar_no("Comando", "");
    adicionar_filho(cmd, parse_expressao());
    esperar("DELIMITADOR", ";");
    return cmd;
}

NoSintatico *parse_bloco() {
    esperar("DELIMITADOR", "{");
    NoSintatico *bloco = criar_no("Bloco", "");
    while (!(strcmp(token_atual()->tipo, "DELIMITADOR") == 0 && strcmp(token_atual()->valor, "}") == 0)) {
        if (strcmp(token_atual()->tipo, "PALAVRA-CHAVE") == 0 &&
           (strcmp(token_atual()->valor, "int") == 0 || strcmp(token_atual()->valor, "float") == 0 ||
            strcmp(token_atual()->valor, "char") == 0)) {
            adicionar_filho(bloco, parse_declaracao());
        } else {
            adicionar_filho(bloco, parse_comando());
        }
    }
    esperar("DELIMITADOR", "}");
    return bloco;
}

NoSintatico *parse_funcao() {
    Token *tipo = token_atual();
    pos++;
    Token *nome = token_atual();
    esperar("IDENTIFICADOR", NULL);
    esperar("DELIMITADOR", "(");
    esperar("DELIMITADOR", ")");
    NoSintatico *no = criar_no("Funcao", nome->valor);
    adicionar_filho(no, parse_bloco());
    return no;
}

NoSintatico *analisar_sintatico() {
    NoSintatico *raiz = criar_no("Programa", "");
    pos = 0;
    while (pos < num_tokens) {
        if (strcmp(token_atual()->tipo, "PALAVRA-CHAVE") == 0 &&
           (strcmp(token_atual()->valor, "int") == 0 || strcmp(token_atual()->valor, "float") == 0 ||
            strcmp(token_atual()->valor, "char") == 0 || strcmp(token_atual()->valor, "void") == 0)) {
            if (strcmp(tokens[pos + 2].valor, "(") == 0)
                adicionar_filho(raiz, parse_funcao());
            else
                adicionar_filho(raiz, parse_declaracao());
        } else {
            adicionar_filho(raiz, parse_comando());
        }
    }
    imprimir_arvore(raiz, 0);
    return raiz;
}

void imprimir_arvore(NoSintatico *no, int nivel) {
    if (!no) return;
    for (int i = 0; i < nivel; i++) printf("  ");
    printf("%s", no->tipo);
    if (strlen(no->valor)) printf(": %s", no->valor);
    printf("\n");
    imprimir_arvore(no->filho, nivel + 1);
    imprimir_arvore(no->irmao, nivel);
}
