#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BLACK 0
#define RED 1

typedef struct arvoreRB
{
	char info[100];
	int count;
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

int buscar(ArvoreRB *a, char v[])
{
	return a != NULL && (strcmp(v, a->info) < 0 ? buscar(a->esq, v) : (strcmp(v, a->info) > 0 ? buscar(a->dir, v) : 1));
}

void printTreeOrder(ArvoreRB *a)
{
	if (!a)
		return;
	printTreeOrder(a->esq);
	printf("%s - %d\n", a->info, a->count);
	printTreeOrder(a->dir);
}

void printTree(ArvoreRB *a, int height)
{
	if (a != NULL)
	{
		printf("%d - %s / %d : %s\n", height, a->info, a->count, a->cor ? "RED" : "BLACK");
		printTree(a->esq, height + 1);
		printTree(a->dir, height + 1);
	}
}

ArvoreRB *inserir(ArvoreRB *a, char v[])
{
	int change = 0;
	if (a == NULL)
	{
		a = (ArvoreRB *)malloc(sizeof(ArvoreRB));
		strcpy(a->info, v);
		a->count = 1;
		a->cor = BLACK;
		a->esq = a->dir = NULL;
	}
	else if (strcmp(v, a->info) < 0)
	{
		change = a->esq == NULL;
		a->esq = inserir(a->esq, v);
		if (change)
			a->esq->cor = RED;
	}
	else if (strcmp(v, a->info) > 0)
	{
		change = a->dir == NULL;
		a->dir = inserir(a->dir, v);
		if (change)
			a->dir->cor = RED;
	}
	else
		a->count++;

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
			   : ((a->esq == NULL || strcmp(a->info, a->esq->info) > 0) && (a->dir == NULL || strcmp(a->info, a->dir->info) < 0) && arv_bin_check(a->esq) && arv_bin_check(a->dir));
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

	return arv_rb_check(a->esq, myHeight + is_black_node(a), height) && arv_rb_check(a->dir, myHeight + is_black_node(a), height);
}

void getTheBestWords(ArvoreRB *a, int quantity, char **bestWords, int wordsCount[])
{
	if (!a)
		return;

	for (int i = quantity - 1; i >= 0; i--)
	{
		if (a->count <= wordsCount[i])
		{
			if (i == quantity - 1)
				break;
			strcpy(bestWords[i], a->info);
			wordsCount[i] = a->count;
		}
		if (i == 0)
		{
			strcpy(bestWords[i], a->info);
			wordsCount[i] = a->count;
		}
		else
		{
			strcpy(bestWords[i], bestWords[i - 1]);
			wordsCount[i] = wordsCount[i - 1];
		}
	}

	getTheBestWords(a->esq, quantity, bestWords, wordsCount);
	getTheBestWords(a->dir, quantity, bestWords, wordsCount);
}

int showOptions()
{
	printf("OPÇÕES\n\n--freq N ARQUIVO\nExibe o número de ocorrência das N palavras que mais aparecem em ARQUIVO, em ordem decrescente de ocorrência.\n\n--freq-word PALAVRA ARQUIVO\nExibe o número de ocorrências de PALAVRA em ARQUIVO.\n\n--search TERMO ARQUIVO [ARQUIVO ...]\nExibe uma listagem dos ARQUIVOS mais relevantes encontrados pela busca por TERMO. A listagem é apresentada em ordem descrescente de relevância. TERMO pode conter mais de uma palavra. Neste caso, deve ser indicado entre àspas.\n");
	return -1;
}

int freq(int words, char *file)
{
	ArvoreRB *a;

	/* Try to open file */
	FILE *fptr = fopen(file, "r");

	/* Exit if file not opened successfully */
	if (fptr == NULL)
	{
		printf("\nArquivo não encontrado ou sem permissão de leitura.\n");
		exit(EXIT_FAILURE);
	}

	char c;
	int i = 0, len;
	char wordRead[100];
	char wordReadStripped[100];
	while ((c = getc(fptr)) != EOF)
	{
		c = tolower(c);
		if ((c > 47 && c < 58) || (c >= 97 && c <= 122))
		{
			wordRead[i] = c;
			i++;
		}
		else if (c == ' ' || c == '\n' || c == 32 || c == '-')
		{
			wordRead[i] = '\0';
			i = 0;

			len = strlen(wordRead);
			if (len > 1)
			{
				// remove special chars
				int j = 0, k = 0;
				for (; j < strlen(wordRead); j++)
				{
					if (isalnum(wordRead[j]))
					{
						wordReadStripped[k] = wordRead[j];
						k++;
					}
				}
				wordReadStripped[k] = '\0';

				a = inserir(a, wordReadStripped);
			}
		}
	}

	char *resultString[words];
	int resultInt[words];

	for (int i = 0; i < words; i++)
	{
		resultString[i] = (char *)malloc(100 * sizeof(char));
		strcpy(resultString[i], "");
		resultInt[i] = 0;
	}

	getTheBestWords(a, words, resultString, resultInt);

	// printTree(a, 1);
	// printf("\nBINARY OK: %d\n", arv_bin_check(a));
	// printf("RED BLACK OK: %d\n\n", arv_rb_check(a, 0, get_tree_height(a)));
	// printTreeOrder(a);

	printf("As %d palavras mais usadas no arquivo %s:\n", words, file);
	// Close file and tree
	for (int i = 0; i < words; i++)
	{
		printf("%s - %d vezes\n", resultString[i], resultInt[i]);
		free(resultString[i]);
	}
	fclose(fptr);
	arv_libera(a);

	return 1;
}

int freqWord(char *word, char *file)
{
	int count = 0, len;

	/* Try to open file */
	FILE *fptr = fopen(file, "r");

	/* Exit if file not opened successfully */
	if (fptr == NULL)
	{
		printf("\nArquivo não encontrado ou sem permissão de leitura.\n");
		exit(EXIT_FAILURE);
	}

	char c;
	int i = 0;
	char wordRead[100];
	char wordReadStripped[100];
	while ((c = getc(fptr)) != EOF)
	{
		c = tolower(c);
		if ((c > 47 && c < 58) || (c >= 97 && c <= 122))
		{
			wordRead[i] = c;
			i++;
		}
		else if (c == ' ' || c == '\n' || c == 32 || c == '-')
		{
			wordRead[i] = '\0';
			i = 0;

			// remove special chars
			int j = 0, k = 0;
			for (; j < strlen(wordRead); j++)
			{
				if (isalnum(wordRead[j]))
				{
					wordReadStripped[k] = wordRead[j];
					k++;
				}
			}
			wordReadStripped[k] = '\0';

			if (strcmp(wordReadStripped, word) == 0)
				count++;
		}
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
		return showOptions();

	if (strcmp(args[1], "--freq") == 0 && argsLength == 4)
		return freq(atoi(args[2]), args[3]);
	else if (strcmp(args[1], "--freq-word") == 0 && argsLength == 4)
		return freqWord(args[2], args[3]);
	else if (strcmp(args[1], "--search") == 0 && argsLength >= 4)
		return search(argsLength, args);
	else
		return showOptions();

	return 1;
}
