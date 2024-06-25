#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>

int** create_matriz(int linhas, int colunas){
  /*
    função para alocar uma matriz de tamanho linhas x colunas

      args:
      - linhas: número de linhas da matriz
      - colunas: número de colunas da matriz

      return:
      - matriz: ponteiro para ponteiro da matriz[linhas][colunas]
  */

  int **matriz;
  //alocando cada linha da matriz
  matriz = (int **)malloc(linhas * sizeof(int *));
  //alocando cada coluna de cada linha da matriz_img
  matriz[0] = (int *)malloc(colunas * linhas * sizeof(int));
  for(int i = 1; i<linhas; i++){
    matriz[i] = matriz[i-1] + colunas;
  }
  return matriz;
}

int main(void){
  int pid1, pid2, filho1, filho2, estado1, estado2; // variaveis auxiliares para a criação dos processos
  
  //primeira parte - alocando uma matriz com os valores da imagem
  FILE *arquivo = fopen("coins.ascii.pgm","r"); // obtendo um ponteiro para o arquivo base
  int colunas, linhas;

  char buffer[256];

  if(arquivo == NULL){
    printf("erro na abertura de arquivo");
  }else{
    //desconsiderando algumas linhas que não são importantes para o programa
    fgets(buffer, sizeof(buffer), arquivo);
    fgets(buffer, sizeof(buffer), arquivo);
    fscanf(arquivo, "%d %d", &colunas, &linhas); //obtendo o tamanho da imagem 
    fgets(buffer, sizeof(buffer), arquivo);
    fgets(buffer, sizeof(buffer), arquivo);
  }

  int **matriz_img = create_matriz(linhas, colunas); // criando uma matriz do tamanho da imagem

  //add valores da imagem na matriz_img
  for(int i = 0; i<linhas; i++){
    for(int j=0;j<colunas;j++){
      int valor;
        fscanf(arquivo, "%d", &valor);
        matriz_img[i][j] = valor;
    }
  }
  
  fclose(arquivo);
  
  //criando o primeiro processo filho que gerará o arquivo com a imagem processada no eixo x
  pid1 = fork(); // criando o processo 1
  
  // verificação se o processo 1 foi criado com sucesso
  if(pid1 < 0){
    printf("Erro na criação do processo filho!");
    exit(-1);
  }

  // inicio do processo 1 
  if(pid1 == 0){
    int **matriz_x = create_matriz(linhas, colunas); // criando uma matriz com os valores processados para o eixo x, para auxiliar nos cálculos
    FILE *arquivo_x = fopen("coins_x.ascii.pgm", "w"); // criando um arquivo para armazenar os valores obtidos da convolução da imagem no eixo x
    
    //incluir as linhas bases
    fprintf(arquivo_x,"P2\n");
    fprintf(arquivo_x,"# fred.pgm created by PGMA_IO::PGMA_WRITE.\n");
    fprintf(arquivo_x,"%d %d\n", 300, 246);
    fprintf(arquivo_x, "%d\n", 255);

    if(arquivo_x == NULL){
      printf("erro na abertura de arquivo");
    }else{  
      for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
          if(i == 0 || j == 0 || i == linhas-1 || j == colunas-1){
            fprintf(arquivo_x, "%d  ", matriz_img[i][j]);
          }else{
            // realizando a operação de convolução para o eixo x
            int parcela1 = matriz_img[i-1][j-1] + matriz_img[i][j-1] + matriz_img[i+1][j-1];
            int parcela2 = matriz_img[i-1][j+1] + matriz_img[i][j+1] + matriz_img[i+1][j+1];
            matriz_x[i][j] = parcela2 - parcela1;
          
            // ajustando limites de valores
            if(matriz_x[i][j] > 255){
              matriz_x[i][j] = 255;
            }
            
            // ajustando limites de valores
            if(matriz_x[i][j] < 0){
              matriz_x[i][j] = -1*matriz_x[i][j];
              if(matriz_x[i][j] > 255){
                matriz_x[i][j] = 255;
              }
            }
            // armazenando no arquivo_x o valor em sua posição correspondente
            fprintf(arquivo_x, "%d  ", matriz_x[i][j]);
          }
        }
        fprintf(arquivo_x, "\n");
      }
    }
    // fechando o arquivo e liberando memoria da matriz x
    fclose(arquivo_x);
    free(matriz_x[0]);
    free(matriz_x);
  }else{ 
    filho1 = wait(&estado1); // enquanto o processo não finalizar ele ficará aguardando
    exit(0); // processo finalizado
  }
  
  //criando o processo do filho 2 para realizar o processamento na imagem no eixo y
  pid2 = fork(); // criando o processo 2
  
  // verificação se o processo 2 foi criado com sucesso
  if(pid2 < 0){
    printf("Erro na criação do processo filho!");
    exit(-1);
  }
  
  // inicio do processo 2
  if(pid2 == 0){
    int **matriz_y = create_matriz(linhas, colunas); // criando uma matriz com os valores processados para o eixo y, para auxiliar nos cálculos
    FILE *arquivo_y = fopen("coins_y.ascii.pgm", "w"); // criando um arquivo para armazenar os valores obtidos da convolução da imagem no eixo y

    //incluir as linhas bases
    fprintf(arquivo_y,"P2\n");
    fprintf(arquivo_y,"# fred.pgm created by PGMA_IO::PGMA_WRITE.\n");
    fprintf(arquivo_y,"%d %d\n", 300, 246);
    fprintf(arquivo_y, "%d\n", 255);

    if(arquivo_y == NULL){
      printf("erro na abertura de arquivo");
    }else{
      for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
          if(i == 0 || j == 0 || i == linhas-1 || j == colunas-1){
            fprintf(arquivo_y, "%d  ", matriz_img[i][j]);
          }else{
            // realizando a operação de convolução para o eixo y
            int parcela1 = matriz_img[i-1][j-1] + matriz_img[i-1][j] + matriz_img[i-1][j+1];
            int parcela2 = matriz_img[i+1][j-1] + matriz_img[i+1][j] + matriz_img[i+1][j+1];
            matriz_y[i][j] = parcela2 - parcela1;
            
            // ajustando limites de valores
            if(matriz_y[i][j] > 255){
              matriz_y[i][j] = 255;
            }
            
            // ajustando limites de valores
            if(matriz_y[i][j] < 0){
              matriz_y[i][j] = -1*matriz_y[i][j];
              if(matriz_y[i][j] > 255){
                matriz_y[i][j] = 255;
              }
            }
            // armazenando no arquivo_y o valor em sua posição correspondente
            fprintf(arquivo_y, "%d  ", matriz_y[i][j]);
          }
        }
        fprintf(arquivo_y, "\n");
      }
    }
    // fechando o arquivo e liberando memoria da matriz y
    fclose(arquivo_y);
    free(matriz_y[0]);
    free(matriz_y);
  }else{
    filho2 = wait(&estado2); // enquanto o processo não finalizar ele ficará aguardando
    exit(0); // processo finalizado
  }
  
  //unindo os arquivos
  FILE *arquivo_final = fopen("arquivo_final.ascii.pgm", "w"); // criando o arquivo final que conterá a junção dos arquivo_x e arquivo_y
  FILE *arquivo_X = fopen("coins_x.ascii.pgm", "r"); // abrindo o arquivo_x
  FILE *arquivo_Y = fopen("coins_y.ascii.pgm", "r"); // abrindo o arquivo_y
  
  if(arquivo_final == NULL){
    printf("Erro em abrir o arquivo_final!");
  }else{
    //incluir as linhas bases
    fprintf(arquivo_final,"P2\n");
    fprintf(arquivo_final,"# fred.pgm created by PGMA_IO::PGMA_WRITE.\n");
    fprintf(arquivo_final,"%d %d\n", 300, 246);
    fprintf(arquivo_final, "%d\n", 255);
    
    if(arquivo_X == NULL && arquivo_Y == NULL){ // verificando a existência dos arquivos x e y
      printf("Erro em abrir o arquivo_x ou arquivo_y!");
    }else{
      //desconsiderando algumas linhas arquivo x e do arquivo y
      for(int i=0; i<3; i++){
        fgets(buffer, sizeof(buffer), arquivo_X);
        fgets(buffer, sizeof(buffer), arquivo_Y);
      }

      // realizando a junção através da operação de módulo
      for(int i=0; i<linhas; i++){
        for(int j=0; j<colunas; j++){
          int valueX, valueY, sumValues;
          fscanf(arquivo_X,"%d", &valueX);
          fscanf(arquivo_Y,"%d", &valueY);
          sumValues = sqrt((valueX * valueX) + (valueY * valueY));
          
          // ajustando valores
          if(sumValues > 255){
            sumValues = 255;
          }
          
          // escrevendo o valor resultante no arquivo_final
          fprintf(arquivo_final, "%d ", sumValues);
        }
      }
    }
  }
  
  // fechando o arquivo e liberando memoria da matriz img
  free(matriz_img[0]);
  free(matriz_img);
  return 0;
}