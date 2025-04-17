#include <stdio.h>

int main(){

    int rodou = 1, escolha = 1;

    while (escolha)
    {
        printf("PRINTADO %d VEZES\n", rodou);
        rodou++;
        printf("Quer sair? 1 para continuar 0 para parar\n\n");
        scanf("%d", &escolha);
    }
    

}