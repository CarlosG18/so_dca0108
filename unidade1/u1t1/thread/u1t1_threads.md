# Solução com threads

> Algoritmo para detecção de borda em imagens usando o conceito de threads

Bom já que entendemos como devemos calcular a borda de uma imagem, vamos ultilizar os conceitos de threads para nos ajudar nessa tarefa.

## 🔄 Fluxo do programa

vamos dividir nosso programa da seguinte maneira:

<p align='center'>
    <img width=350 src=""/>
</p>

### [1/4] 🔵 Criando um thread Mãe

**👨‍💻 REPRESENTAÇÃO NO CÓDIGO EM C**:
```c

```

**🔑 EXPLICANDO PARTES CHAVES**

### [2/4] 🔵 Criando a thread filha para processamento da borda no eixo x

**👨‍💻 REPRESENTAÇÃO NO CÓDIGO EM C**:
```c

```

**🔑 EXPLICANDO PARTES CHAVES**

### [3/4] 🔵 Criando a thread filha para processamento da borda no eixo y

**👨‍💻 REPRESENTAÇÃO NO CÓDIGO EM C**:
```c

```

**🔑 EXPLICANDO PARTES CHAVES**

### [4/4] 🔵 junção dos dados processados pelas threads filhas

**👨‍💻 REPRESENTAÇÃO NO CÓDIGO EM C**:
```c

```

**🔑 EXPLICANDO PARTES CHAVES**

## 🔍 Comparação com a solução com processos

