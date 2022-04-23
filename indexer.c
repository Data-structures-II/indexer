#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *strlwr(char *str)
{
	unsigned char *p = (unsigned char *)str;

	while (*p)
	{
		*p = tolower((unsigned char)*p);
		p++;
	}

	return str;
}

#define BLACK 0
#define RED 1

typedef struct arvoreRB
{
	int info;
	int cor;
	struct arvoreRB *esq;
	struct arvoreRB *dir;
} ArvoreRB;

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

int showOptions()
{
	printf("OPÇÕES\n\n--freq N ARQUIVO\nExibe o número de ocorrência das N palavras que mais aparecem em ARQUIVO, em ordem decrescente de ocorrência.\n\n--freq-word PALAVRA ARQUIVO\nExibe o número de ocorrências de PALAVRA em ARQUIVO.\n\n--search TERMO ARQUIVO [ARQUIVO ...]\nExibe uma listagem dos ARQUIVOS mais relevantes encontrados pela busca por TERMO. A listagem é apresentada em ordem descrescente de relevância. TERMO pode conter mais de uma palavra. Neste caso, deve ser indicado entre àspas.\n");
	return -1;
}

int freq(int words, char *file)
{
	return showOptions();
}

int freqWord(char *word, char *file)
{
	int count = 0, len;
	char wordRead[100];

	/* Try to open file */
	FILE *fptr = fopen(file, "r");

	/* Exit if file not opened successfully */
	if (fptr == NULL)
	{
		printf("\nArquivo não encontrado ou sem permissão de leitura.\n");
		exit(EXIT_FAILURE);
	}

	while (fscanf(fptr, "%s", wordRead) != EOF)
	{
		// Convert word to lowercase
		strlwr(wordRead);

		// Remove last punctuation character
		len = strlen(wordRead);
		if (ispunct(wordRead[len - 1]))
			wordRead[len - 1] = '\0';

		if (strcmp(wordRead, word) == 0)
			count++;
	}

	// Close file
	fclose(fptr);

	printf("\nA palavra %s ocorre %d vezes no arquivo %s.\n", word, count, file);
	return 1;
}

int search(int argsLength, char **args)
{
	for (int i = 0; i < argsLength; ++i)
	{
		printf("args[%d]: %s\n", i, args[i]);
	}
	return showOptions();
}

int main(int argsLength, char **args)
{
	if (argsLength <= 3)
	{
		return showOptions();
	}

	if (strcmp(args[1], "--freq") == 0 && argsLength == 4)
	{
		return freq(atoi(args[2]), args[3]);
	}
	else if (strcmp(args[1], "--freq-word") == 0 && argsLength == 4)
	{
		return freqWord(args[2], args[3]);
	}
	else if (strcmp(args[1], "--search") == 0 && argsLength >= 4)
	{
		return search(argsLength, args);
	}
	else
	{
		return showOptions();
	}

	return 1;
	// ArvoreRB *a;
	// a = inserir(a, 5);
	// a = inserir(a, 3);
	// a = inserir(a, 9);
	// a = inserir(a, 1);
	// a = inserir(a, 4);
	// a = inserir(a, 7);
	// a = inserir(a, 2);
	// a = inserir(a, 6);
	// a = inserir(a, 8);
	// a = inserir(a, 10);
	// printTree(a, 1);
	// printf("\nBINARY OK: %d\n", arv_bin_check(a));
	// printf("RED BLACK OK: %d\n\n", arv_rb_check(a, 0, get_tree_height(a)));
	// printTreeOrder(a);
	// arv_libera(a);
}
