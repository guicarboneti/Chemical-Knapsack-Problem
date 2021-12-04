#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura do item para guardar valor, peso e valor por peso do item
typedef struct Item {
  float weight;
  float value;
  float VpW;  // valor por item
  int *pairs; // pares proibidos
  int select; // flag indicando se o item foi selecionado ou não
  float bound;  // limite superior do melhor lucro possível na subárvore do nó
  int id;
} Item;

int proibido(Item *items, int curIndex, int n) {
  int i;
  for (i=0; i<curIndex; i++) {
    if ((items[i].id != items[curIndex].id) && items[i].select)
      if (items[i].pairs[items[curIndex].id] == 1)
        return 1;
  }
  return 0;
}

void findBound(int W, Item *items, int curIndex, int n) {
  int i;
  for (i=curIndex; i<n; i++) {
    if (W-items[i].weight >= 0) {  // adiciona os itens mais valiosos enquanto cabem na mochila
      items[curIndex].bound += items[i].value;
      W -= items[i].weight;
    }
    else {  // adiciona uma fração do valor do item se não couber inteiro na mochila
      items[curIndex].bound += (W * items[i].value) / items[i].weight;
      break;
    }
  }
}

float BranchNBound(int W, Item *items, int i, int n, int *nodes, float *bestProfit, float currentProfit) {

  *nodes += 1;

  // retorna se excedeu a capacidade da mochila
  if (W < 0)
    return -1;

  // encontra a melhor solução possível para a subárvore
  items[i].bound = currentProfit;
  if (i<n-1)
    findBound(W, items, i, n);
  else if (W-items[i].weight >= 0)
    items[i].bound += items[i].value;

  // retorna caso a melhor solução da subárvore seja pior que uma já encontrada
  if (items[i].bound <= *bestProfit)
    return -1;

  // retorna a melhor solução
  if (i+1==n || W == 0) {
    if (W-items[i].weight >= 0 && !proibido(items, i, n)) {  // adiciona o valor do último item se não exceder a capacidade da mochila
      for (int j=i+1; j<n; j++) // apaga seleção de possíveis itens posteriores
        items[j].select=0;
      items[i].select = 1;
      *bestProfit = currentProfit + items[i].value;
      return *bestProfit;
    }
    else {
      items[i].select = 0;
      if (currentProfit > *bestProfit) {
        for (int j=i+1; j<n; j++) // apaga seleção de possíveis itens posteriores
          items[j].select=0;
        *bestProfit = currentProfit;
        return *bestProfit;
      } else return -1;
    }
  }

  // armazena valor atual do select
  int aux = items[i].select;

  // seleciona o item se não for proibido
  items[i].select = 1;
  float esq;
  if (!proibido(items, i, n))
    esq = BranchNBound(W-items[i].weight, items, i+1, n, nodes, bestProfit, currentProfit+items[i].value);
  else esq = -1;

  // não seleciona o item
  items[i].select = 0;
  float dir = BranchNBound(W, items, i+1, n, nodes, bestProfit, currentProfit);

  if (esq == *bestProfit) {
    items[i].select = 1;
    return esq;
  }
  else if (dir == *bestProfit) {
    items[i].select = 0;
    return dir;
  }
  else {
    items[i].select = aux;
    return 0;
  }
}

void printBnBSolution(Item *items, int n, int W, float bestProfit) {
  int i, j;
  Item aux;
  // reordena vetor original
  for (i=0; i<n; i++) {
    for (j=0; j<n; j++) {
      if (items[j].id == i && i!=j) {
        aux = items[i];
        items[i] = items[j];
        items[j] = aux;
      }
    }
  }
  for (i=0; i<n; i++) {
    if (items[i].select) {
      printf("%d ", items[i].id+1);
    }
  }
  printf("\n");
}

int main()
{
  int W, n, m;
  
  scanf("%d %d %d", &n, &m, &W); // lê a quantidade de itens e o tamanho da mochila

  clock_t t;
  t = clock();

  Item *items = (Item *) malloc(sizeof(Item)*n);  // inicializa vetor de itens
  for (int i=0; i<n; i++)
    items[i].pairs = calloc(n, sizeof(int));  // inicializa vetores de pares proibidos

  // lê valor e peso dos itens
  for (int i=0; i<n; i++) {
    scanf("%f", &items[i].weight);
    items[i].id = i;
  }
  for (int i=0; i<n; i++)
    scanf("%f", &items[i].value);
  
  // lê pares proibidos
  for (int i=0; i<m; i++) {
    int i1, i2;
    scanf("%d %d", &i1, &i2);
    for (int j=0; j<n; j++) {
      if (j+1==i1)
        items[j].pairs[i2-1] = 1;
      if (j+1==i2)
        items[j].pairs[i1-1] = 1;
    }
  }

  // calcula o valor por peso de cada item
  for (int i=0; i < n; i++)
    items[i].VpW = items[i].value / items[i].weight;

  // ordena vetor do maior pro menor valor por peso
  Item aux;
  for (int i=1; i<n; i++) {
    for (int j=i; j>0; j--) {
      if (items[j-1].VpW < items[j].VpW) {
        aux = items[j];
        items[j] = items[j-1];
        items[j-1] = aux;
      }
    }
  }

  // Branch and Bound
  float bestProfit=0;
  int nodes=0;

  printf("%g\n", BranchNBound(W, items, 0, n, &nodes, &bestProfit, 0));
  t = clock() - t;
  printBnBSolution(items, n, W, bestProfit);
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
  fprintf(stderr, "Número de nós: %d\n", nodes);
  fprintf(stderr, "Tempo: %f segundos\n", time_taken);

  free(items);

  return 0;
}
