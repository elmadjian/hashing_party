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


/*Node: define um no' (ou entrada) da tabela de simbolos. Guarda uma 
  chave do tipo char, uma lista do tipo Valor e a quantidade de objetos dessa 
  chave (n)*/
typedef struct node
{
	Valor *valor;
	int n;
	struct node *prox;
} Node;


/*========================= Variaveis privadas =======================================*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
static Node **t1enc_head;   /*inicio da tabela*/
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
Valor* insertVal(Valor *ini, Valor *novo)
{
	Valor *p;
	/*caso a lista esteja vazia, novo inicia a lista*/	
	if (ini == NULL)
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
 *         Name:  size
 *  Description:  Devolve o numero de filhos de um no (contando com ele mesmo).
 * =====================================================================================
 */
int ST_t1_count(int modo)
{
	return N;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  newNode
 *  Description:  Cria um novo no que recebe uma palavra "chave", um Valor e uma cor.
 * =====================================================================================
 */
Node* newNode(Valor *val, Node *proximo)
{
	Node *novo = malloc(sizeof *novo);
	novo->valor = insertVal(novo->valor, val);
	novo->n = 1;
	novo->prox = proximo;
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
	Node *p;
	/*condicao para remapear os dados*/
	if (N > M/2)
	{
		/*reinsert*/
		printf("xi, estourou o limite!\n");
		exit(EXIT_FAILURE);
	}
	i = hash(val->palavra, M);
	printf("%s->%d\n",val->palavra, i);
	p = t1enc_head[i];

	/*verifica se ha recorrencia de um valor e insere-o*/
	while (p != NULL && p->valor != NULL)
	{
		if (strcmp(p->valor->palavra, val->palavra) == 0)
		{
			p->valor = insertVal(p->valor, val);
			p->n++;	
			break;
		}
		p = p->prox;
	}
	/*caso nao haja "match", insira no proximo node livre*/
	if (p == NULL)
		t1enc_head[i] = newNode(val, t1enc_head[i]);
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
	if (strcmp(x->valor->palavra, chave) == 0)
		return x->valor;
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
	return searchRec(t1enc_head[hash(chave, M)], chave);
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
	M = 193; /*primo mais proximo de 2^7*/
	t1enc_head = malloc(M * sizeof(Node));
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chavesRec
 *  Description:  Devolve todos os tokens ou palavras encontradas na LLRBT.
                  modo 0: tokens.
				  modo 1: palavras.
 * =====================================================================================
 */
/* void chavesRec(Node *raiz, char **tokens, int modo)
 * {
 * 	if (raiz != NULL)
 * 	{
 * 		chavesRec(raiz->esquerda, tokens, modo);
 * 		chavesRec(raiz->direita, tokens, modo);
 * 		if (modo == 0)
 * 			tokens[cnt++] = raiz->chave;
 * 		else if (modo == 1)
 * 		{
 * 			if (isalpha(raiz->chave[0]))
 * 				tokens[cnt++] = raiz->chave;
 * 		}
 * 	}
 * }
 */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chavePalST
 *  Description:  Devolve lista de tokens ou palavras para o cliente.
                  modo 0: tokens.
				  modo 1: palavras.
 * =====================================================================================
 */

/* void chavePalST(int modo) 
 * {
 * 	char **tokens;
 * 	int i, tam = rootPalavra->filhos;
 * 
 * 	cnt = 0;
 * 	tokens = malloc(tam * sizeof(char*));
 * 	chavesRec(rootPalavra, tokens, modo);
 * 
 * 	qsort(tokens, cnt, sizeof(tokens[0]), comparaString);
 * 	for (i = 0; i < cnt; i++)
 * 		printf("%s\n", tokens[i]);	
 * 	printf("\n");
 * 	free(tokens);
 * }
 */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  chaveLemST
 *  Description:  Devolve lista de lemas ou lemas+derivacoes para o cliente.
                  modo 0: lemas.
				  modo 1: lemas+derivacoes.
 * =====================================================================================
 */
/* void chaveLemST(int modo)
 * {
 * 	char **tokens;
 * 	int i, tam = rootLemma->filhos;
 * 
 * 	cnt = 0;
 * 	tokens = malloc(tam * sizeof(char*));
 * 	chavesRec(rootLemma, tokens, 0);
 * 	qsort(tokens, tam, sizeof(tokens[0]), comparaString);
 * 
 * 	if (modo == 1)
 * 	{
 * 		for (i = 0; i < tam; i++)
 * 		{
 * 			printf("%s ", tokens[i]);
 * 			printValorLem(searchRec(rootLemma, tokens[i]), 3);
 * 		}
 * 	}
 * 	else
 * 	{
 * 		for (i = 0; i < tam; i++)
 * 			printf("%s\n", tokens[i]);	
 * 	}
 * 	printf("\n");
 * 	free(tokens);
 * }
 */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  contaValDistST
 *  Description:  Devolve uma lista de valores distintos para tokens, palavras e lemas.
                  modo 0: tokens distintos.
				  modo 1: palavras distintas.
				  modo 2: lemas distintos.
 * =====================================================================================
 */
/* int contaValDistST(int modo)
 * {
 * 	char **temp = malloc(rootPalavra->filhos * sizeof(char*));
 * 	switch(modo)
 * 	{
 * 		case 0:
 * 			return rootPalavra->filhos;
 * 		case 1:
 * 			cnt = 0;
 * 			chavesRec(rootPalavra, temp, 1);
 * 			return cnt;
 * 		case 2:
 * 			return rootLemma->filhos;
 * 
 * 	}
 * 	return 0;
 * }
 */

