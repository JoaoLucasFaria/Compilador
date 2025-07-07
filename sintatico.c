#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sintatico.h"
#include "lexico.h"

int pos = 0;

NoSintatico *criar_no(const char *tipo, const char *valor) {
    NoSintatico *no = malloc(sizeof(NoSintatico));
    strcpy(no->tipo, tipo);
    strcpy(no->valor, valor);
    no->num_filhos = 0;
    return no;
}

void adicionar_filho(NoSintatico *pai, NoSintatico *filho) {
    if (pai->num_filhos < MAX_FILHOS) {
        pai->filhos[pai->num_filhos++] = filho;
    }
}

void imprimir_arvore(NoSintatico *no, int nivel) {
    for (int i = 0; i < nivel; i++)
        printf("  ");
    if (strlen(no->valor) > 0)
        printf("%s: %s\n", no->tipo, no->valor);
    else
        printf("%s\n", no->tipo);

    for (int i = 0; i < no->num_filhos; i++) {
        imprimir_arvore(no->filhos[i], nivel + 1);
    }
}

int token_atual_e(const char *tipo, const char *valor) {
    return pos < num_tokens && strcmp(tokens[pos].tipo, tipo) == 0 &&
           (valor == NULL || strcmp(tokens[pos].valor, valor) == 0);
}

Token *consumir_token() {
    return &tokens[pos++];
}

void erro_sintatico(const char *esperado) {
    if (pos < num_tokens) {
        printf("ERRO SINTÁTICO: Esperado %s na linha %d, mas encontrou %s (%s)\n",
               esperado, tokens[pos].linha, tokens[pos].tipo, tokens[pos].valor);
    } else {
        printf("ERRO SINTÁTICO: Esperado %s no final do arquivo\n", esperado);
    }
    exit(1);
}

// === Parsing ===

NoSintatico *parse_bloco();
NoSintatico *parse_comando();
NoSintatico *parse_expressao();

NoSintatico *parse_if() {
    NoSintatico *no = criar_no("IF", "");
    consumir_token(); // 'if'

    if (!token_atual_e("DELIMITADOR", "("))
        erro_sintatico("DELIMITADOR '('");
    consumir_token();

    adicionar_filho(no, parse_expressao());

    if (!token_atual_e("DELIMITADOR", ")"))
        erro_sintatico("DELIMITADOR ')'");
    consumir_token();

    adicionar_filho(no, parse_comando());

    if (token_atual_e("PALAVRA-CHAVE", "else")) {
        consumir_token();
        adicionar_filho(no, parse_comando());
    }

    return no;
}

NoSintatico *parse_while() {
    NoSintatico *no = criar_no("WHILE", "");
    consumir_token(); // 'while'

    if (!token_atual_e("DELIMITADOR", "("))
        erro_sintatico("DELIMITADOR '('");
    consumir_token();

    adicionar_filho(no, parse_expressao());

    if (!token_atual_e("DELIMITADOR", ")"))
        erro_sintatico("DELIMITADOR ')'");
    consumir_token();

    adicionar_filho(no, parse_comando());
    return no;
}

NoSintatico *parse_for() {
    NoSintatico *no = criar_no("FOR", "");
    consumir_token(); // 'for'

    if (!token_atual_e("DELIMITADOR", "("))
        erro_sintatico("DELIMITADOR '('");
    consumir_token();

    adicionar_filho(no, parse_expressao());
    if (!token_atual_e("DELIMITADOR", ";"))
        erro_sintatico("DELIMITADOR ';'");
    consumir_token();

    adicionar_filho(no, parse_expressao());
    if (!token_atual_e("DELIMITADOR", ";"))
        erro_sintatico("DELIMITADOR ';'");
    consumir_token();

    adicionar_filho(no, parse_expressao());
    if (!token_atual_e("DELIMITADOR", ")"))
        erro_sintatico("DELIMITADOR ')'");
    consumir_token();

    adicionar_filho(no, parse_comando());
    return no;
}

NoSintatico *parse_return() {
    NoSintatico *no = criar_no("RETURN", "");
    consumir_token(); // 'return'

    adicionar_filho(no, parse_expressao());

    if (!token_atual_e("DELIMITADOR", ";"))
        erro_sintatico("DELIMITADOR ';'");
    consumir_token();
    return no;
}

NoSintatico *parse_bloco() {
    if (!token_atual_e("DELIMITADOR", "{"))
        erro_sintatico("DELIMITADOR '{'");
    consumir_token();

    NoSintatico *no = criar_no("BLOCO", "");
    while (!token_atual_e("DELIMITADOR", "}")) {
        adicionar_filho(no, parse_comando());
    }

    consumir_token(); // '}'
    return no;
}

NoSintatico *parse_expressao() {
    NoSintatico *no = criar_no("EXPR", "");
    int parenteses_abertos = 0;

    while (pos < num_tokens) {
        if (token_atual_e("DELIMITADOR", "(")) {
            parenteses_abertos++;
        } else if (token_atual_e("DELIMITADOR", ")")) {
            if (parenteses_abertos == 0)
                break;
            parenteses_abertos--;
        }

        if (parenteses_abertos == 0) {
            if (token_atual_e("DELIMITADOR", ";"))
                break;
            if (token_atual_e("DELIMITADOR", "}"))
                erro_sintatico("DELIMITADOR ';' antes de '}'");
        }

        adicionar_filho(no, criar_no(tokens[pos].tipo, tokens[pos].valor));
        consumir_token();
    }

    return no;
}

NoSintatico *parse_comando() {
    if (token_atual_e("PALAVRA-CHAVE", "int") || token_atual_e("PALAVRA-CHAVE", "float") || token_atual_e("PALAVRA-CHAVE", "char")) {
        NoSintatico *no = criar_no("DECLARACAO", tokens[pos].valor);
        consumir_token(); // tipo

        if (!token_atual_e("IDENTIFICADOR", NULL))
            erro_sintatico("IDENTIFICADOR");
        adicionar_filho(no, criar_no("Nome", tokens[pos].valor));
        consumir_token();

        while (token_atual_e("DELIMITADOR", ",")) {
            consumir_token();
            if (!token_atual_e("IDENTIFICADOR", NULL))
                erro_sintatico("IDENTIFICADOR");
            adicionar_filho(no, criar_no("Nome", tokens[pos].valor));
            consumir_token();
        }

        if (token_atual_e("OPERADOR", "=")) {
            consumir_token();
            adicionar_filho(no, parse_expressao());
        }

        if (!token_atual_e("DELIMITADOR", ";"))
            erro_sintatico("DELIMITADOR ';'");
        consumir_token();

        return no;
    }

    if (token_atual_e("PALAVRA-CHAVE", "if"))
        return parse_if();
    if (token_atual_e("PALAVRA-CHAVE", "while"))
        return parse_while();
    if (token_atual_e("PALAVRA-CHAVE", "for"))
        return parse_for();
    if (token_atual_e("PALAVRA-CHAVE", "return"))
        return parse_return();
    if (token_atual_e("DELIMITADOR", "{"))
        return parse_bloco();

    NoSintatico *no = criar_no("COMANDO", "");
    adicionar_filho(no, parse_expressao());
    if (!token_atual_e("DELIMITADOR", ";"))
        erro_sintatico("DELIMITADOR ';'");
    consumir_token();
    return no;
}

NoSintatico *parse_funcao() {
    NoSintatico *no = criar_no("FUNCAO", "");

    if (!token_atual_e("PALAVRA-CHAVE", NULL))
        erro_sintatico("tipo de retorno");
    adicionar_filho(no, criar_no("Tipo", tokens[pos].valor));
    consumir_token();

    if (!token_atual_e("IDENTIFICADOR", NULL))
        erro_sintatico("IDENTIFICADOR");
    adicionar_filho(no, criar_no("Nome", tokens[pos].valor));
    consumir_token();

    if (!token_atual_e("DELIMITADOR", "("))
        erro_sintatico("DELIMITADOR '('");
    consumir_token();

    if (!token_atual_e("DELIMITADOR", ")"))
        erro_sintatico("DELIMITADOR ')'");
    consumir_token();

    adicionar_filho(no, parse_bloco());
    return no;
}

NoSintatico *analisar_sintatico() {
    pos = 0;
    NoSintatico *raiz = criar_no("Programa", "");

    while (pos < num_tokens) {
        if (strcmp(tokens[pos].tipo, "DIRETIVA") == 0) {
            pos++;
            continue;
        }
        adicionar_filho(raiz, parse_funcao());
    }

    imprimir_arvore(raiz, 0);
    return raiz;
}

