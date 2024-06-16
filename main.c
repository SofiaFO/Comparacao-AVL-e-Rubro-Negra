#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avl.h"
#include "rubro.h"

void testaInsercao(avl *arv) {
    clock_t inicio, fim;
    double tempo;
    int valor;

    inicio = clock();
    for (int i = 1; i <= 10000; i++) {
        valor = i;
        insereNo(arv, valor);
    }
    fim = clock();
    tempo = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo total de insercao na avl: %f segundos\n", tempo);
    printf("Numero total de rotacoes de insercao avl: %d\n", rotacoes_insercao);
}

void testaRemocao(avl *arv) {
    clock_t inicio, fim;
    double tempo;
    int valor;

    inicio = clock();
    for (int i = 1; i <= 10000; i++) {
        valor = i;
        removeNo(arv, valor);
    }
    fim = clock();
    tempo = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo total de remocao na avl: %f segundos\n", tempo);
    printf("Numero total de rotacoes de remocao na avl: %d\n", rotacoes_remocao);
}

int main() {
    ArvoreRB* arvoreRb = criarArvore();
    avl *arvore = criaArvore();
    int n = 10000;
    int* valores = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        valores[i] = i;
    }
    srand(time(NULL));

    printf("Insercoes na arvore:\n");
    testaInsercao(arvore);
    medirInsercao(arvoreRb, valores, n);
    arvoreRb->rotacoes = 0;

    printf("\nRemocoes na arvore:\n");
    testaRemocao(arvore);
    medirRemocao(arvoreRb, valores, n);
    for (int i = 0; i < n; i++) {
        inserir(arvoreRb, valores[i]);
    }
    int* valoresBusca = (int*)malloc(1000 * sizeof(int));
    for (int i = 0; i < 1000; i++) {
        valoresBusca[i] = rand() % n;
    }

    printf("\nBuscas na arvore:\n");
    busca(arvore);
    medirBusca(arvoreRb, valoresBusca, 1000);

    free(valores);
    free(valoresBusca);
    free(arvoreRb->NIL);
    free(arvoreRb);    
    free(arvore);
    return 0;
}
