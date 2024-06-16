#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rubro.h"

typedef struct NoRB NoRB;
typedef struct ArvoreRB ArvoreRB;

NoRB* criarNo(int dado, CorNo cor, NoRB* NIL) {
    NoRB* no = (NoRB*)malloc(sizeof(NoRB));
    no->dado = dado;
    no->cor = cor;
    no->esquerda = NIL;
    no->direita = NIL;
    no->pai = NIL;
    return no;
}

ArvoreRB* criarArvore() {
    ArvoreRB* arvore = (ArvoreRB*)malloc(sizeof(ArvoreRB));
    arvore->NIL = criarNo(0, PRETO, NULL); // Nó sentinela
    arvore->raiz = arvore->NIL;
    arvore->rotacoes = 0;
    return arvore;
}

void rotacaoEsquerda(ArvoreRB* arvore, NoRB* x) {
    NoRB* y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != arvore->NIL) {
        y->esquerda->pai = x;
    }
    y->pai = x->pai;
    if (x->pai == arvore->NIL) {
        arvore->raiz = y;
    } else if (x == x->pai->esquerda) {
        x->pai->esquerda = y;
    } else {
        x->pai->direita = y;
    }
    y->esquerda = x;
    x->pai = y;
    arvore->rotacoes++;
}

void rotacaoDireita(ArvoreRB* arvore, NoRB* y) {
    NoRB* x = y->esquerda;
    y->esquerda = x->direita;
    if (x->direita != arvore->NIL) {
        x->direita->pai = y;
    }
    x->pai = y->pai;
    if (y->pai == arvore->NIL) {
        arvore->raiz = x;
    } else if (y == y->pai->direita) {
        y->pai->direita = x;
    } else {
        y->pai->esquerda = x;
    }
    x->direita = y;
    y->pai = x;
    arvore->rotacoes++;
}

void corrigirInsercao(ArvoreRB* arvore, NoRB* z) {
    while (z->pai->cor == VERMELHO) {
        if (z->pai == z->pai->pai->esquerda) {
            NoRB* y = z->pai->pai->direita;
            if (y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->direita) {
                    z = z->pai;
                    rotacaoEsquerda(arvore, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoDireita(arvore, z->pai->pai);
            }
        } else {
            NoRB* y = z->pai->pai->esquerda;
            if (y->cor == VERMELHO) {
                z->pai->cor = PRETO;
                y->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                z = z->pai->pai;
            } else {
                if (z == z->pai->esquerda) {
                    z = z->pai;
                    rotacaoDireita(arvore, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoEsquerda(arvore, z->pai->pai);
            }
        }
    }
    arvore->raiz->cor = PRETO;
}

void inserir(ArvoreRB* arvore, int dado) {
    NoRB* z = criarNo(dado, VERMELHO, arvore->NIL);
    NoRB* y = arvore->NIL;
    NoRB* x = arvore->raiz;

    while (x != arvore->NIL) {
        y = x;
        if (z->dado < x->dado) {
            x = x->esquerda;
        } else {
            x = x->direita;
        }
    }

    z->pai = y;
    if (y == arvore->NIL) {
        arvore->raiz = z;
    } else if (z->dado < y->dado) {
        y->esquerda = z;
    } else {
        y->direita = z;
    }

    corrigirInsercao(arvore, z);
}

void corrigirRemocao(ArvoreRB* arvore, NoRB* x) {
    while (x != arvore->raiz && x->cor == PRETO) {
        if (x == x->pai->esquerda) {
            NoRB* w = x->pai->direita;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoEsquerda(arvore, x->pai);
                w = x->pai->direita;
            }
            if (w->esquerda->cor == PRETO && w->direita->cor == PRETO) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->direita->cor == PRETO) {
                    w->esquerda->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoDireita(arvore, w);
                    w = x->pai->direita;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->direita->cor = PRETO;
                rotacaoEsquerda(arvore, x->pai);
                x = arvore->raiz;
            }
        } else {
            NoRB* w = x->pai->esquerda;
            if (w->cor == VERMELHO) {
                w->cor = PRETO;
                x->pai->cor = VERMELHO;
                rotacaoDireita(arvore, x->pai);
                w = x->pai->esquerda;
            }
            if (w->direita->cor == PRETO && w->esquerda->cor == PRETO) {
                w->cor = VERMELHO;
                x = x->pai;
            } else {
                if (w->esquerda->cor == PRETO) {
                    w->direita->cor = PRETO;
                    w->cor = VERMELHO;
                    rotacaoEsquerda(arvore, w);
                    w = x->pai->esquerda;
                }
                w->cor = x->pai->cor;
                x->pai->cor = PRETO;
                w->esquerda->cor = PRETO;
                rotacaoDireita(arvore, x->pai);
                x = arvore->raiz;
            }
        }
    }
    x->cor = PRETO;
}

void transplante(ArvoreRB* arvore, NoRB* u, NoRB* v) {
    if (u->pai == arvore->NIL) {
        arvore->raiz = v;
    } else if (u == u->pai->esquerda) {
        u->pai->esquerda = v;
    } else {
        u->pai->direita = v;
    }
    v->pai = u->pai;
}

NoRB* minimoArvore(NoRB* node, NoRB* NIL) {
    while (node->esquerda != NIL) {
        node = node->esquerda;
    }
    return node;
}

void removerNo(ArvoreRB* arvore, NoRB* z) {
    NoRB* y = z;
    NoRB* x;
    CorNo corOriginalY = y->cor;
    if (z->esquerda == arvore->NIL) {
        x = z->direita;
        transplante(arvore, z, z->direita);
    } else if (z->direita == arvore->NIL) {
        x = z->esquerda;
        transplante(arvore, z, z->esquerda);
    } else {
        y = minimoArvore(z->direita, arvore->NIL);
        corOriginalY = y->cor;
        x = y->direita;
        if (y->pai == z) {
            x->pai = y;
        } else {
            transplante(arvore, y, y->direita);
            y->direita = z->direita;
            y->direita->pai = y;
        }
        transplante(arvore, z, y);
        y->esquerda = z->esquerda;
        y->esquerda->pai = y;
        y->cor = z->cor;
    }
    if (corOriginalY == PRETO) {
        corrigirRemocao(arvore, x);
    }
}

void medirInsercao(ArvoreRB* arvore, int* valores, int n) {
    clock_t inicio = clock();
    for (int i = 0; i < n; i++) {
        inserir(arvore, valores[i]);
    }
    clock_t fim = clock();
    printf("Tempo de insercao na rubro: %f segundos\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
    printf("Quantidade de rotacoes na insercao na rubro: %d\n", arvore->rotacoes);
}

void medirRemocao(ArvoreRB* arvore, int* valores, int n) {
    clock_t inicio = clock();
    for (int i = 0; i < n; i++) {
        removerNo(arvore, arvore->raiz);
    }
    clock_t fim = clock();
    printf("Tempo de remocao na rubro: %f segundos\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
    printf("Quantidade de rotacoes na remocao na rubro: %d\n", arvore->rotacoes);
}

void medirBusca(ArvoreRB* arvore, int* valores, int n) {
    clock_t inicio = clock();
    for (int i = 0; i < n; i++) {
        NoRB* no = arvore->raiz;
        while (no != arvore->NIL) {
            if (valores[i] == no->dado) break;
            no = (valores[i] < no->dado) ? no->esquerda : no->direita;
        }
    }
    clock_t fim = clock();
    printf("Tempo de busca na rubro: %f segundos\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
}
