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
	struct node *prox;
} Node;


/*========================= Variaveis privadas =======================================*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
static Node **t1enc_head;  /*inicio da tabela*/
static int M;			   /*numero de entradas disponiveis na tabela*/
static int N;              /*numero de entradas preenchidas na tabela*/
static int P = 0;          /*numero de palavras inseridas*/
static int cnt;            /*contador para funcoes recursivas*/




/*========================= Implementacao das funcoes ================================*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  insertVal
 *  Description:  Insere um Valor novo numa lista do tipo Valor. Nao obedece ordem
                  de entrada por motivos de performance.
 * =====================================================================================
 */
static Valor* insertVal(Valor *ini, Valor *novo)
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
 *         Name:  newNode
 *  Description:  Cria um novo no que recebe uma palavra "chave", um Valor e uma cor.
 * =====================================================================================
 */
static Node* newNode(Valor *val, Node *proximo)
{
	Node *novo = malloc(sizeof *novo);
	novo->valor = insertVal(novo->valor, val);
	novo->prox = proximo;
	return novo;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  remapeiaTabela
 *  Description:  Funcao que cuida do remapeamento dos dados quando N se aproxima
                  de um valor critico. Para isso, a funcao dobra (aproximadamente) o
				  tamanho da tabela atual e faz um novo hash dos dados ja inseridos.
 * =====================================================================================
 */

static void remapeiaTabela()
{
	int i, j = 0, k;
	Node *t;
	Valor **tabela = malloc(N * sizeof(Valor*));	
	static int pos = 0;
	int primo[16] = {389, 769, 1543, 3079, 6151, 12289, 24593, 49157,
		98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917};
	
	/*copia valores da tabela atual e libera ponteiros*/
	for (i = 0; i < M; i++)
	{
		if(t1enc_head[i] != NULL && t1enc_head[i]->valor != NULL)
		{
			t = t1enc_head[i];
			while (t != NULL)
			{
				tabela[j++] = t->valor;
				t = t->prox;
			}
			t1enc_head[i] = NULL;
		}
	}
	
	/*realoca memoria da tabela, reinicializa-a e redefine M*/
	if (M < primo[pos])
		M = primo[pos++];
	t1enc_head = malloc(M * sizeof(Node));

	/*remapeia os valores antigos*/
	for (i = 0; i < j; i++)
	{	
		k = hash(tabela[i]->palavra, M);
		t1enc_head[k] = newNode(tabela[i], t1enc_head[k]);
	}
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
	if (N > M-1)
		remapeiaTabela();
	i = hash(val->palavra, M);
	p = t1enc_head[i];

	/*verifica se ha recorrencia de um valor e insere-o*/
	while (p != NULL && p->valor != NULL)
	{
		if (strcmp(p->valor->palavra, val->palavra) == 0)
		{
			p->valor = insertVal(p->valor, val);
			break;
		}
		p = p->prox;
	}
	/*caso nao haja "match", insira no proximo node livre*/
	if (p == NULL)
		t1enc_head[i] = newNode(val, t1enc_head[i]);
	if (isalpha(val->palavra[0]))
		P++;
	N++;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  searchRec
 *  Description:  Funcao de busca recursiva na LLRBT.
 * =====================================================================================
 */
static Valor* searchRec(Node *x, char *chave)
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
 *         Name:  listRec
 *  Description:  Devolve todos os tokens ou palavras encontradas na LLRBT.
                  modo 0: tokens.
				  modo 1: palavras.
 * =====================================================================================
 */
static void listRec(Node *x, char **tokens, int modo)
{
	if (modo == 0)
		tokens[cnt++] = x->valor->palavra;
	else if (modo == 1)
	{
		if (isalpha(x->valor->palavra[0]))
			tokens[cnt++] = x->valor->palavra;
	}
	if (x->prox != NULL)
		listRec(x->prox, tokens, modo);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ST_t1_list
 *  Description:  Devolve lista de tokens ou palavras para o cliente.
                  modo 0: tokens.
				  modo 1: palavras.
 * =====================================================================================
 */

void ST_t1_list(int modo) 
{
	char **tokens;
	int i;

	cnt = 0;
	tokens = malloc(N * sizeof(char*));
	for (i = 0; i < M; i++)
	{
		if(t1enc_head[i] != NULL)
			listRec(t1enc_head[i], tokens, modo);
	}
	printf("cnt: %d\n", cnt);
	qsort(tokens, cnt, sizeof(tokens[0]), comparaString);
	for (i = 0; i < cnt; i++)
		printf("%s\n", tokens[i]);	
	printf("\n");
	free(tokens);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ST_t1_count
 *  Description:  Devolve o numero correspondete 'a quantidade de alguns valores da 
                  tabela de simbolos a serem especificados pelo usuario.
				  modo 0: numero de tokens.
				  modo 1: numero de palavras.
				  modo 2: numero de tokens distintos.
				  modo 3: numero de palavras distintas.
 * =====================================================================================
 */
int ST_t1_count(int modo)
{
	int i;
	char **temp = malloc(N * sizeof(char*));
	switch(modo)
	{
		case 0:
			return N;
		case 1:
			return P;
		case 2:
			cnt = 0;
			for (i = 0; i < M; i++)
			{
				if (t1enc_head[i] != NULL)
					listRec(t1enc_head[i], temp, 0);
			}
			return cnt;
		case 3:
			cnt = 0;
			for (i = 0; i < M; i++)
			{
				if (t1enc_head[i] != NULL)
					listRec(t1enc_head[i], temp, 1);
			}
			return cnt;
		default:
			break;
	}
	free (temp);
	return 0;
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

