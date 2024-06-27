# Threads

> Entendendo melhor o conceito de threads em sistemas operacionais

## [1/3] 📝 Conceitos básicos

🔶 **conceito de thread clássico**: A tradução para o termo thread em português está associado a fio/caminho. Então podemos dizer que uma thread é um caminho de execução de um programa que compartilha o mesmo espaço de endereçamento com outras threads de um mesmo processo. Isso implica que threads que são criadas em um mesmo processo, possuem acesso as variaveis locais do processo e todos os arquivos disponiveis pelo processo ao qual foram criadas.

## [2/3] 🧵 Threads POSIX

O padrão 1003.1c desenvolvido pelo IEEE, fornece um pacote **Pthreads** para se trabalhar com threads nos sistemas UNIX. temos várias chamadas de função para serem ultilizadas, as principais são:

- **Pthread_create**: Função para criar uma nova thread;
- **Pthread_exit**: Função para finalizar uma thread e liberar sua pilha;
- **Pthread_join**: Espera que uma thread especifica termine seu processamento antes de continuar com o seu próprio processamento.

## [3/3] 🔍 Analizando um programa usando Threads com o pacote Pthreads

Este exemplo foi obtido do livro **Sistemas Operacionais Modernos - Tanenbaum - 4 Edição** na seção - **2.2.3 Threads POSIX**:

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 10

void *print_hello_world(void *tid){
    /* Esta funcao imprime o identificador do thread e sai. */
    printf("Ola mundo. Boas vindas do thread %d\n", tid);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    /* O programa principal cria 10 threads e sai. */
    pthread_t_threads[NUMBER_OF_THREADS];
    int status, i;
    for(i=0; i < NUMBER_OF_THREADS; i++) {
        printf("Metodo Main. Criando thread %d\n", i);
        status = pthread_create(&threads[i], NULL, print_hello_world, (void *)i);
        
        if (status != 0) {
            printf("Oops. pthread create returned error code %d\n", status);
            exit(-1);
        }
    }
    exit(NULL);
}
```

O que este programa simples faz é criar 10 threads e quando cada uma thread é criada ela imprime na tela seu id e depois é encerrada. Mas temos que analisar melhor alguns trechos:

**🔑 EXPLICANDO PARTES CHAVES DO CÓDIGO**

- **status = pthread_create(&threads[i], NULL, print_hello_world, (void *)i);**: A chamada pthread_create possui alguns parâmetros, são eles:

    - 1 parâmetro: Este é um ponteiro para a variável pthread_t que representará a nova thread;

    - 2 parâmetro: Este parâmetro pode ser usado para especificar atributos da thread, como a pilha de tamanho ou a política de escalonamento. Se passado como NULL, a thread é criada com os atributos padrão;

    - 3 parâmetro: Este é um ponteiro para a função que será executada pela nova thread;

    - 4 parâmetro: Este é o argumento que será passado para a função definida no parâmetro 3.

- **if (status != 0)**: Neste trecho, fazemos uma verificação para sabermos se a thread foi criada com sucesso, neste caso se o valor do status for igual a zero, significa que a criação da thread foi realizada com sucesso, caso contrario, ou seja o valor da variavel status for diferente de zero, significa que houve um erro na criação da thread.

- **pthread_exit(NULL);**: Esta chamada faz com que a thread seja encerrada.
