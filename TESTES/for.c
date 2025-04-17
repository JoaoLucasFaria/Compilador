#include <stdio.h>

int main(){

int quantidade;
printf("Escolha uma quantidade\n\n");
scanf("%d", &quantidade);

for(int quantos_rodou = 1; quantos_rodou<=quantidade; quantos_rodou++){
    printf("Rodou %d vezes\n", quantos_rodou);
}


}