#ifndef AVL_H
#define AVL_H

typedef struct no no;
typedef struct avl avl;
avl *criaArvore();
int insereNo(avl *arv, int valor);
int removeNo(avl *arv, int valor);
no *getRaiz(avl *arv);
int getNumElementos(avl *arv);
void atualizaFB_insercao(avl *arv, no *novoNo);
void balanceamento(avl *arv, no *noDesbal);
void rotacaoDir(avl *arv, no *noDesbal);
void rotacaoEsq(avl *arv, no *noDesbal);
void atualizaFB_remocao(avl *arv, no *pai, int chaveRemovida);
void preencheArvoreOrdenada(avl *arv);
void busca(avl *arv);

extern int rotacoes_insercao;
extern int rotacoes_remocao;

#endif