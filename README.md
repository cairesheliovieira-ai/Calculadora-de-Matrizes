# Calculadora de Matrizes

Aplicacao interativa em C para cadastrar matrizes e calcular operacoes sequenciais de adicao, subtracao, multiplicacao e divisao matricial.

## Requisitos atendidos

- Permite cadastrar de 2 a 10 matrizes.
- Para cada matriz, solicita de 1 a 10 linhas e de 1 a 10 colunas, e pede individualmente cada elemento real finito.
- Executa as operacoes na ordem em que as matrizes foram cadastradas, da esquerda para a direita.
- Adicao e subtracao exigem dimensoes iguais em cada etapa.
- Multiplicacao verifica a compatibilidade entre as colunas do resultado parcial e as linhas da proxima matriz.
- Divisao e definida como `A / B = A x B^-1`; cada divisor deve ser quadrado, inversivel e compativel com o resultado parcial. Com tres ou mais matrizes, a divisao e feita sequencialmente da esquerda para a direita.
- A inversa e calculada pelo metodo de Gauss-Jordan com pivoteamento parcial. Matrizes singulares sao detectadas e informadas.
- A matriz resultante e apresentada em linhas e colunas organizadas.
- O menu permite executar varias operacoes, cadastrar um novo conjunto de matrizes ou encerrar o programa.

## Arquivos

- `main.c`: fluxo principal e menu interativo.
- `matrix.c`: leitura e validacao de entrada, exibicao, operacoes e inversao de matrizes.
- `matrix.h`: tipos, limites e prototipos compartilhados.
- `Makefile`: compilacao e limpeza dos arquivos gerados.

## Compilacao e execucao

Requisitos: GCC ou compilador C11 compativel e a biblioteca matematica padrao.

Com Make:

```bash
make
./calculadora
```

Ou compilando diretamente:

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c matrix.c -lm -o calculadora
./calculadora
```

No Windows, execute `calculadora.exe` depois da compilacao. Para limpar os arquivos gerados pelo Make:

```bash
make clean
```

## Uso

1. Informe a quantidade de matrizes (entre 2 e 10).
2. Para cada matriz, informe linhas, colunas e cada elemento.
3. Escolha no menu: adicao, subtracao, multiplicacao ou divisao.
4. Use a opcao de novas matrizes para cadastrar outro conjunto, ou escolha encerrar.

Os limites de dimensao evitam exceder o armazenamento estatico definido pelo programa. Numeros nao finitos e entradas fora dos limites sao rejeitados.
