#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
    srand(time(NULL));

    int n, m, c;

    // pega quantos itens como entrada do usuário
    if(argc == 2) n = atoi(argv[1]);
    else {
        fprintf(stderr,"usagem: ./a.out $n\n");
        exit(1);
    }
    // define o número de pares com uma conta qualquer baseada no número de itens
    m = rand()%(n/2) + 3;
    // define a capacidade baseada no número de pares
    c = m*5;
    printf("%d %d %d\n", n, m, c);
    // gera o valor e peso de cada item
    for(int i = 0; i < n; i++){
        printf("%d ", rand()%(c/2) + 1);
    }
    printf("\n");
    for(int i = 0; i < n; i++){
        printf("%d ", rand()%c + 1);
    }
    printf("\n");

    int i = 0;
    // gera os pares, evitando que tenham o mesmo número
    // não evita pares repetidos
    while(i < m){
        int a, b;
        a = rand()%n + 1;
        b = rand()%n + 1;
        if(a == b){
            continue;
        }
        printf("%d %d\n", a, b);
        i++;
    }

}