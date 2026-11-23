#include "matrix.h"

#include <stdio>
#include <stdlib.h>

#define QUANTIDADE_MAXIMA 100

static void imprimir_menu(void) {
    printf("\n===== OPERACOES =====\n");
    printf("1 - Adicao\n");
    printf("2 - Subtracao\n");
    printf("3 - Multiplicacao\n");
    printf("4 - Divisao matricial (A x B^-1)\n");
    printf("0 - Encerrar\n");
}

static void imprimir_resultado(ResultadoOperacao estado, const Matriz *resultado) {
    switch (estado) {
        case OP_ERRO_OK:
            printf("\nMatriz resultante:\n");
            matriz_exibir(resultado);
            break;
        case OP_ERRO_DIMENSOES:
            printf("\nOperacao impossivel: dimensoes incompativeis em uma etapa da sequencia.\n");
            break;
        case OP_ERRO_SINGULAR:
            printf("\nOperacao impossivel: uma matriz divisora e singular (sem inversa).\n");
            break;
        case OP_ERRO_NAO_QUADRADA:
            printf("\nOperacao impossivel: toda matriz divisora deve ser quadrada.\n");
            break;
        case OP_ERRO_MEMORIA:
            printf("\nErro: memoria insuficiente.\n");
            break;
    }
}

int main(void) {
    printf("=====================================\n");
    printf("SISTEMA DE CALCULADORAS DE MATRIZES\n");
    printf("=====================================\n");

    size_t quantidade = ler_tamanho("Quantidade de matrizes (minimo 2): ", 2, QUANTIDADE_MAXIMA);
    Matriz *matrizes = (Matriz *)calloc(quantidade, sizeof(Matriz));

    if (matrizes == NULL) {
        printf("Erro: nao foi possivel alocar memoria para a lista de matrizes.\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < quantidade; ++i) {
        matrizes[i] = matriz_ler(i + 1);
        if (matrizes[i].dados == NULL) {
            for (size_t j = 0; j < i; ++j) {
                matriz_liberar(&matrizes[j]);
            }
            free(matrizes);
            return EXIT_FAILURE;
        }
    }

    for (;;) {
        imprimir_menu();
        int opcao = (int)ler_tamanho("Escolha uma operacao: ", 0, 4);

        if (opcao == 0) {
            printf("Encerrando o programa.\n");
            break;
        }

        Matriz resultado = {0, 0, NULL};
        ResultadoOperacao estado;

        switch (opcao) {
            case 1:
                estado = operacao_adicao_sequencial(matrizes, quantidade, &resultado);
                break;
            case 2:
                estado = operacao_subtracao_sequencial(matrizes, quantidade, &resultado);
                break;
            case 3:
                estado = operacao_multiplicacao_sequencial(matrizes, quantidade, &resultado);
                break;
            case 4:
                estado = operacao_divisao_sequencial(matrizes, quantidade, &resultado);
                break;
            default:
                estado = OP_ERRO_DIMENSOES;
                break;
        }

        imprimir_resultado(estado, &resultado);
        matriz_liberar(&resultado);
    }

    for (size_t i = 0; i < quantidade; ++i) {
        matriz_liberar(&matrizes[i]);
    }
    free(matrizes);

    return EXIT_SUCCESS;
}
