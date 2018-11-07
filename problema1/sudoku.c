//  para compilar e rodar: gcc sudoku.c -lpthread && ./a.out
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "auxiliares.h"
#include "colors.h"
//#include <stdbool.h>
#define VECT {0,0,0,0,0,0,0,0,0}
#define TAM 9
#define CAMPOS 3 // linhas, colunas e subgrids
#define LINS 0
#define COLS 1
#define SUB_GRID 2

static pthread_mutex_t mutexPrint = PTHREAD_MUTEX_INITIALIZER;

int grid[TAM][TAM];
// vetores dizendo se determinada regiao é válida
int campValido[CAMPOS][TAM];
int sudokuValido = 1;

// protótipos das funções / threads
void * checaLinha(void *);
void * checaColuna(void *);
void * checaSubgrid(void *);
void carregarGrid();
void printarGrid();
void * (*qualFuncao(int)) (void *);
char *qualCampo(int);

int main(int argc, char *argv[]) {
  pthread_t thread[CAMPOS][TAM];
  pthread_attr_t attrs[CAMPOS][TAM];
  int quemSou[CAMPOS][TAM], i, campo;
  // testando
  carregarGrid();
  limparTela();
  printarGrid();
  for (campo = 0; campo < CAMPOS; campo++) {
    for (i = 0; i < TAM; i++) {
      campValido[campo][i] = 1; // setup
    }
  }
  for (campo = 0; campo < CAMPOS; campo++) {
    for (i = 0; i < TAM; i++) {
      quemSou[campo][i] = i;
      pthread_attr_init(&attrs[campo][i]);
      pthread_create(&thread[campo][i], &attrs[campo][i], qualFuncao(campo), &quemSou[campo][i]);
    }
  }

  for (campo = 0; campo < CAMPOS; campo++) {
    for (i = 0; i < TAM; i++) {
      pthread_join(thread[campo][i], NULL);
      //printf("esperando thread %d terminar\n", i);
    }
  }
  printf("\n\nFIM!!!\n\n");
  printf("o sudoku eh %s ", (sudokuValido==1) ? "valido" : "invalido");
  for (campo = 0; campo < CAMPOS; campo++) {
    for (i = 0; i < TAM; i++) {
      if(!campValido[campo][i]) {
        printf("na %s %d\n", qualCampo(campo), i+1);
      }
    }
  }
  printf("\n");
  return 0;
}


// ========  declaração das funções  ============

void printarGrid() {
  int linha, coluna;
  for (linha = 0; linha < TAM; linha++) {
    for (coluna = 0; coluna < TAM; coluna++) {
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
    //qualArq = lerInteiro();
    qualArq = 1;
  } while((qualArq < 1) || (qualArq > 10));

  sprintf(nomeArq, "sudokus/%d.txt", qualArq);

  arq = fopen(nomeArq, "r");
  if (!arq) {
    printf("Erro ao ler arquivo!\n");
    pausar();
    return ;
  }

  for (linha = 0; linha < TAM; linha++) {
    for (coluna = 0; coluna < TAM; coluna++) {
      fscanf(arq, "%d", &grid[linha][coluna]);
    }
  }

  fclose(arq);
}
/*
void * checaLinha(void *);
void * checaColuna(void *);
void * checaSubgrid(void *);
*/
void *checaLinha(void *l) {
  int i;
  int *linha = l;

  campValido[LINS][*linha] = 1;
  int vezesQaparece[] = VECT;

  for (i = 0; i < TAM; i++) {
    vezesQaparece[ grid[*linha][i]-1 ]++;
  }
  for (i = 0; i < TAM; i++) {
    if (vezesQaparece[i] != 1) {
      printf("\nNa linha\n");
      campValido[LINS][*linha] = 0;
      sudokuValido = 0;
      break;
    }
  }
  /*pthread_mutex_lock(&mutexPrint);
  printf("thread linha %d pronto\n", *linha);
  pthread_mutex_unlock(&mutexPrint);*/
  return NULL;
}
void *checaColuna(void *c) {
  int i;
  int *coluna = c;

  campValido[*coluna][COLS] = 1;
  int vezesQaparece[] = VECT;

  for (i = 0; i < TAM; i++) {
    vezesQaparece[ grid[i][*coluna]-1 ]++;
  }
  for (i = 0; i < TAM; i++) {
    if (vezesQaparece[i] != 1) {
      printf("\nNa coluna %d valor %d repete\n", *coluna + 1, i+1);
      campValido[*coluna][COLS] = 0;
      sudokuValido = 0;
      break;
    }
  }
  /*pthread_mutex_lock(&mutexPrint);
  printf("thread coluna %d pronto\n", *coluna);
  pthread_mutex_unlock(&mutexPrint);*/
  return NULL;
}
void *checaSubgrid(void *g) {
  int i;
  int *subGrid = g;

  for (i = 0; i < TAM; i++) {
    //
  }
  /*pthread_mutex_lock(&mutexPrint);
  printf("thread subGrid %d pronto\n", *subGrid);
  pthread_mutex_unlock(&mutexPrint);*/
  return NULL;
}

void * (*qualFuncao(int campo)) (void * c) {
  //printf("esse: %d\n", campo);
  switch (campo) {
    case LINS:
      return checaLinha;
    case COLS:
      return checaColuna;
    case SUB_GRID:
      return checaSubgrid;
  }
}

char *qualCampo(int campo) {
  char *nome = (char *)malloc(20);
  if (campo == LINS) {
    strcpy(nome, "linha");
    return nome;
  }
  if (campo == COLS) {
    strcpy(nome, "coluna");
    return nome;
  }
  if (campo == SUB_GRID) {
    strcpy(nome, "subgrid");
    return nome;
  }
  printf("nao identificado\n");
}
