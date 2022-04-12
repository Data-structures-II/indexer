#include <stdio.h>
#include <stdlib.h>

#define DOUBLE_BLACK -1
#define BLACK 0
#define RED 1

typedef struct arvoreRB
{
  int info;
  int cor;
  struct arvoreRB *esq;
  struct arvoreRB *dir;
} ArvoreRB;

int is_double_black_node(ArvoreRB *no)
{
  return no && no->cor == DOUBLE_BLACK;
}

int is_black_node(ArvoreRB *no)
{
  return no && no->cor == BLACK;
}

int is_red_node(ArvoreRB *no)
{
  return no && no->cor == RED;
}

int verifica_arv_vazia(ArvoreRB *a)
{
  return (a == NULL);
}

ArvoreRB *rot_esq(ArvoreRB *no)
{
  ArvoreRB *tree = no->dir;
  no->dir = tree->esq;
  tree->esq = no;
  tree->cor = no->cor;
  no->cor = RED;
  return (tree);
}

ArvoreRB *rot_dir(ArvoreRB *no)
{
  ArvoreRB *tree = no->esq;
  no->esq = tree->dir;
  tree->dir = no;
  tree->cor = no->cor;
  no->cor = RED;
  return (tree);
}

void flip_cor(ArvoreRB *no)
{
  no->cor = RED;
  no->esq->cor = BLACK;
  no->dir->cor = BLACK;
}

ArvoreRB *fixRBTree(ArvoreRB *a)
{
  if (is_red_node(a->dir) && is_black_node(a->esq))
    a = rot_esq(a);
  else if (is_red_node(a->esq) && is_red_node(a->esq->esq))
    a = rot_dir(a);
  else if (is_red_node(a->dir) && is_red_node(a->esq))
    flip_cor(a);

  return a;
}

int buscar(ArvoreRB *a, int v)
{
  return a != NULL && (v < a->info ? buscar(a->esq, v) : (v > a->info ? buscar(a->dir, v) : 1));
}

void printTreeOrder(ArvoreRB *a)
{
  if (!a)
    return;
  printTreeOrder(a->esq);
  printf("%d ", a->info);
  printTreeOrder(a->dir);
}

void printTree(ArvoreRB *a, int height)
{
  if (a != NULL)
  {
    printf("%d - %d : %s\n", height, a->info, a->cor ? "RED" : "BLACK");
    printTree(a->esq, height + 1);
    printTree(a->dir, height + 1);
  }
}

ArvoreRB *inserir(ArvoreRB *a, int v)
{
  int change = 0;
  if (a == NULL)
  {
    a = (ArvoreRB *)malloc(sizeof(ArvoreRB));
    a->info = v;
    a->cor = BLACK;
    a->esq = a->dir = NULL;
  }
  else if (v < a->info)
  {
    change = a->esq == NULL;
    a->esq = inserir(a->esq, v);
    if (change)
      a->esq->cor = RED;
  }
  else
  {
    change = a->dir == NULL;
    a->dir = inserir(a->dir, v);
    if (change)
      a->dir->cor = RED;
  }

  change = 0;

  return fixRBTree(a);
}

// https://iq.opengenus.org/red-black-tree-deletion/
// http://www.stolerman.net/studies/cs521/red_black_trees.pdf
// https://www.cs.purdue.edu/homes/ayg/CS251/slides/chap13c.pdf
// https://algs4.cs.princeton.edu/33balanced/RedBlackBST.java.html
ArvoreRB *remover(ArvoreRB *a, int x)
{
  ArvoreRB *aux, *pai_aux;
  int filhos = 0, tmp, isBlack;

  if (!a)
    return (NULL);

  if (a->info < x)
  {
    isBlack = is_black_node(a->dir);
    a->dir = remover(a->dir, x);
    if ((isBlack && is_red_node(a)) || (!isBlack && is_black_node(a)))
    {
      if (a->dir == NULL)
        a->cor = BLACK;
      else if (a->dir->esq == NULL && a->dir->dir == NULL)
        a->dir->cor = BLACK;
    }
    else if (isBlack && is_black_node(a))
      // a = rot_dir(a);
      if (verifica_arv_vazia(a->dir) && verifica_arv_vazia(a->esq))
        a->cor = DOUBLE_BLACK;
      else if (!verifica_arv_vazia(a->dir))
        a->dir->cor = DOUBLE_BLACK;
  }
  else if (a->info > x)
  {
    isBlack = is_black_node(a->esq);
    a->esq = remover(a->esq, x);
    if ((isBlack && is_red_node(a)) || (!isBlack && is_black_node(a)))
    {
      if (a->esq == NULL)
        a->cor = BLACK;
      else if (a->esq->esq == NULL && a->esq->dir == NULL)
        a->esq->cor = BLACK;
    }
    else if (isBlack && is_black_node(a))
    {
      // a = rot_esq(a);
      if (verifica_arv_vazia(a->esq) && verifica_arv_vazia(a->dir))
        a->cor = DOUBLE_BLACK;
      else if (!verifica_arv_vazia(a->esq))
        a->esq->cor = DOUBLE_BLACK;
    }
  }
  else
  {
    if (a->esq)
      filhos++;
    if (a->dir)
      filhos++;

    if (filhos == 0)
    {
      free(a);
      return (NULL);
    }
    else if (filhos == 1)
    {
      aux = a->esq ? a->esq : a->dir;
      free(a);
      return (aux);
    }
    else
    {
      aux = a->esq;
      pai_aux = a;
      while (aux->dir)
      {
        pai_aux = aux;
        aux = aux->dir;
      }
      tmp = a->info;
      a->info = aux->info;
      aux->info = tmp;
      pai_aux->dir = remover(aux, tmp);
      return (a);
    }
  }

  // Checking double black
  if (is_double_black_node(a->esq))
  {
    if (is_red_node(a->dir))
      a = rot_esq(a);
    if (is_black_node(a->dir) && is_black_node(a->dir->esq) && is_black_node(a->dir->dir))
    {
      a->cor = DOUBLE_BLACK;
      a->dir->cor = RED;
    }
    if (is_black_node(a->dir) && is_red_node(a->dir->esq) && is_black_node(a->dir->dir))
      a = rot_dir(a->dir);
    if (is_black_node(a->dir) && is_black_node(a->dir->esq) && is_red_node(a->dir->dir))
    {
      a->dir->cor = a->cor;
      a->dir->dir->cor = BLACK;
      a = rot_esq(a);
      a->cor = BLACK;
    }
  }
  else if (is_double_black_node(a->dir))
  {
    if (is_red_node(a->esq))
      a = rot_esq(a);
    if (is_black_node(a->esq) && is_black_node(a->esq->esq) && is_black_node(a->esq->dir))
    {
      a->cor = DOUBLE_BLACK;
      a->dir->cor = RED;
    }
    if (is_black_node(a->esq) && is_red_node(a->esq->esq) && is_black_node(a->esq->dir))
      a = rot_dir(a->esq);
    if (is_black_node(a->esq) && is_black_node(a->esq->esq) && is_red_node(a->esq->dir))
    {
      a->esq->cor = a->cor;
      a->esq->dir->cor = BLACK;
      a = rot_esq(a);
      a->cor = BLACK;
    }
  }

  return fixRBTree(a);
}

ArvoreRB *arv_libera(ArvoreRB *a)
{
  if (!verifica_arv_vazia(a))
  {
    arv_libera(a->esq);
    arv_libera(a->dir);
    free(a);
  }
  return NULL;
}

int arv_bin_check(ArvoreRB *a)
{
  return a == NULL
             ? 1
             : ((a->esq == NULL || a->info > a->esq->info) && (a->dir == NULL || a->info < a->dir->info) && arv_bin_check(a->esq) && arv_bin_check(a->dir));
}

int get_tree_height(ArvoreRB *a)
{
  int height = 0;
  ArvoreRB *no = a;
  while (no != NULL)
  {
    if (is_black_node(no))
      height++;
    no = no->esq;
  }
  return height;
}

int arv_rb_check(ArvoreRB *a, int myHeight, int height)
{
  if (a == NULL)
    return myHeight == height;

  return arv_rb_check(a->esq, myHeight + !(is_red_node(a)), height) && arv_rb_check(a->dir, myHeight + !(is_red_node(a)), height);
}

int main()
{
  ArvoreRB *a;
  a = inserir(a, 5);
  a = inserir(a, 3);
  a = inserir(a, 9);
  a = inserir(a, 1);
  a = inserir(a, 4);
  a = inserir(a, 7);
  a = inserir(a, 2);
  a = inserir(a, 6);
  a = inserir(a, 8);
  a = inserir(a, 10);
  printTree(a, 1);
  printf("\nBINARY OK: %d\n", arv_bin_check(a));
  printf("RED BLACK OK: %d\n\n", arv_rb_check(a, 0, get_tree_height(a)));
  // a = remover(a, 1);
  // a = remover(a, 6);
  // a = remover(a, 8);
  a = remover(a, 9);
  a = remover(a, 4);
  a = remover(a, 7);
  printTree(a, 1);
  printf("\nBINARY OK: %d\n", arv_bin_check(a));
  printf("RED BLACK OK: %d\n\n", arv_rb_check(a, 0, get_tree_height(a)));
  // printTreeOrder(a);
  arv_libera(a);
}
