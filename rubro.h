#ifndef RUBRO_H
#define RUBRO_H

typedef enum { VERMELHO, PRETO } CorNo;
typedef struct NoRB {
    int dado;
    CorNo cor;
    struct NoRB *esquerda, *direita, *pai;
} NoRB;

typedef struct ArvoreRB {
    NoRB *raiz;
    NoRB *NIL;
    int rotacoes;
} ArvoreRB;

NoRB* criarNo(int dado, CorNo cor, NoRB* NIL);
ArvoreRB* criarArvore();
void rotacaoEsquerda(ArvoreRB* arvore, NoRB* x);
void rotacaoDireita(ArvoreRB* arvore, NoRB* y);
void corrigirInsercao(ArvoreRB* arvore, NoRB* z);
void inserir(ArvoreRB* arvore, int dado);
void corrigirRemocao(ArvoreRB* arvore, NoRB* x);
void transplante(ArvoreRB* arvore, NoRB* u, NoRB* v);
NoRB* minimoArvore(NoRB* node, NoRB* NIL);
void removerNo(ArvoreRB* arvore, NoRB* z);
void preencheArvore(ArvoreRB* arvore);
void medirInsercao(ArvoreRB* arvore, int* valores, int n);
void medirRemocao(ArvoreRB* arvore, int* valores, int n);
void medirBusca(ArvoreRB* arvore, int* valores, int n);

#endif // RUBRO_H
