#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    size_t linhas;
    size_t colunas;
    double **dados;
} Matriz;

typedef enum {
    OP_ERRO_OK = 0,
    OP_ERRO_DIMENSOES,
    OP_ERRO_SINGULAR,
    OP_ERRO_NAO_QUADRADA,
    OP_ERRO_MEMORIA
} ResultadoOperacao;

Matriz matriz_criar(size_t linhas, size_t colunas);
Matriz matriz_clonar(const Matriz *origem);
void matriz_liberar(Matriz *matriz);

Matriz matriz_ler(size_t numero);
void matriz_exibir(const Matriz *matriz);

Matriz matriz_soma(const Matriz *a, const Matriz *b, ResultadoOperacao *resultado);
Matriz matriz_subtracao(const Matriz *a, const Matriz *b, ResultadoOperacao *resultado);
Matriz matriz_multiplicacao(const Matriz *a, const Matriz *b, ResultadoOperacao *resultado);
Matriz matriz_inverter(const Matriz *matriz, ResultadoOperacao *resultado);

ResultadoOperacao operacao_adicao_sequencial(const Matriz *matrizes, size_t quantidade,
                                            Matriz *resultado);
ResultadoOperacao operacao_subtracao_sequencial(const Matriz *matrizes, size_t quantidade,
                                               Matriz *resultado);
ResultadoOperacao operacao_multiplicacao_sequencial(const Matriz *matrizes, size_t quantidade,
                                                   Matriz *resultado);
ResultadoOperacao operacao_divisao_sequencial(const Matriz *matrizes, size_t quantidade,
                                              Matriz *resultado);

size_t ler_tamanho(const char *mensagem, size_t minimo, size_t maximo);
double ler_double(const char *mensagem);
void limpar_entrada(void);

#ifdef __cplusplus
}
#endif

#endif
