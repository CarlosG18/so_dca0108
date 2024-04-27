#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void){
  int pid1, pid2, filho1, filho2, estado1, estado2;
  
  //primeira parte - alocando uma matriz com os valores da imagem
  FILE *arquivo = fopen("coins.ascii.pgm","r");
  int larg, alt, linhas;
  int colunas = 12; //numero de bits que serao usados para representar um pixel

  char buffer[256];

  int **matriz_img;

  if(arquivo == NULL){
    printf("erro na abertura de arquivo");
  }else{
    //desconsiderando algumas linhas
    fgets(buffer, sizeof(buffer), arquivo);
    fgets(buffer, sizeof(buffer), arquivo);
    fscanf(arquivo, "%d %d", &larg, &alt); //obtendo o tamanho da imagem 
    fgets(buffer, sizeof(buffer), arquivo);
    fgets(buffer, sizeof(buffer), arquivo);

    linhas = (larg*alt)/colunas;

    //alocando as linhas da matriz_img
    matriz_img = (int **)malloc(linhas * sizeof(int *));

    //alocando cada coluna de cada linha da matriz_img
    for(int i =0; i<linhas; i++){
      matriz_img[i] = (int *)malloc(colunas * sizeof(int));
    }

    //add valores da imagem na matriz_img
    for(int i = 0; i<linhas; i++){
      for(int j=0;j<colunas;j++){
        int valor;
          fscanf(arquivo, "%d", &valor);
          matriz_img[i][j] = valor;
      }
    }

  }
  fclose(arquivo);
  
  //criando o primeiro processo filho que gerará o arquivo com a imagem processada no eixo x
  pid1 = fork();
    
  if(pid1 < 0){
    printf("Erro na criação do processo filho!");
    exit(-1);
  }
    
  if(pid1 == 0){
    printf("processo filho 1 PID = %d\n", getpid());
    //obtendo a imagem com o processamento de deteccao de borda do eixo x
    FILE *arquivo_x = fopen("coins_x.ascii.pgm", "w");
    
    if(arquivo_x == NULL){
      printf("erro na abertura de arquivo");
    }else{
      for(int i=1; i<linhas-2; i++){
        for(int j=1; j<colunas-2; j++){
          matriz_img[i][j] = (-matriz_img[i-1][j-1] - matriz_img[i][j-1] - matriz_img[i+1][j-1]) + (matriz_img[i-1][j+1] + matriz_img[i][j+1] + matriz_img[i+1][j+1]);
          
          if(matriz_img[i][j] > 255){
            matriz_img[i][j] = 255;
          }
          
          if(matriz_img[i][j] < 0){
            matriz_img[i][j] = -1*matriz_img[i][j];
          }
          
          fprintf(arquivo_x, "%d ", matriz_img[i][j]);
        }
      }
    }
    fclose(arquivo_x);
  }else{
    filho1 = wait(&estado1);
    //printf("\nProcesso pai: PID = %d", getpid());
    //printf("\nFilho 1 PID = %d finalizado!", filho1);
    exit(0);
  }
  
  //criando o processo do filho 2 para realizar o processamento na imagem no eixo y
  pid2 = fork();
  
  if(pid2 < 0){
    printf("Erro na criação do processo filho!");
    exit(-1);
  }
  
  if(pid2 == 0){
    printf("processo filho 2 PID = %d\n", getpid());
    //obtendo a imagem com o processamento de deteccao de borda do eixo y
    FILE *arquivo_y = fopen("coins_y.ascii.pgm", "w");
    
    if(arquivo_y == NULL){
      printf("erro na abertura de arquivo");
    }else{
      for(int i=1; i<linhas-2; i++){
        for(int j=1; j<colunas-2; j++){
          matriz_img[i][j] = (-matriz_img[i-1][j-1] - matriz_img[i-1][j] - matriz_img[i-1][j+1]) + (matriz_img[i+1][j-1] + matriz_img[i+1][j] + matriz_img[i+1][j+1]);
          
          if(matriz_img[i][j] > 255){
            matriz_img[i][j] = 255;
          }
          
          if(matriz_img[i][j] < 0){
            matriz_img[i][j] = -1*matriz_img[i][j];
          }
          
          fprintf(arquivo_y, "%d ", matriz_img[i][j]);
        }
      }
    }
    fclose(arquivo_y);
  }else{
    filho2 = wait(&estado2);
    //printf("\nProcesso pai: PID = %d", getpid());
    //printf("\nFilho 2 PID = %d finalizado!", filho2);
    exit(0);
  }
  
  //unindo os arquivos
  FILE *arquivo_final = fopen("arquivo_final.ascii.pgm", "w");
  FILE *arquivo_X = fopen("coins_x.ascii.pgm", "r");
  FILE *arquivo_Y = fopen("coins_y.ascii.pgm", "r");
  
  if(arquivo_final == NULL){
    printf("Erro em abrir o arquivo_final!");
  }else{
    printf("abertura do arquivo_final ok!\n");
    //incluir as linhas bases
    fprintf(arquivo_final,"P2\n");
    fprintf(arquivo_final,"# fred.pgm created by PGMA_IO::PGMA_WRITE.\n");
    fprintf(arquivo_final,"%d %d\n", 300, 246);
    fprintf(arquivo_final, "%d\n", 255);
    
    if(arquivo_X == NULL && arquivo_Y == NULL){
      printf("Erro em abrir o arquivo_x ou arquivo_y!");
    }else{
      printf("abertura do arquivo_x e y ok!\n");
      for(int i=1; i<linhas-2; i++){
        for(int j=1; j<colunas-2; j++){
          int valueX, valueY, sumValues;
          fscanf(arquivo_X,"%d", &valueX);
          fscanf(arquivo_Y,"%d", &valueY);
          sumValues = valueX + valueY;
          
          if(sumValues > 255){
            sumValues = 255;
          }
          
          fprintf(arquivo_final, "%d ", sumValues);
        }
      }
    }
  }
  fclose(arquivo_X);
  printf("fechadura X ok!\n");
  fclose(arquivo_Y);
  printf("fechadura Y ok!\n");
  fclose(arquivo_final);
  printf("fechadura final ok!\n");
  return 0;
}