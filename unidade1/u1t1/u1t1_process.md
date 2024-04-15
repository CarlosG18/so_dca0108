# Solução usando processos

> Algoritmo para detecção de borda em imagens usando o conceito de processos

## 🤔 Problemática

Elabore um programa, escrito em **C** ou **C++**, em um sistema operacional com suporte a POSIX
(Linux, por exemplo), de tal modo que possam ser usados os compiladores **GCC** ou **G++**, para:

1. Criar um processo pai que possa fazer a abertura de uma imagem em nível de cinza
(que será disponibilizada e usada para detecção de bordas) e que crie as estruturas de
dados necessárias para representar essa imagem (como uma matriz I, por exemplo);

2. O processo pai deve executar a função fork() para criar um processo filho. Este novo
processo deve usar a matriz I para gerar as informações das bordas, na direção x, da
imagem de entrada. Estas informações da borda devem ficar em uma matriz Rx e,posteriormente, disponibilizadas para o processo pai (a utilização de arquivo é uma
possibilidade de solução para este problema). Após executar fork(), o processo pai deve
aguardar a execução do processo filho através da função wait(). Ele retorna à execução
quando o filho for finalizado.

3. O processo pai deve executar uma nova chamada da função fork(), gerando um novo
processo filho. Este novo processo deve usar a matriz I para gerar as informações da
borda da imagem na direção y, salvando os resultados em uma matriz Ry e
disponibilizando-a para o email processo pai (de forma similar ao aplicado com a matriz
Rx). Do mesmo modo, o pai deve executar a chamada wait(), fazendo com que ele fique
em estado de espera até que este novo filho finalize sua tarefa.

4. Por fim, após a execução dos filhos, o processo pai utiliza as informações das matrizes
Rx e Ry para gerar a matriz com as informações das bordas (matriz R). Esse resultado
deve ser salvo como uma imagem.
