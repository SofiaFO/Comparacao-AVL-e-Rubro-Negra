#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avl.h"

typedef struct no {
    int chave;
    struct no *fesq;
    struct no *fdir;
    struct no *pai;
    int fb; 
} no;

struct avl {
    no *raiz;
    no *sentinela;
    int numElementos;
};

int rotacoes_insercao = 0;
int rotacoes_remocao = 0;

avl *criaArvore() {
    avl *arv = (avl *)malloc(sizeof(avl));
    arv->sentinela = (no *)malloc(sizeof(no));
    arv->sentinela->chave = -1000;
    arv->sentinela->fesq = arv->sentinela->fdir = arv->sentinela->pai = NULL;
    arv->sentinela->fb = 0;
    arv->raiz = arv->sentinela;
    arv->numElementos = 0;
    return arv;
}

no* criaNo(int valor) {
    no *novo = (no *)malloc(sizeof(no));
    novo->chave = valor;
    novo->fesq = novo->fdir = novo->pai = NULL;
    novo->fb = 0;
    return novo;
}

int insereNo(avl *arv, int valor) {
    no *sentinela = arv->sentinela;
    no *novo = criaNo(valor);
    if (arv->raiz == sentinela) {
        arv->raiz = novo;
        novo->pai = sentinela;
        arv->numElementos++;
        return 1;
    }

    no *atual = arv->raiz;
    while (1) {
        if (valor < atual->chave) {
            if (atual->fesq == NULL) {
                atual->fesq = novo;
                break;
            } else {
                atual = atual->fesq;
            }
        } else if (valor > atual->chave) {
            if (atual->fdir == NULL) {
                atual->fdir = novo;
                break;
            } else {
                atual = atual->fdir;
            }
        } else {
            free(novo);
            return 0;
        }
    }

    novo->pai = atual;
    atualizaFB_insercao(arv, novo);
    arv->numElementos++;
    return 1;
}

no *getRaiz(avl *arv) {
    return arv->raiz;
}

int getNumElementos(avl *arv) {
    return arv->numElementos;
}

void atualizaFB_insercao(avl *arv, no *novoNo) {
    no *atual = novoNo->pai;
    while (atual != arv->sentinela) {
        if (novoNo == atual->fesq) {
            atual->fb--;
        } else {
            atual->fb++;
        }
        if (atual->fb == 0) break;
        if (atual->fb == 2 || atual->fb == -2) {
            balanceamento(arv, atual);
            rotacoes_insercao++; 
            break;
        }
        novoNo = atual;
        atual = atual->pai;
    }
}

void balanceamento(avl *arv, no *noDesbal) {
    if (noDesbal->fb == 2) {
        if (noDesbal->fdir->fb >= 0) {
            rotacaoEsq(arv, noDesbal);
        } else {
            rotacaoDir(arv, noDesbal->fdir);
            rotacaoEsq(arv, noDesbal);
        }
    } else if (noDesbal->fb == -2) {
        if (noDesbal->fesq->fb <= 0) {
            rotacaoDir(arv, noDesbal);
        } else {
            rotacaoEsq(arv, noDesbal->fesq);
            rotacaoDir(arv, noDesbal);
        }
    }
}

void rotacaoDir(avl *arv, no *noDesbal) {
    no *fesqFilho = noDesbal->fesq;
    noDesbal->fesq = fesqFilho->fdir;
    if (fesqFilho->fdir != NULL) {
        fesqFilho->fdir->pai = noDesbal;
    }
    fesqFilho->pai = noDesbal->pai;
    if (noDesbal->pai == arv->sentinela) {
        arv->raiz = fesqFilho;
    } else if (noDesbal == noDesbal->pai->fesq) {
        noDesbal->pai->fesq = fesqFilho;
    } else {
        noDesbal->pai->fdir = fesqFilho;
    }
    fesqFilho->fdir = noDesbal;
    noDesbal->pai = fesqFilho;
    noDesbal->fb = noDesbal->fb + 1 - (fesqFilho->fb < 0 ? fesqFilho->fb : 0);
    fesqFilho->fb = fesqFilho->fb + 1 + (noDesbal->fb > 0 ? noDesbal->fb : 0);
}

void rotacaoEsq(avl *arv, no *noDesbal) {
    no *fdirFilho = noDesbal->fdir;
    noDesbal->fdir = fdirFilho->fesq;
    if (fdirFilho->fesq != NULL) {
        fdirFilho->fesq->pai = noDesbal;
    }
    fdirFilho->pai = noDesbal->pai;
    if (noDesbal->pai == arv->sentinela) {
        arv->raiz = fdirFilho;
    } else if (noDesbal == noDesbal->pai->fdir) {
        noDesbal->pai->fdir = fdirFilho;
    } else {
        noDesbal->pai->fesq = fdirFilho;
    }
    fdirFilho->fesq = noDesbal;
    noDesbal->pai = fdirFilho;
    noDesbal->fb = noDesbal->fb - 1 - (fdirFilho->fb > 0 ? fdirFilho->fb : 0);
    fdirFilho->fb = fdirFilho->fb - 1 + (noDesbal->fb < 0 ? noDesbal->fb : 0);
}

void atualizaFB_remocao(avl *arv, no *pai, int chaveRemovida) {
    no *noAux = pai;
    if (noAux == arv->sentinela) {
        return;
    }

    if (chaveRemovida < noAux->chave) {
        noAux->fb++;
    } else {
        noAux->fb--;
    }

    while (noAux != arv->sentinela) {
        if (noAux->fb == 2 || noAux->fb == -2) {
            balanceamento(arv, noAux);
            rotacoes_remocao++; 
        }
        if (noAux->fb == 1 || noAux->fb == -1) {
            break;
        }
        noAux = noAux->pai;
    }
}

int removeNo(avl *arv, int valor) {
    no *sentinela = arv->sentinela;
    no *atual = arv->raiz;

    while (atual != NULL && atual->chave != valor) {
        if (valor < atual->chave) {
            atual = atual->fesq;
        } else {
            atual = atual->fdir;
        }
    }

    if (atual == NULL) {
        return 0;
    }

    no *pai = atual->pai;
    no *substituto;
    if (atual->fesq == NULL) {
        substituto = atual->fdir;
    } else if (atual->fdir == NULL) {
        substituto = atual->fesq;
    } else {
        no *min = atual->fdir;
        while (min->fesq != NULL) {
            min = min->fesq;
        }
        atual->chave = min->chave;
        return removeNo(arv, min->chave);
    }

    if (substituto != NULL) {
        substituto->pai = pai;
    }

    if (pai == sentinela) {
        arv->raiz = substituto;
    } else if (atual == pai->fesq) {
        pai->fesq = substituto;
    } else {
        pai->fdir = substituto;
    }

    atualizaFB_remocao(arv, pai, valor);
    arv->numElementos--;
    free(atual);
    return 1;
}

void preencheArvoreOrdenada(avl *arv) {
    for (int i = 0; i < 10000; i++) {
        insereNo(arv, i);
    }
}

void busca(avl *arv) {
    clock_t inicio, fim;
    double cpu_time_used;

    inicio = clock();
    for (int i = 0; i < 1000; i++) {
        no *atual = arv->raiz;
        while (atual != NULL && atual->chave != i) {
            if (i < atual->chave) {
                atual = atual->fesq;
            } else {
                atual = atual->fdir;
            }
        }
    }
    fim = clock();
    cpu_time_used = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

    printf("Tempo de busca na avl: %f segundos\n", cpu_time_used);
}