# Processos

> entendendo melhor o conceito de processos em sistemas operacionais

## [1/3] 📝 Conceitos básicos

🔶 **conceito de processo**: Um processo em sistemas operacionais pode ser entendido como uma instância em execução de um programa. Quando um processo é criado, o sistema operacional aloca recursos para ele, como memória, contador de programa, variáveis e outros dados necessários para sua execução. O processo herda as características do programa do qual foi criado, mas também pode ter seu próprio estado e recursos exclusivos, como seu próprio espaço de endereçamento de memória.

🔶 **diferença entre Processo x Programa**: Um programa é um conjunto de instruções armazenadas em disco que realiza um procedimento específico, enquanto um processo é uma instância em execução desse programa, com seus próprios recursos alocados, como CPU, memória e pilha de execução.

> ⌨ o comando `ps` no linux lista os processos que estão atualmente em execução em sua máquina.

### 🔶 PID

**Process id (PID)** é um id que um processo recebe quando ele é criado pelo sistema operacional ou pelo usuario. podemos obter o pid de um processo usando alguns métodos do tipo `pid_t` na linguagen **C**:

```c
#include <unistd.h>

pid_t getpid(); -> retorna o id do processo filho.
pid_t getppid(); -> retorna o id do processo pai
```

### 🔶 Estado de um processo

Um processo pode está em um dos três estados definidos abaixo:

1. **Em execução**: nesse estado o processo está usando a CPU naquele instante.
2. **Pronto**: o processo está executável porém no momento parado para que outro processo seja executado.
3. **Bloqueado**: O processo não pode ser executado, mesmo que a CPU estiver ociosa. isso geralmente ocorre quando o processo nescessita de uma entrada que esta indisponivel ou quando o sistema operacional decide naquele momento usar a CPU para executar outro processo.

## [2/3] 🔧 Como usar?

🔶 **criar um novo processo**: Nos sistemas UNIX, temos apenas uma chamada de sistema para a criação de processos: **fork**. Com essa chamada realizada, um processo filho é criado, sendo um clone exato do processo pai onde possuirá a mesma imagem de memória, as mesmas variáves de ambiente e os mesmos arquivos abertos.

⚠️ **obs**: após a criação do processo, o processo pai e o processo filho possuem seus próprios espaços de endereçamento distintos.

🔶 **finalizar um processo**: A chamada para finalizar um processo é a **exit**. Quando chamada, ela encerra imediatamente o processo, incluindo todos os seus processos filhos. Portanto, é importante ter cuidado ao usá-la, especialmente em processos pai que possuem vários processos filhos em execução ou a serem executados. Se a intenção não for encerrar todo o conjunto de processos (pai e filhos), deve-se tomar cuidado para chamar exit() apenas no processo desejado e tomar as medidas necessárias para garantir que os outros processos continuem a ser executados corretamente. Caso contrário, a execução dos processos restantes pode ser interrompida de forma inesperada.

### 🔍 Exemplo básico

Vamos aplicar os conhecimentos adquiridos nesse readme e fazer um exemplo simples para entendermos os **processos** na prática.

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void){
  
    printf("(fora) PID do processo pai = %d\n", getpid());

    int pid = fork();

    if(pid < 0){
        printf("erro ao criar o processo filho!");
    }else if(pid == 0){
        printf("(dentro) PID do processo pai = %d\n", getppid());
        printf("processo filho criado com sucesso!\n");
        printf("PID do processo filho = %d", getpid());
        exit();
    }
  
    return 0;
}
```

**OUTPUT**:

```txt
(fora) PID do processo pai = 16370
(dentro) PID do processo pai = 16370
processo filho criado com sucesso!
```

Neste exemplo, mostramos o PID do processo pai e criamos um processo filho com o uso da diretiva `fork`. quando usamos o fork, ele retorna o valor **inteiro 0** se o processo filho for criado com sucesso ou retorna **-1** caso contrario. dentro do `else if(pid == 0)` irá conter o programa que o processo filho executará. Dentro do programa do processo filho, printamos novamento o **PID** do processo pai e o **PID** do processo filho. O processo filho é encerrado ao exercutarmos a chamada da diretiva `exit()`.

**alguns exemplos mais elaborados serão abordados em assuntos mais a frete do curso**.

## [3/3] 🎯 Para que usamos processos?

Os processos são elementos fundamentais em sistemas operacionais e podemos alencar alguns pontos positivos e negativos de se usa-ló:

### 👍 Vantangens

1. **Concorrência**: Os processos permitem a execução simultânea de várias tarefas, melhorando a eficiência do sistema operacional.
2. **Isolamento**: Cada processo é isolado dos outros, o que aumenta a segurança e a estabilidade do sistema, já que um processo não pode interferir diretamente no funcionamento de outro.
3. **Compartilhamento de recursos**: Apesar do isolamento, os processos podem compartilhar recursos, como arquivos e dispositivos de entrada/saída, por meio de mecanismos de comunicação entre processos.
4. **Facilidade de programação**: O uso de processos simplifica a programação, pois cada processo pode ser desenvolvido e testado separadamente, facilitando a manutenção e a depuração do código.

### 👎 Desvantagens

1. **Overhead**: Cada processo requer recursos do sistema, como memória e tempo de CPU, o que pode resultar em um overhead significativo em sistemas com muitos processos.
2. **Comutação de contexto**: A troca entre processos (comutação de contexto) tem um custo em termos de tempo e recursos do sistema, o que pode impactar a eficiência em sistemas com muitas trocas de contexto.
3. **Sincronização**: Em sistemas com múltiplos processos, a sincronização entre processos pode ser complexa e propensa a erros, especialmente em casos de acesso concorrente a recursos compartilhados.
4. **Complexidade de gerenciamento**: Gerenciar muitos processos pode ser complexo, requerendo algoritmos de escalonamento e gerenciamento de memória eficientes para garantir um desempenho adequado do sistema.
