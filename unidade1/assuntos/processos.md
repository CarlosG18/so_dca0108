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



## [2/3] 🔧 Como usar?

🔶 **criar um novo processo**: Nos sistemas UNIX, temos apenas uma chamada de sistema para a criação de processos: **fork**. Com essa chamada realizada, um processo filho é criado, sendo um clone exato do processo pai onde possuirá a mesma imagem de memória, as mesmas variáves de ambiente e os mesmos arquivos abertos.

⚠️ **obs**: após a criação do processo, o processo pai e o processo filho possuem seus próprios espaços de endereçamento distintos.

🔶 **finalizar um processo**: A chamada para finalizar um processo é a **exit**.

### 🔍 Exemplo básico

Vamos aplicar os conhecimentos adquiridos nesse readme e fazer um exemplo simples para entendermos os **processos** na prática.

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