#include <stdio.h>

int main(void){
  FILE *arquivo;
  char nome_do_arquivo[] = "coins.png";
  char linha[100];
  
  arquivo = fopen(nome_do_arquivo,"r");
  if(arquivo == NULL){
    printf("Erro em abrir o arquivo! \n");
  }else{
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        printf("%s", linha);
    }
  }
  
  fclose(arquivo);
  return 0;
}

//pegar a img e representar como matriz