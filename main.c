#include "matrix.h"

#include <stdio.h>

static void exibirMenu(void) {
    puts("\n===== MENU =====");
    puts(" 1 - Adicao");
    puts(" 2 - Subtracao");
    puts(" 3 - Multiplicacao");
    puts(" 4 - Divisao (A x B^-1)");
    puts(" 5 - Cadastrar novas matrizes");
    puts(" 0 - Encerrar");
}

int main(void) {
    Matriz matrizes[MAX_MATRIZES];
    int quantidade = cadastrarMatrizes(matrizes);

    for (;;) {
        int opcao;
        exibirMenu();
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) {
            int caractere;
            while ((caractere = getchar()) != '\n' && caractere != EOF) {
            }
            puts("Opcao invalida. Digite um numero de 0 a 5.");
            continue;
        }
        int caractere;
        while ((caractere = getchar()) != '\n' && caractere != EOF) {
        }

        if (opcao == 0) {
            puts("Programa encerrado.");
            return 0;
        }
        if (opcao >= 1 && opcao <= 4) {
            executarOperacao(opcao, matrizes, quantidade);
        } else if (opcao == 5) {
            quantidade = cadastrarMatrizes(matrizes);
        } else {
            puts("Opcao invalida. Digite um numero de 0 a 5.");
        }
    }
}
