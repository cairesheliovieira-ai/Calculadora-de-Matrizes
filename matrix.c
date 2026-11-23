#include "matrix.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define EPSILON 1e-10

static void imprimir_erro(ResultadoOperacao resultado) {
    switch (resultado) {
        case OP_ERRO_DIMENSOES:
            printf("Operacao impossivel: dimensoes incompativeis.\n");
            break;
        case OP_ERRO_SINGULAR:
            printf("Operacao impossivel: matriz divisora singular (sem inversa).\n");
            break;
        case OP_ERRO_NAO_QUADRADA:
            printf("Operacao impossivel: a matriz divisora precisa ser quadrada.\n");
            break;
        case OP_ERRO_MEMORIA:
            printf("Erro: memoria insuficiente.\n");
            break;
        default:
            break;
    }
}

void limpar_entrada(void) {
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }
}

size_t ler_tamanho(const char *mensagem, size_t minimo, size_t maximo) {
    size_t valor;

    for (;;) {
        printf("%s", mensagem);
        if (scanf("%zu", &valor) == 1 && valor >= minimo && valor <= maximo) {
            limpar_entrada();
            return valor;
        }
        limpar_entrada();
        printf("Entrada invalida. Informe um numero inteiro entre %zu e %zu.\n",
               minimo, maximo);
    }
}

double ler_double(const char *mensagem) {
    double valor;

    for (;;) {
        printf("%s", mensagem);
        if (scanf("%lf", &valor) == 1 && isfinite(valor)) {
            limpar_entrada();
            return valor;
        }
        limpar_entrada();
        printf("Entrada invalida. Informe um numero real valido.\n");
    }
}

Matriz matriz_criar(size_t linhas, size_t colunas) {
    Matriz matriz = {0, 0, NULL};

    if (linhas == 0 || colunas == 0 || linhas > SIZE_MAX / sizeof(double *) ||
        colunas > SIZE_MAX / sizeof(double *)) {
        return matriz;
    }

    matriz.dados = (double **)calloc(linhas, sizeof(double *));
    if (matriz.dados == NULL) {
        return matriz;
    }

    matriz.linhas = linhas;
    matriz.colunas = colunas;

    for (size_t i = 0; i < linhas; ++i) {
        matriz.dados[i] = (double *)calloc(colunas, sizeof(double));
        if (matriz.dados[i] == NULL) {
            matriz_liberar(&matriz);
            return (Matriz){0, 0, NULL};
        }
    }

    return matriz;
}

void matriz_liberar(Matriz *matriz) {
    if (matriz == NULL) {
        return;
    }

    if (matriz->dados != NULL) {
        for (size_t i = 0; i < matriz->linhas; ++i) {
            free(matriz->dados[i]);
        }
        free(matriz->dados);
    }

    matriz->dados = NULL;
    matriz->linhas = 0;
    matriz->colunas = 0;
}

Matriz matriz_clonar(const Matriz *origem) {
    Matriz copia = matriz_criar(origem->linhas, origem->colunas);

    if (copia.dados == NULL) {
        return copia;
    }

    for (size_t i = 0; i < origem->linhas; ++i) {
        for (size_t j = 0; j < origem->colunas; ++j) {
            copia.dados[i][j] = origem->dados[i][j];
        }
    }

    return copia;
}

Matriz matriz_ler(size_t numero) {
    char mensagem[128];

    printf("\nMatriz %zu:\n", numero);
    size_t linhas = ler_tamanho("Quantidade de linhas: ", 1, 1000);
    size_t colunas = ler_tamanho("Quantidade de colunas: ", 1, 1000);

    Matriz matriz = matriz_criar(linhas, colunas);
    if (matriz.dados == NULL) {
        printf("Erro: nao foi possivel alocar memoria para a matriz.\n");
        return matriz;
    }

    for (size_t i = 0; i < linhas; ++i) {
        for (size_t j = 0; j < colunas; ++j) {
            snprintf(mensagem, sizeof(mensagem), "Elemento [%zu][%zu]: ", i + 1, j + 1);
            matriz.dados[i][j] = ler_double(mensagem);
        }
    }

    return matriz;
}

void matriz_exibir(const Matriz *matriz) {
    if (matriz == NULL || matriz->dados == NULL) {
        printf("(matriz vazia)\n");
        return;
    }

    for (size_t i = 0; i < matriz->linhas; ++i) {
        printf("  ");
        for (size_t j = 0; j < matriz->colunas; ++j) {
            double valor = matriz->dados[i][j];
            if (fabs(valor) < EPSILON) {
                valor = 0.0;
            }
            printf("%10.3lf", valor);
        }
        printf("\n");
    }
}

Matriz matriz_soma(const Matriz *a, const Matriz *b, ResultadoOperacao *resultado) {
    if (a->linhas != b->linhas || a->colunas != b->colunas) {
        *resultado = OP_ERRO_DIMENSOES;
        return (Matriz){0, 0, NULL};
    }

    Matriz resposta = matriz_criar(a->linhas, a->colunas);
    if (resposta.dados == NULL) {
        *resultado = OP_ERRO_MEMORIA;
        return resposta;
    }

    for (size_t i = 0; i < a->linhas; ++i) {
        for (size_t j = 0; j < a->colunas; ++j) {
            resposta.dados[i][j] = a->dados[i][j] + b->dados[i][j];
        }
    }

    *resultado = OP_ERRO_OK;
    return resposta;
}

Matriz matriz_subtracao(const Matriz *a, const Matriz *b, ResultadoOperacao *resultado) {
    if (a->linhas != b->linhas || a->colunas != b->colunas) {
        *resultado = OP_ERRO_DIMENSOES;
        return (Matriz){0, 0, NULL};
    }

    Matriz resposta = matriz_criar(a->linhas, a->colunas);
    if (resposta.dados == NULL) {
        *resultado = OP_ERRO_MEMORIA;
        return resposta;
    }

    for (size_t i = 0; i < a->linhas; ++i) {
        for (size_t j = 0; j < a->colunas; ++j) {
            resposta.dados[i][j] = a->dados[i][j] - b->dados[i][j];
        }
    }

    *resultado = OP_ERRO_OK;
    return resposta;
}

Matriz matriz_multiplicacao(const Matriz *a, const Matriz *b, ResultadoOperacao *resultado) {
    if (a->colunas != b->linhas) {
        *resultado = OP_ERRO_DIMENSOES;
        return (Matriz){0, 0, NULL};
    }

    Matriz resposta = matriz_criar(a->linhas, b->colunas);
    if (resposta.dados == NULL) {
        *resultado = OP_ERRO_MEMORIA;
        return resposta;
    }

    for (size_t i = 0; i < a->linhas; ++i) {
        for (size_t j = 0; j < b->colunas; ++j) {
            double soma = 0.0;
            for (size_t k = 0; k < a->colunas; ++k) {
                soma += a->dados[i][k] * b->dados[k][j];
            }
            resposta.dados[i][j] = soma;
        }
    }

    *resultado = OP_ERRO_OK;
    return resposta;
}

Matriz matriz_inverter(const Matriz *matriz, ResultadoOperacao *resultado) {
    if (matriz->linhas != matriz->colunas) {
        *resultado = OP_ERRO_NAO_QUADRADA;
        return (Matriz){0, 0, NULL};
    }

    size_t n = matriz->linhas;
    Matriz aumentada = matriz_criar(n, 2 * n);
    if (aumentada.dados == NULL) {
        *resultado = OP_ERRO_MEMORIA;
        return aumentada;
    }

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            aumentada.dados[i][j] = matriz->dados[i][j];
            aumentada.dados[i][j + n] = (i == j) ? 1.0 : 0.0;
        }
    }

    for (size_t p = 0; p < n; ++p) {
        size_t linha_pivo = p;
        double maior = fabs(aumentada.dados[p][p]);

        for (size_t i = p + 1; i < n; ++i) {
            double valor = fabs(aumentada.dados[i][p]);
            if (valor > maior) {
                maior = valor;
                linha_pivo = i;
            }
        }

        if (maior <= EPSILON) {
            matriz_liberar(&aumentada);
            *resultado = OP_ERRO_SINGULAR;
            return (Matriz){0, 0, NULL};
        }

        if (linha_pivo != p) {
            double *temporario = aumentada.dados[p];
            aumentada.dados[p] = aumentada.dados[linha_pivo];
            aumentada.dados[linha_pivo] = temporario;
        }

        double pivo = aumentada.dados[p][p];
        for (size_t j = 0; j < 2 * n; ++j) {
            aumentada.dados[p][j] /= pivo;
        }

        for (size_t i = 0; i < n; ++i) {
            if (i == p) {
                continue;
            }
            double fator = aumentada.dados[i][p];
            if (fabs(fator) <= EPSILON) {
                continue;
            }
            for (size_t j = 0; j < 2 * n; ++j) {
                aumentada.dados[i][j] -= fator * aumentada.dados[p][j];
            }
        }
    }

    Matriz inversa = matriz_criar(n, n);
    if (inversa.dados == NULL) {
        matriz_liberar(&aumentada);
        *resultado = OP_ERRO_MEMORIA;
        return inversa;
    }

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            inversa.dados[i][j] = aumentada.dados[i][j + n];
        }
    }

    matriz_liberar(&aumentada);
    *resultado = OP_ERRO_OK;
    return inversa;
}

ResultadoOperacao operacao_adicao_sequencial(const Matriz *matrizes, size_t quantidade,
                                            Matriz *resultado) {
    *resultado = matriz_clonar(&matrizes[0]);
    if (resultado->dados == NULL) {
        return OP_ERRO_MEMORIA;
    }

    for (size_t i = 1; i < quantidade; ++i) {
        ResultadoOperacao estado;
        Matriz seguinte = matriz_soma(resultado, &matrizes[i], &estado);
        matriz_liberar(resultado);
        if (estado != OP_ERRO_OK) {
            return estado;
        }
        *resultado = seguinte;
    }

    return OP_ERRO_OK;
}

ResultadoOperacao operacao_subtracao_sequencial(const Matriz *matrizes, size_t quantidade,
                                               Matriz *resultado) {
    *resultado = matriz_clonar(&matrizes[0]);
    if (resultado->dados == NULL) {
        return OP_ERRO_MEMORIA;
    }

    for (size_t i = 1; i < quantidade; ++i) {
        ResultadoOperacao estado;
        Matriz seguinte = matriz_subtracao(resultado, &matrizes[i], &estado);
        matriz_liberar(resultado);
        if (estado != OP_ERRO_OK) {
            return estado;
        }
        *resultado = seguinte;
    }

    return OP_ERRO_OK;
}

ResultadoOperacao operacao_multiplicacao_sequencial(const Matriz *matrizes, size_t quantidade,
                                                   Matriz *resultado) {
    *resultado = matriz_clonar(&matrizes[0]);
    if (resultado->dados == NULL) {
        return OP_ERRO_MEMORIA;
    }

    for (size_t i = 1; i < quantidade; ++i) {
        ResultadoOperacao estado;
        Matriz seguinte = matriz_multiplicacao(resultado, &matrizes[i], &estado);
        matriz_liberar(resultado);
        if (estado != OP_ERRO_OK) {
            return estado;
        }
        *resultado = seguinte;
    }

    return OP_ERRO_OK;
}

ResultadoOperacao operacao_divisao_sequencial(const Matriz *matrizes, size_t quantidade,
                                              Matriz *resultado) {
    *resultado = matriz_clonar(&matrizes[0]);
    if (resultado->dados == NULL) {
        return OP_ERRO_MEMORIA;
    }

    for (size_t i = 1; i < quantidade; ++i) {
        if (matrizes[i].linhas != matrizes[i].colunas) {
            matriz_liberar(resultado);
            return OP_ERRO_NAO_QUADRADA;
        }

        ResultadoOperacao estado;
        Matriz inversa = matriz_inverter(&matrizes[i], &estado);
        if (estado != OP_ERRO_OK) {
            matriz_liberar(resultado);
            return estado;
        }

        Matriz seguinte = matriz_multiplicacao(resultado, &inversa, &estado);
        matriz_liberar(&inversa);
        matriz_liberar(resultado);

        if (estado != OP_ERRO_OK) {
            return estado;
        }
        *resultado = seguinte;
    }

    return OP_ERRO_OK;
}

