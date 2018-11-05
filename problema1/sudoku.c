//  para compilar e rodar: gcc sudoku.c -lpthread && ./a.out
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "auxiliares.h"
#include "colors.h"
//#include <stdbool.h>

int grid[9][9];

// protótipos das funções / threads
int checaLinha();
int checaColuna();
int checaSubgrid();
void carregarGrid();
void printarGrid();

int main(int argc, char *argv[]) {

  // testando
  carregarGrid();
  limparTela();
  printarGrid();

  return 0;
}


// ========  declaração das funções  ============

void printarGrid() {
  int linha, coluna;
  for (linha = 0; linha < 9; linha++) {
    for (coluna = 0; coluna < 9; coluna++) {
      printf("%d ", grid[linha][coluna]);
    }
    printf("\n");
  }
}

void carregarGrid() {
  FILE *arq = NULL;
  char nomeArq[50];
  int qualArq, linha, coluna;
  limparTela();
  printf("Qual arquivo quer ler?\n");
  do {
    qualArq = lerInteiro();
  } while((qualArq < 1) || (qualArq > 10));

  sprintf(nomeArq, "sudokus/%d.txt", qualArq);

  arq = fopen(nomeArq, "r");
  if (!arq) {
    printf("Erro ao ler arquivo!\n");
    pausar();
    return ;
  }

  for (linha = 0; linha < 9; linha++) {
    for (coluna = 0; coluna < 9; coluna++) {
      fscanf(arq, "%d", &grid[linha][coluna]);
    }
  }

  fclose(arq);
}
