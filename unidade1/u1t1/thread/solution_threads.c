#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

// criando as variaveis globais do sistema
int** matriz_I;
int** matriz_x;
int** matriz_y;
int linhas, colunas;

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

void get_matriz_img(){
  /*
    função para obter os valores de um arquivo (.pgm) e atribuir a matriz_I

      args:
      - NULL

      return:
      - NULL
  */

  FILE *arquivo = fopen("coins.ascii.pgm","r"); // obtendo um ponteiro para o arquivo base

  char buffer[256];

  if(arquivo == NULL){ // verificando a abertura do arquivo
    printf("erro na abertura de arquivo");
  }else{
    //desconsiderando algumas linhas que não são importantes para o programa
    fgets(buffer, sizeof(buffer), arquivo);
    fgets(buffer, sizeof(buffer), arquivo);
    fscanf(arquivo, "%d %d", &colunas, &linhas); //obtendo o tamanho da imagem 
    fgets(buffer, sizeof(buffer), arquivo);
    fgets(buffer, sizeof(buffer), arquivo);
  }

  matriz_I = create_matriz(linhas, colunas); // alocando memoria para a matriz_I

  //add valores da imagem na matriz_I
  for(int i = 0; i<linhas; i++){
    for(int j=0;j<colunas;j++){
      int valor;
        fscanf(arquivo, "%d", &valor);
        matriz_I[i][j] = valor;
    }
  }
  fclose(arquivo); // fechando o arquivo
}

void* processamento_eixo_x(){
  /*
  função para calcular a convolução no eixo x e atribuir a matriz_x - THREAD_ => thread filha da thread_mae

    args:
    - NULL

    return:
    - NULL
  */

  matriz_x = create_matriz(linhas, colunas); // alocando memória para a matriz_x

  // percorrendo a matriz_I
  for(int i = 0; i < linhas; i++){
    for(int j = 0; j < colunas; j++){
        if(i == 0 || j == 0 || i == linhas-1 || j == colunas-1){
            matriz_x[i][j] = matriz_I[i][j];
        }else{
            // realizando a operação de convolução para o eixo x
            int parcela1 = matriz_I[i-1][j-1] + matriz_I[i][j-1] + matriz_I[i+1][j-1];
            int parcela2 = matriz_I[i-1][j+1] + matriz_I[i][j+1] + matriz_I[i+1][j+1];
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
        }
    }
  }
  pthread_exit(NULL); // finalizando a thread filha x
}

void* processamento_eixo_y(){  
  /*
  função para calcular a convolução no eixo y e atribuir a matriz_y - THREAD_Y => thread filha da thread_mae

    args:
    - NULL

    return:
    - NULL
  */
  
  matriz_y = create_matriz(linhas,colunas); // alocando memória para a matriz_y

  // percorrendo a matriz_I
  for(int i = 0; i < linhas; i++){
    for(int j = 0; j < colunas; j++){
      if(i == 0 || j == 0 || i == linhas-1 || j == colunas-1){
        matriz_y[i][j] = matriz_I[i][j];
      }else{
        // realizando a operação de convolução para o eixo y
        int parcela1 = matriz_I[i-1][j-1] + matriz_I[i-1][j] + matriz_I[i-1][j+1];
        int parcela2 = matriz_I[i+1][j-1] + matriz_I[i+1][j] + matriz_I[i+1][j+1];
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
      }
    }
  }

  pthread_exit(NULL); // finalizando a thread filha y
}

void* thread_mae_routine(){
  /*
  rotina da thread mãe, onde será obtido a matriz_I e criado as threads filhas para o processamento da imagem.

    args:
    - NULL

    return:
    - NULL
  */

  get_matriz_img(); // obtenção da matriz_I a partir do arquivo .pgm
  pthread_t thread_x, thread_y; // definindo as variaveis correspondentes as threads filhas do tipo pthread_t

  // criando a thread filha para o processamento da imagem no eixo x
  if(pthread_create(&thread_x, NULL, processamento_eixo_x,NULL)){ // a thread será criada e será executada a função processamento_eixo_x
    fprintf(stderr, "Erro ao criar a thread_x\n"); // caso a criação não ocorra com sucesso o retorno da função pthread_create será 1 e entrará no if imprimindo a mensagem de erro
  }

  // aguardando a thread_x terminar seu processamento para que a thread_y possa começar o seu processamento
  if (pthread_join(thread_x, NULL)) {
        fprintf(stderr, "Erro ao aguardar a thread_x\n");
        exit(1);
  }

  // criando a thread filha para o processamento da imagem no eixo y
  if(pthread_create(&thread_y, NULL, processamento_eixo_y,NULL)){ // a thread será criada e será executada a função processamento_eixo_x
    fprintf(stderr, "Erro ao criar a thread_y\n"); // caso a criação não ocorra com sucesso o retorno da função pthread_create será 1 e entrará no if imprimindo a mensagem de erro
  }
  
  // aguardando a thread_y terminar seu processamento para que a thread_mae possa obter a imagem a partir da matriz_img resultante dos dois processamentos
  if (pthread_join(thread_y, NULL)) {
        fprintf(stderr, "Erro ao aguardar a thread_y\n");
        exit(1);
  }
  
  //gerando um arquivo com os dados processados
  FILE *arquivo_final = fopen("arquivo_final.ascii.pgm", "w");
  
  if(arquivo_final == NULL){ // verificando criação do novo arquivo
    printf("Erro em abrir o arquivo_final!");
  }else{
    //incluir as linhas bases
    fprintf(arquivo_final,"P2\n");
    fprintf(arquivo_final,"# fred.pgm created by PGMA_IO::PGMA_WRITE.\n");
    fprintf(arquivo_final,"%d %d\n", colunas, linhas);
    fprintf(arquivo_final, "%d\n", 255);

  // percorrendo as matrizes x e y
  for(int i=0; i<linhas; i++){
    for(int j=0; j<colunas; j++){
      int valueX = matriz_x[i][j]; // obtendo o valor correspondente a posição da linha i e coluna j da matriz_x
      int valueY = matriz_y[i][j]; // obtendo o valor correspondente a posição da linha i e coluna j da matriz_y
      int sumValues = valueX + valueY; // somando os valores para ser atribuido ao arquivo final
          
      // ajustando valores
      if(sumValues > 255){
        sumValues = 255;
      }
          
      // escrevendo o valor resultante no arquivo_final
      fprintf(arquivo_final, "%d ", sumValues);
    }
  }

  pthread_exit(NULL); // finalizando a thread mãe
  }
}

int main(void){
  pthread_t thread_mae; // definindo a variavel conrrespondente a thread mãe
  
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