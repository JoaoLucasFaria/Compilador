#include <stdio.h>

int main(){
    int choice = 1, custo, lucro = 0, cobrar;

    while (choice){

        printf("Digite quanto Cobrou e o Custo\n");
        scanf("%d %d", &cobrar, &custo);

        lucro += cobrar - custo;

        if (!custo && !cobrar)
        {
            choice = 0;
        }
        
    }

    printf("Lucro: %d\n\n", lucro);
}