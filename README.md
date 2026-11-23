# Calculadora de Matrizes

Aplicacao interativa em C para cadastrar matrizes e calcular operacoes sequenciais de adicao, subtracao, multiplicacao e divisao matricial.

## Requisitos atendidos

- Permite cadastrar de 2 a 10 matrizes.
- Para cada matriz, solicita de 1 a 10 linhas e de 1 a 10 colunas e pede cada elemento real finito.
- Executa as operacoes em ordem da esquerda para a direita.
- Adicao e subtracao exigem dimensoes iguais em cada etapa.
- Multiplicacao verifica a compatibilidade das dimensoes.
- Divisao e definida como `A / B = A x B^-1` e exige divisor quadrado e inversivel.
- A inversa usa o metodo de Gauss-Jordan com pivoteamento parcial.
- O menu permite executar varias operacoes, cadastrar outro conjunto ou encerrar o programa.

## Arquivos

- `main.c`: codigo completo da calculadora, incluindo entrada, operacoes matriciais e menu.
- `README.md`: instrucoes de compilacao e uso.

## Compilacao e execucao

Requisitos: GCC ou compilador C11 compativel e a biblioteca matematica padrao.

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c -lm -o calculadora
./calculadora
```

No Windows:

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c -lm -o calculadora.exe
calculadora.exe
```

## Uso

1. Informe a quantidade de matrizes, entre 2 e 10.
2. Para cada matriz, informe linhas, colunas e todos os elementos.
3. Escolha adicao, subtracao, multiplicacao ou divisao.
4. Use a opcao de novas matrizes para cadastrar outro conjunto ou escolha encerrar.

Os limites de dimensao evitam exceder o armazenamento estatico definido pelo programa.
