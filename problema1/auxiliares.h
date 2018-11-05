#include <string.h>

int lerInteiro(){
  int valor;
  printf("Digite o valor: ");
  scanf("%d",&valor);
  return valor;
}

void lerString(char *string, int nroLetras) {
  int i = 0;
  char lixo;
  scanf("%c", string);
  if (*string != '\n')
    while ( ((string[++i] = getc(stdin) ) != '\n') && (i < nroLetras-1)) {}
  if(i == nroLetras-1) while ((lixo = getc(stdin) ) != '\n') {}
  string[i] = '\0';
}

void limparTela() {
    system("clear");
}

void pausar() {
  int i = 0;
  char lixo;
  printf("\nPressione enter para continuar...\n");
  scanf("%c", &lixo);
  while (lixo != '\n') { lixo = getc(stdin); }
}


// outros
void  limpaBuffer() {
  char lixo;
  do {
    scanf("%c",&lixo);
  }while (lixo != '\n');
}

void tiraTerminador(char *texto) {
	if(texto[strlen(texto)-1] == '\n')
		texto[strlen(texto)-1] = '\0';
}
