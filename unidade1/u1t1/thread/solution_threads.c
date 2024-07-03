#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

typedef struct{
    int linhas;
    int colunas;
    int **matriz_imagem;
}Imagem_data;

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

void get_matriz_img(Imagem_data *img){
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

  img->linhas = linhas;
  img->colunas = colunas;

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

  img->matriz_imagem = matriz_img;
}

void* processamento_eixo_x(void *args){
    Imagem_data *dados = (Imagem_data *)args;
    printf("thread do processamento x!");
    /*
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
    */
    pthread_exit(NULL);
}

void* processamento_eixo_y(void * args){
    Imagem_data *dados = (Imagem_data *)args;
    printf("thread do processamento y!");

    /*
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
    */
    pthread_exit(NULL);
}

void* thread_mae_routine(){
  Imagem_data dados;
  get_matriz_img(&dados);
  pthread_t thread_x, thread_y; 

  // criando a thread filha para o processamento da imagem no eixo x
  if(pthread_create(&thread_x, NULL, processamento_eixo_x,(void *) &dados)){ // a thread será criada e será executada a função processamento_eixo_x
    fprintf(stderr, "Erro ao criar a thread_x\n"); // caso a criação não ocorra com sucesso o retorno da função pthread_create será 1 e entrará no if imprimindo a mensagem de erro
  }

  // aguardando a thread_x terminar seu processamento para que a thread_y possa começar o seu processamento
  if (pthread_join(thread_x, NULL)) {
        fprintf(stderr, "Erro ao aguardar a thread_x\n");
        exit(1);
    }

  // criando a thread filha para o processamento da imagem no eixo y
  if(pthread_create(&thread_y, NULL, processamento_eixo_y,(void *) &dados)){ // a thread será criada e será executada a função processamento_eixo_x
    fprintf(stderr, "Erro ao criar a thread_y\n"); // caso a criação não ocorra com sucesso o retorno da função pthread_create será 1 e entrará no if imprimindo a mensagem de erro
  }

  // aguardando a thread_y terminar seu processamento para que a thread_mae possa obter a imagem a partir da matriz_img resultante dos dois processamentos
  if (pthread_join(thread_y, NULL)) {
        fprintf(stderr, "Erro ao aguardar a thread_y\n");
        exit(1);
    }

  //gerando um arquivo com os dados processados
  FILE *arquivo_final = fopen("arquivo_final.ascii.pgm", "w");
  
  if(arquivo_final == NULL){
    printf("Erro em abrir o arquivo_final!");
  }else{
    //incluir as linhas bases
    fprintf(arquivo_final,"P2\n");
    fprintf(arquivo_final,"# fred.pgm created by PGMA_IO::PGMA_WRITE.\n");
    fprintf(arquivo_final,"%d %d\n", 300, 246);
    fprintf(arquivo_final, "%d\n", 255);

    // varendo cada pixel da imagem e realizando a escrita no arquivo final
    for(int i=0; i<dados.linhas; i++){
        for(int j=0; j<dados.colunas; j++){
            // ajustando valores
            if(dados.matriz_imagem[i][j] > 255){
                // escrevendo o valor resultante no arquivo_final
                fprintf(arquivo_final, "%d ", 255);
            }else{
                // escrevendo o valor resultante no arquivo_final
                fprintf(arquivo_final, "%d ", dados.matriz_imagem[i][j]);
            }
            
            
        }
    }
  }

    pthread_exit(NULL);
}

int main(void){
  pthread_t thread_mae;
  
  // criando a thread mãe do algoritimo, responsável por gerar a matriz imagem e administrar as threads filhas
  if(pthread_create(&thread_mae, NULL, thread_mae_routine, NULL)){ // a thread será criada
    fprintf(stderr, "Erro ao criar a thread mãe\n"); // caso a criação não ocorra com sucesso o retorno da função pthread_create será 1 e entrará no if imprimindo a mensagem de erro
    return 1;
  }

  // aguardando a thread_mae terminar
  if (pthread_join(thread_mae, NULL)) {
        fprintf(stderr, "Erro ao aguardar a thread mãe\n");
        exit(1);
    }
    
  printf("programa finalizado!");
  
  return 0;
}