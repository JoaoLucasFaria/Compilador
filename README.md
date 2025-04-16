Trabalho feito para a disciplina de Compiladores na UFSJ, 2025/1. O trabalho tem como finalidade montar um analisador léxico de um compilador para uma linguagem escolhida (Neste caso, a linguagem C).
O arquivo inclui um Makefile (digitar "make" no terminal) para a compilação do código, que então cria um arquivo executável chamado "main". Para a execução, basta usar: 

./main codigo.c

Exemplo de código e execução:

for.c:

    #include <stdio.h>

    int main(){

    int quantidade;
    printf("Escolha uma quantidade\n\n");
    scanf("%d", &quantidade);

    for(int quantos_rodou = 1; quantos_rodou<=quantidade; quantos_rodou++){
        printf("Rodou %d vezes\n", quantos_rodou);
    }
    

    }


Execução:
./main for.c

Saída:
== Tokens encontrados no arquivo 'for.c' ==

l1.c1 DIRETIVA: #include <stdio.h>
l3.c1 PALAVRA-CHAVE: int
l3.c5 IDENTIFICADOR: main
l3.c9 DELIMITADOR: (
l3.c10 DELIMITADOR: )
l3.c11 DELIMITADOR: {
l5.c5 PALAVRA-CHAVE: int
l5.c9 IDENTIFICADOR: quantidade
l5.c19 DELIMITADOR: ;
l6.c5 IDENTIFICADOR: printf
l6.c11 DELIMITADOR: (
l6.c12 STRING: "Escolha uma quantidade\n\n"
l6.c40 DELIMITADOR: )
l6.c41 DELIMITADOR: ;
l7.c5 IDENTIFICADOR: scanf
l7.c10 DELIMITADOR: (
l7.c11 STRING: "%d"
l7.c15 DELIMITADOR: ,
l7.c17 OPERADOR: &
l7.c18 IDENTIFICADOR: quantidade
l7.c28 DELIMITADOR: )
l7.c29 DELIMITADOR: ;
l9.c5 PALAVRA-CHAVE: for
l9.c8 DELIMITADOR: (
l9.c9 PALAVRA-CHAVE: int
l9.c13 IDENTIFICADOR: quantos_rodou
l9.c27 OPERADOR: =
l9.c29 NÚMERO: 1
l9.c30 DELIMITADOR: ;
l9.c32 IDENTIFICADOR: quantos_rodou
l9.c45 OPERADOR: <=
l9.c47 IDENTIFICADOR: quantidade
l9.c57 DELIMITADOR: ;
l9.c59 IDENTIFICADOR: quantos_rodou
l9.c72 OPERADOR: ++
l9.c74 DELIMITADOR: )
l9.c75 DELIMITADOR: {
l10.c9 IDENTIFICADOR: printf
l10.c15 DELIMITADOR: (
l10.c16 STRING: "Rodou %d vezes\n"
l10.c34 DELIMITADOR: ,
l10.c36 IDENTIFICADOR: quantos_rodou
l10.c49 DELIMITADOR: )
l10.c50 DELIMITADOR: ;
l11.c5 DELIMITADOR: }
l14.c1 DELIMITADOR: }

Feito por: João Lucas de Vilas Bôas Faria
Data: 10/04/25
