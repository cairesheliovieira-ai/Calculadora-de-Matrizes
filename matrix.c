/* Calculadora de Matrizes */
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_MATRIZES 10
#define MAX_DIM 10
#define EPSILON 1e-9
typedef struct { int linhas, colunas; double dados[MAX_DIM][MAX_DIM]; } Matriz;
typedef enum { OK, ERRO_DIMENSAO, ERRO_NAO_QUADRADA, ERRO_SINGULAR } Status;
static const char *ERROS[] = {"", "dimensoes incompativeis", "divisora nao e quadrada", "divisora singular (sem inversa)"};

/* Entrada e validacao */
double lerReal(const char *msg) {
    char buf[128], *fim; double v;
    for (;;) {
        printf("%s", msg);
        if (!fgets(buf, sizeof buf, stdin)) { puts("\nFim da entrada."); exit(0); }
        v = strtod(buf, &fim);
        while (isspace((unsigned char)*fim)) fim++;
        if (fim != buf && !*fim && isfinite(v)) return v;
        puts("  Entrada invalida. Digite um numero.");
    }
}
int lerInteiro(const char *msg, int min, int max) {
    char txt[128]; double v;
    snprintf(txt, sizeof txt, "%s [%d a %d]: ", msg, min, max);
    for (;;) {
        v = lerReal(txt);
        if (v == floor(v) && v >= min && v <= max) return (int)v;
        puts("  Valor invalido ou fora do intervalo.");
    }
}
int cadastrarMatrizes(Matriz ms[]) {
    char txt[64];
    int q = lerInteiro("\nQuantidade de matrizes", 2, MAX_MATRIZES);
    for (int k = 0; k < q; k++) {
        printf("\n--- Matriz %d ---\n", k + 1);
        ms[k].linhas = lerInteiro("Linhas", 1, MAX_DIM); ms[k].colunas = lerInteiro("Colunas", 1, MAX_DIM);
        for (int i = 0; i < ms[k].linhas; i++)
            for (int j = 0; j < ms[k].colunas; j++) {
                snprintf(txt, sizeof txt, "Elemento [%d][%d]: ", i + 1, j + 1);
                ms[k].dados[i][j] = lerReal(txt);
            }
    }
    return q;
}
void exibirMatriz(const Matriz *m) {
    for (int i = 0; i < m->linhas; i++) {
        printf("  |");
        for (int j = 0; j < m->colunas; j++) printf(" %9.3f", fabs(m->dados[i][j]) < 5e-4 ? 0.0 : m->dados[i][j]);
        puts(" |");
    }
}

/* Operacoes (retornam Status) */
Status somarOuSubtrair(const Matriz *a, const Matriz *b, Matriz *r, int sinal) {
    if (a->linhas != b->linhas || a->colunas != b->colunas) return ERRO_DIMENSAO;
    r->linhas = a->linhas; r->colunas = a->colunas;
    for (int i = 0; i < r->linhas; i++)
        for (int j = 0; j < r->colunas; j++) r->dados[i][j] = a->dados[i][j] + sinal * b->dados[i][j];
    return OK;
}
Status multiplicar(const Matriz *a, const Matriz *b, Matriz *r) {
    if (a->colunas != b->linhas) return ERRO_DIMENSAO;
    r->linhas = a->linhas; r->colunas = b->colunas;
    for (int i = 0; i < r->linhas; i++)
        for (int j = 0; j < r->colunas; j++) {
            double soma = 0;
            for (int k = 0; k < a->colunas; k++) soma += a->dados[i][k] * b->dados[k][j];
            r->dados[i][j] = soma;
        }
    return OK;
}
Status inverter(const Matriz *m, Matriz *inv) {  /* Gauss-Jordan sobre [A | I] */
    double g[MAX_DIM][2 * MAX_DIM], t;
    int n = m->linhas, p;
    if (m->linhas != m->colunas) return ERRO_NAO_QUADRADA;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < 2 * n; j++) g[i][j] = j < n ? m->dados[i][j] : (i == j - n);
    for (int k = 0; k < n; k++) {
        p = k;
        for (int i = k + 1; i < n; i++) if (fabs(g[i][k]) > fabs(g[p][k])) p = i;
        if (fabs(g[p][k]) < EPSILON) return ERRO_SINGULAR;
        for (int j = 0; j < 2 * n; j++) { t = g[k][j]; g[k][j] = g[p][j]; g[p][j] = t; }
        t = g[k][k];
        for (int j = 0; j < 2 * n; j++) g[k][j] /= t;
        for (int i = 0; i < n; i++) {
            t = g[i][k];
            for (int j = 0; i != k && j < 2 * n; j++) g[i][j] -= t * g[k][j];
        }
    }
    inv->linhas = inv->colunas = n;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) inv->dados[i][j] = g[i][j + n];
    return OK;
}
Status dividir(const Matriz *a, const Matriz *b, Matriz *r) {  /* A / B = A x B^-1 */
    Matriz inv; Status s;
    if (b->linhas != b->colunas) return ERRO_NAO_QUADRADA;
    if (a->colunas != b->linhas) return ERRO_DIMENSAO;
    s = inverter(b, &inv);
    return s != OK ? s : multiplicar(a, &inv, r);
}

/* Controle */
void executarOperacao(int op, const Matriz ms[], int q) {  /* ((M1 op M2) op M3) ... */
    Matriz acc = ms[0], prox; Status s;
    for (int i = 1; i < q; i++) {
        if (op <= 2) s = somarOuSubtrair(&acc, &ms[i], &prox, op == 1 ? 1 : -1);
        else s = op == 3 ? multiplicar(&acc, &ms[i], &prox) : dividir(&acc, &ms[i], &prox);
        if (s != OK) { printf("\n[ERRO] Etapa %d (com a matriz %d): %s.\n", i, i + 1, ERROS[s]); return; }
        acc = prox;
    }
    printf("\nResultado (%dx%d):\n", acc.linhas, acc.colunas);
    exibirMatriz(&acc);
}
int main(void) {
    Matriz ms[MAX_MATRIZES];
    int q = cadastrarMatrizes(ms), op;
    do {
        puts("\n===== MENU =====\n 1-Adicao  2-Subtracao  3-Multiplicacao  4-Divisao (A x B^-1)\n5-Novas matrizes  0-Encerrar");
        op = lerInteiro("Opcao", 0, 5);
        if (op >= 1 && op <= 4) executarOperacao(op, ms, q);
        else if (op == 5) q = cadastrarMatrizes(ms);
    } while (op != 0);
    puts("Programa encerrado.");
    return 0;
}
