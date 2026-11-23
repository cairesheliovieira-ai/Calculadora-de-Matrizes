#include "matrix.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define EPSILON 1e-9

static const char *ERROS[] = {
    "",
    "dimensoes incompativeis",
    "divisora nao e quadrada",
    "divisora singular (sem inversa)"
};

/* Le uma linha completa e aceita somente um numero real finito. */
static double lerReal(const char *mensagem) {
    char buffer[128];

    for (;;) {
        char *fim;
        double valor;

        printf("%s", mensagem);
        if (fgets(buffer, sizeof buffer, stdin) == NULL) {
            puts("\nFim da entrada.");
            exit(EXIT_FAILURE);
        }

        valor = strtod(buffer, &fim);
        while (isspace((unsigned char)*fim)) {
            ++fim;
        }

        if (fim != buffer && *fim == '\0' && isfinite(valor)) {
            return valor;
        }
        puts("  Entrada invalida. Digite um numero real finito.");
    }
}

static int lerInteiro(const char *mensagem, int minimo, int maximo) {
    char texto[128];
    double valor;

    snprintf(texto, sizeof texto, "%s [%d a %d]: ", mensagem, minimo, maximo);
    for (;;) {
        valor = lerReal(texto);
        if (valor == floor(valor) && valor >= minimo && valor <= maximo) {
            return (int)valor;
        }
        puts("  Valor invalido ou fora do intervalo.");
    }
}

int cadastrarMatrizes(Matriz matrizes[]) {
    char mensagem[64];
    int quantidade = lerInteiro("\nQuantidade de matrizes", 2, MAX_MATRIZES);

    for (int k = 0; k < quantidade; ++k) {
        printf("\n--- Matriz %d ---\n", k + 1);
        matrizes[k].linhas = lerInteiro("Linhas", 1, MAX_DIM);
        matrizes[k].colunas = lerInteiro("Colunas", 1, MAX_DIM);

        for (int i = 0; i < matrizes[k].linhas; ++i) {
            for (int j = 0; j < matrizes[k].colunas; ++j) {
                snprintf(mensagem, sizeof mensagem, "Elemento [%d][%d]: ", i + 1, j + 1);
                matrizes[k].dados[i][j] = lerReal(mensagem);
            }
        }
    }
    return quantidade;
}

void exibirMatriz(const Matriz *matriz) {
    for (int i = 0; i < matriz->linhas; ++i) {
        printf("  |");
        for (int j = 0; j < matriz->colunas; ++j) {
            double valor = matriz->dados[i][j];
            printf(" %9.3f", fabs(valor) < 5e-4 ? 0.0 : valor);
        }
        puts(" |");
    }
}

Status somarOuSubtrair(const Matriz *a, const Matriz *b, Matriz *resultado, int sinal) {
    if (a->linhas != b->linhas || a->colunas != b->colunas) {
        return ERRO_DIMENSAO;
    }

    resultado->linhas = a->linhas;
    resultado->colunas = a->colunas;
    for (int i = 0; i < resultado->linhas; ++i) {
        for (int j = 0; j < resultado->colunas; ++j) {
            resultado->dados[i][j] = a->dados[i][j] + sinal * b->dados[i][j];
        }
    }
    return OK;
}

Status multiplicar(const Matriz *a, const Matriz *b, Matriz *resultado) {
    if (a->colunas != b->linhas) {
        return ERRO_DIMENSAO;
    }

    resultado->linhas = a->linhas;
    resultado->colunas = b->colunas;
    for (int i = 0; i < resultado->linhas; ++i) {
        for (int j = 0; j < resultado->colunas; ++j) {
            double soma = 0.0;
            for (int k = 0; k < a->colunas; ++k) {
                soma += a->dados[i][k] * b->dados[k][j];
            }
            resultado->dados[i][j] = soma;
        }
    }
    return OK;
}

Status inverter(const Matriz *matriz, Matriz *inversa) {
    double aumentada[MAX_DIM][2 * MAX_DIM];
    int n = matriz->linhas;

    if (matriz->linhas != matriz->colunas) {
        return ERRO_NAO_QUADRADA;
    }

    /* Monta [A | I] para aplicar Gauss-Jordan com pivoteamento parcial. */
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < 2 * n; ++j) {
            aumentada[i][j] = j < n ? matriz->dados[i][j] : (i == j - n ? 1.0 : 0.0);
        }
    }

    for (int coluna = 0; coluna < n; ++coluna) {
        int pivo = coluna;
        for (int i = coluna + 1; i < n; ++i) {
            if (fabs(aumentada[i][coluna]) > fabs(aumentada[pivo][coluna])) {
                pivo = i;
            }
        }
        if (fabs(aumentada[pivo][coluna]) < EPSILON) {
            return ERRO_SINGULAR;
        }

        for (int j = 0; j < 2 * n; ++j) {
            double temporario = aumentada[coluna][j];
            aumentada[coluna][j] = aumentada[pivo][j];
            aumentada[pivo][j] = temporario;
        }

        double divisor = aumentada[coluna][coluna];
        for (int j = 0; j < 2 * n; ++j) {
            aumentada[coluna][j] /= divisor;
        }

        for (int i = 0; i < n; ++i) {
            if (i == coluna) {
                continue;
            }
            double fator = aumentada[i][coluna];
            for (int j = 0; j < 2 * n; ++j) {
                aumentada[i][j] -= fator * aumentada[coluna][j];
            }
        }
    }

    inversa->linhas = inversa->colunas = n;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inversa->dados[i][j] = aumentada[i][j + n];
        }
    }
    return OK;
}

Status dividir(const Matriz *a, const Matriz *b, Matriz *resultado) {
    Matriz inversa;
    Status estado;

    if (b->linhas != b->colunas) {
        return ERRO_NAO_QUADRADA;
    }
    if (a->colunas != b->linhas) {
        return ERRO_DIMENSAO;
    }

    estado = inverter(b, &inversa);
    return estado != OK ? estado : multiplicar(a, &inversa, resultado);
}

void executarOperacao(int operacao, const Matriz matrizes[], int quantidade) {
    Matriz acumulada = matrizes[0];

    /* Aplica cada operação da esquerda para a direita: ((M1 op M2) op M3)... */
    for (int i = 1; i < quantidade; ++i) {
        Matriz proxima;
        Status estado;

        if (operacao == 1 || operacao == 2) {
            estado = somarOuSubtrair(&acumulada, &matrizes[i], &proxima,
                                     operacao == 1 ? 1 : -1);
        } else if (operacao == 3) {
            estado = multiplicar(&acumulada, &matrizes[i], &proxima);
        } else {
            estado = dividir(&acumulada, &matrizes[i], &proxima);
        }

        if (estado != OK) {
            printf("\n[ERRO] Etapa %d (com a matriz %d): %s.\n",
                   i, i + 1, ERROS[estado]);
            return;
        }
        acumulada = proxima;
    }

    printf("\nResultado (%dx%d):\n", acumulada.linhas, acumulada.colunas);
    exibirMatriz(&acumulada);
}
