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

l1.c1 DIRETIVA: #include <stdio.h><br />
l3.c1 PALAVRA-CHAVE: int<br />
l3.c5 IDENTIFICADOR: main<br />
l3.c9 DELIMITADOR: (<br />
l3.c10 DELIMITADOR: )<br />
l3.c11 DELIMITADOR: {<br />
l5.c5 PALAVRA-CHAVE: int<br />
l5.c9 IDENTIFICADOR: quantidade<br />
l5.c19 DELIMITADOR: ;<br />
l6.c5 IDENTIFICADOR: printf<br />
l6.c11 DELIMITADOR: (<br />
l6.c12 STRING: "Escolha uma quantidade\n\n"<br />
l6.c40 DELIMITADOR: )<br />
l6.c41 DELIMITADOR: ;<br />
l7.c5 IDENTIFICADOR: scanf<br />
l7.c10 DELIMITADOR: (<br />
l7.c11 STRING: "%d"<br />
l7.c15 DELIMITADOR: ,<br />
l7.c17 OPERADOR: &<br />
l7.c18 IDENTIFICADOR: quantidade<br />
l7.c28 DELIMITADOR: )<br />
l7.c29 DELIMITADOR: ;<br />
l9.c5 PALAVRA-CHAVE: for<br />
l9.c8 DELIMITADOR: (<br />
l9.c9 PALAVRA-CHAVE: int<br />
l9.c13 IDENTIFICADOR: quantos_rodou<br />
l9.c27 OPERADOR: =<br />
l9.c29 NÚMERO: 1<br />
l9.c30 DELIMITADOR: ;<br />
l9.c32 IDENTIFICADOR: quantos_rodou<br />
l9.c45 OPERADOR: <=<br />
l9.c47 IDENTIFICADOR: quantidade<br />
l9.c57 DELIMITADOR: ;<br />
l9.c59 IDENTIFICADOR: quantos_rodou<br />
l9.c72 OPERADOR: ++<br />
l9.c74 DELIMITADOR: )<br />
l9.c75 DELIMITADOR: {<br />
l10.c9 IDENTIFICADOR: printf<br />
l10.c15 DELIMITADOR: (<br />
l10.c16 STRING: "Rodou %d vezes\n"<br />
l10.c34 DELIMITADOR: ,<br />
l10.c36 IDENTIFICADOR: quantos_rodou<br />
l10.c49 DELIMITADOR: )<br />
l10.c50 DELIMITADOR: ;<br />
l11.c5 DELIMITADOR: }<br />
l14.c1 DELIMITADOR: }<br />

onde lX.cY são, respectivamente, a linha e a coluna onde o analisador encontrou uma ocorrencia<br />



Feito por: João Lucas de Vilas Bôas Faria<br />
Data: 10/04/25
