# Sistema Calculadora de Matrizes

Aplicação em linguagem C para cadastrar pelo menos duas matrizes e executar operações sequenciais de adição, subtração, multiplicação e divisão matricial (definida como `A / B = A × B⁻¹`).

## Funcionalidades

- Cadastro de duas ou mais matrizes com dimensões validadas.
- Entrada individual dos elementos reais.
- Operações executadas da esquerda para a direita:
  - `A + B + C`
  - `A - B - C`
  - `A × B × C`
  - `A × B⁻¹ × C⁻¹`
- Validação de dimensões e de compatibilidade entre matrizes.
- Cálculo da inversa por eliminação de Gauss-Jordan.
- Detecção de matriz singular.
- Permite executar várias operações sem reiniciar o programa.
- Mensagens claras para entradas inválidas e operações impossíveis.

## Estrutura dos arquivos

- `main.c`: menu, fluxo principal e coordenação das operações.
- `matrix.c`: leitura, validação, armazenamento e operações matriciais.
- `matrix.h`: estrutura `Matriz` e protótipos das funções.
- `Makefile`: compilação e limpeza dos arquivos gerados.

## Compilação

É necessário um compilador C compatível com C11, como GCC:

```bash
gcc -std=c11 -Wall -Wextra -pedantic -g matrix.c main.c -lm -o calculadora
```

Também é possível usar o Makefile:

```bash
make
```

## Execução

Linux, macOS ou terminal no Windows:

```bash
./calculadora
```

No Windows, caso tenha gerado `calculadora.exe`:

```bash
calculadora.exe
```

Para remover os arquivos compilados:

```bash
make clean
```

## Uso no GDB

Compilar com símbolos de depuração:

```bash
gcc -std=c11 -Wall -Wextra -pedantic -g matrix.c main.c -lm -o calculadora
```

Iniciar o depurador:

```bash
gdb ./calculadora
```

Comandos úteis:

```text
(gdb) break main
(gdb) run
(gdb) next
(gdb) print matriz[0]
(gdb) continue
(gdb) quit
```

Também é possível iniciar diretamente com:

```bash
gdb --args ./calculadora
```

## Exemplo de utilização

```text
Quantidade de matrizes (mínimo 2): 2

Matriz 1:
Linhas: 2
Colunas: 2
Elemento [1][1]: 1
Elemento [1][2]: 2
Elemento [2][1]: 3
Elemento [2][2]: 4

Matriz 2:
Linhas: 2
Colunas: 2
Elemento [1][1]: 4
Elemento [1][2]: 0
Elemento [2][1]: 1
Elemento [2][2]: 2

===== OPERACOES =====
1 - Adicao
2 - Subtracao
3 - Multiplicacao
4 - Divisao matricial (A x B^-1)
0 - Encerrar
Escolha uma operacao: 3

Matriz resultante:
    6.000    4.000
   16.000    8.000
```

As condições matemáticas são:

- Adição e subtração: todas as matrizes envolvidas devem ter as mesmas dimensões.
- Multiplicação: o número de colunas da matriz atual deve ser igual ao número de linhas da próxima matriz em cada etapa.
- Divisão: cada divisor da sequência deve ser quadrado e inversível. Além disso, as dimensões precisam ser compatíveis para multiplicação pela inversa.

## Funções implementadas

- `matriz_criar`, `matriz_liberar`, `matriz_clonar`: criação e gerenciamento de memória.
- `matriz_ler`, `matriz_exibir`: entrada e saída organizada.
- `matriz_soma`, `matriz_subtracao`, `matriz_multiplicacao`: operações básicas.
- `matriz_inverter`: inversa por Gauss-Jordan.
- `matriz_divisao_sequencial`: divisão pela inversa da(s) matriz(es) divisora(s).
- `operacao_adicao_sequencial`, `operacao_subtracao_sequencial`, `operacao_multiplicacao_sequencial`: processamento da esquerda para a direita.
- `ler_tamanho`, `ler_double`, `limpar_entrada`, `validar_operacao`: validações e tratamento de entrada.
