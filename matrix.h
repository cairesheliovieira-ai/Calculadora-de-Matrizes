#ifndef MATRIX_H
#define MATRIX_H

#define MAX_MATRIZES 10
#define MAX_DIM 10

typedef struct {
    int linhas;
    int colunas;
    double dados[MAX_DIM][MAX_DIM];
} Matriz;

typedef enum {
    OK,
    ERRO_DIMENSAO,
    ERRO_NAO_QUADRADA,
    ERRO_SINGULAR
} Status;

/* Cadastro, leitura e exibicao. */
int cadastrarMatrizes(Matriz matrizes[]);
void exibirMatriz(const Matriz *matriz);

/* Operacoes elementares e execucao sequencial. */
Status somarOuSubtrair(const Matriz *a, const Matriz *b, Matriz *resultado, int sinal);
Status multiplicar(const Matriz *a, const Matriz *b, Matriz *resultado);
Status inverter(const Matriz *matriz, Matriz *inversa);
Status dividir(const Matriz *a, const Matriz *b, Matriz *resultado);
void executarOperacao(int operacao, const Matriz matrizes[], int quantidade);

#endif
