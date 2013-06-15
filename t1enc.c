/*=================================================*/
/*|||||||||||||||| MAC 323 - EP4 ||||||||||||||||||*/
/*||||| Nome: Carlos Eduardo Leao Elmadjian |||||||*/
/*||||| NUSP: 5685741 |||||||||||||||||||||||||||||*/
/*||||| Arquivo: t1enc.c ||||||||||||||||||||||||||*/
/*=================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "interface.h"

/*========================= Definicao de tipos e structs =============================*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*Sentenca: guarda o id, a frase e a sentenca anotada (info)*/
typedef struct sentenca
{
	int id[2];
	int frase[2];
	int info[2];
} Sentenca;

/*Node: define um no' (ou entrada) da tabela de simbolos. Guarda uma 
  chave do tipo char, uma lista do tipo Valor e a quantidade de objetos dessa 
  chave (n)*/
typedef struct node
{
	Valor *chave;
	struct node *ant;
	struct node *prox;
	int n;
} Node;


/*========================= Variaveis privadas =======================================*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
static Node *t1enc_head;   /*inicio da tabela*/
static Sentenca sentences;  /*guarda posicoes do id, frase e sent. anotada no STREAM*/ 
static int M;			   /*numero de entradas disponiveis na tabela*/
static int N;              /*numero de entradas preenchidas na tabela*/




/*========================= Implementacao das funcoes ================================*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  insertVal
 *  Description:  Insere um Valor novo numa lista do tipo Valor. Nao obedece ordem
                  de entrada por motivos de performance.
 * =====================================================================================
 */
Node* insertEnc(Node *ini, Node *novo)
{
	Node *p;
	/*caso a lista esteja vazia, novo inicia a lista*/	
	if (ini->chave == NULL)
		return ini = novo;
	/*senao insere novo no fim da fila*/
	p = ini->ant;
	p->prox = novo;
	ini->ant = novo;
	novo->prox = ini;
	novo->ant = p;
	return ini;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  buildVal
 *  Description:  Cria um novo Valor, que contem uma palavra, um lema correspondente,
                  indicadores de posicao inicial e final no texto, e id da sentenca.
 * =====================================================================================
 */
Valor* buildVal(char *palavra, char *lema, int sent_id)
{
	/*aloca espaco para atributos de Valor*/
	char *pal = malloc((strlen(palavra)+1) * sizeof(char));
	char *lem = malloc((strlen(lema)+1) * sizeof(char));
	Valor *val = malloc(sizeof *val);
	/*salva atributos dados*/
	strcpy(pal, palavra);
	strcpy(lem, lema);
	val->palavra = pal;
	val->lema = lem;
	val->sentenca = sent_id;
	return val;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  printValorPal
 *  Description:  Imprime o conteudo de uma sentenca atrelado a uma palavra (Valor).
                  modo 0: conf. printSentence em modo 0.
				  modo 1: conf. printSentence em modo 1.
				  modo 2: conf. printSentence em modo 2.
 * =====================================================================================
 */
void printValorPal(Valor *val, char *palavra, int modo)
{
	Valor *t = val;
	int i = 0, frase_id = -1;

	if (t == NULL)
		printf("Nenhum resultado encontrado\n");
	while (t != NULL)
	{
		if (strcmp(t->palavra, palavra) == 0)
		{
			if (t->sentenca != frase_id)
			{
				printSentence(t->sentenca, modo);
				printf("\n");
			}
			i++;
		}
		frase_id = t->sentenca;
		t = t->prox;
	}
	if (i == 0 && val != NULL)
		printf("Nenhum resultado encontrado\n");
	printf("\n");
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  printValorLem
 *  Description:  Imprime o conteudo de uma sentenca atrelado a um lema.
                  modo 0: conf. printSentence em modo 0.
				  modo 1: conf. printSentence em modo 1.
				  modo 2: conf. printSentence em modo 2.
				  modo 3: imprime lema e suas derivacoes.
 * =====================================================================================
 */
void printValorLem(Valor *val, int modo)
{
	char **pal;
	int i = 0, j, k, frase_id = -1;
	Valor *t = val;

	if (t == NULL)
		printf("Nenhum resultado encontrado\n");

	/* imprime derivacoes do lema */
	if (modo == 3)
	{
		while (t != NULL)
		{
			i++;
			t = t->prox;
		}
		pal = malloc(i * sizeof(char*));
		t = val;
		j = 0;
		printf(" -->");
		while (t != NULL)
		{
			k = 0;
			pal[j] = t->palavra;
			for (i = 0; i < j; i++)
			{
				if (strcmp(pal[i], t->palavra) == 0)
				{
					k = 1;
					break;
				}
			}
			if (k != 1)
				printf(" %s", t->palavra);
			j++;
			t = t->prox;
		}
		free(pal);
	}
	/*imprime sentencas*/
	else
	{
		while (t != NULL)
		{
			if (t->sentenca != frase_id) 
			{
				printSentence(t->sentenca, modo);
				printf("\n");
			}
			frase_id = t->sentenca;
			t = t->prox;
		}
	}
	printf("\n");
}





/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  size
 *  Description:  Devolve o numero de filhos de um no (contando com ele mesmo).
 * =====================================================================================
 */
int ST_t1_count();
{
	return N;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  newNode
 *  Description:  Cria um novo no que recebe uma palavra "chave", um Valor e uma cor.
 * =====================================================================================
 */
Node* newNode(Valor *val)
{
	Node *novo = malloc(sizeof *novo);
	novo->chave = val;
	/*condicao para existir uma fila circular num no' novo*/
	novo->ant = novo->prox = novo;
	return novo;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ST_t1_insert
 *  Description:  Insere um valor na tabela de simbolos t1 para o cliente. Utiliza
                  metodo de hashing por encadeamento e tem comportamento dinamico.
 * =====================================================================================
 */
void ST_t1_insert(Valor *val)
{
	int i;
	if (N > M/2)
		/*reinsert*/
	i = hash(val->palavra, M);
	t1_enc_head[i] = insertEnc(t1_enc_head[i]->chave, newNode(val));
	N++;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  searchRec
 *  Description:  Funcao de busca recursiva na LLRBT.
 * =====================================================================================
 */
Valor* searchRec(Node *x, char *chave)
{
	if (x == NULL) 
		return NULL;
	if (strcmp(x->chave, chave) == 0)
		return x->chave;
	return searchRec(x->prox, chave);	
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ST_t1_search
 *  Description:  
 * =====================================================================================
 */
Valor* ST_t1_search(char *chave)
{
	return searchRec(t1_enc_head[hash(chave, M)]);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ST_t1_init
 *  Description:  
 * =====================================================================================
 */
void ST_t1_init()
{
	N = 0;
	M = 3571; /*500o maior primeiro primo*/
	t1_enc_head = malloc(M * sizeof(Node));
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chavesRec
 *  Description:  Devolve todos os tokens ou palavras encontradas na LLRBT.
                  modo 0: tokens.
				  modo 1: palavras.
 * =====================================================================================
 */
void chavesRec(Node *raiz, char **tokens, int modo)
{
	if (raiz != NULL)
	{
		chavesRec(raiz->esquerda, tokens, modo);
		chavesRec(raiz->direita, tokens, modo);
		if (modo == 0)
			tokens[cnt++] = raiz->chave;
		else if (modo == 1)
		{
			if (isalpha(raiz->chave[0]))
				tokens[cnt++] = raiz->chave;
		}
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chavePalST
 *  Description:  Devolve lista de tokens ou palavras para o cliente.
                  modo 0: tokens.
				  modo 1: palavras.
 * =====================================================================================
 */
void chavePalST(int modo) 
{
	char **tokens;
	int i, tam = rootPalavra->filhos;

	cnt = 0;
	tokens = malloc(tam * sizeof(char*));
	chavesRec(rootPalavra, tokens, modo);

	qsort(tokens, cnt, sizeof(tokens[0]), comparaString);
	for (i = 0; i < cnt; i++)
		printf("%s\n", tokens[i]);	
	printf("\n");
	free(tokens);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chaveLemST
 *  Description:  Devolve lista de lemas ou lemas+derivacoes para o cliente.
                  modo 0: lemas.
				  modo 1: lemas+derivacoes.
 * =====================================================================================
 */
void chaveLemST(int modo)
{
	char **tokens;
	int i, tam = rootLemma->filhos;

	cnt = 0;
	tokens = malloc(tam * sizeof(char*));
	chavesRec(rootLemma, tokens, 0);
	qsort(tokens, tam, sizeof(tokens[0]), comparaString);

	if (modo == 1)
	{
		for (i = 0; i < tam; i++)
		{
			printf("%s ", tokens[i]);
			printValorLem(searchRec(rootLemma, tokens[i]), 3);
		}
	}
	else
	{
		for (i = 0; i < tam; i++)
			printf("%s\n", tokens[i]);	
	}
	printf("\n");
	free(tokens);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  contaValDistST
 *  Description:  Devolve uma lista de valores distintos para tokens, palavras e lemas.
                  modo 0: tokens distintos.
				  modo 1: palavras distintas.
				  modo 2: lemas distintos.
 * =====================================================================================
 */
int contaValDistST(int modo)
{
	char **temp = malloc(rootPalavra->filhos * sizeof(char*));
	switch(modo)
	{
		case 0:
			return rootPalavra->filhos;
		case 1:
			cnt = 0;
			chavesRec(rootPalavra, temp, 1);
			return cnt;
		case 2:
			return rootLemma->filhos;

	}
	return 0;
}

