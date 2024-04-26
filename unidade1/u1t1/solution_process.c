#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
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

    //processo filho para detectar bordas no eixo x
    

    //processo filho para detectar bordas no eixo y


    return 0;
}