#include <stdio.h>

int main(){

 int choice = 1, num, soma = 0;

while (choice)
    {
        printf("Escolha uma número (ou digite 0 para sair)\n\n");
        scanf("%d", &num);

        if(num == 0) choice = 0;

        if (num %2 != 0)
        {
            soma += num;
        }


    }    

    printf("Soma: %d\n\n", soma);

}