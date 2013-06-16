/*=================================================*/
/*|||||||||||||||| MAC 323 - EP4 ||||||||||||||||||*/
/*||||| Nome: Carlos Eduardo Leao Elmadjian |||||||*/
/*||||| NUSP: 5685741 |||||||||||||||||||||||||||||*/
/*||||| Arquivo: t2enc.c ||||||||||||||||||||||||||*/
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
static Node **t2enc_head;  /*inicio da tabela*/
static int M;			   /*numero de entradas disponiveis na tabela*/
static int N;              /*numero de entradas preenchidas na tabela*/
static int L = 0;          /*numero de lemas distintos*/
static int cnt;            /*contador para funcoes recursivas*/



/*========================= Implementacao das funcoes ================================*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  insertVal
 *  Description:  Insere um Valor novo numa fila circular sem cabeca do tipo Valor.
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
		if(t2enc_head[i] != NULL && t2enc_head[i]->valor != NULL)
		{
			t = t2enc_head[i];
			while (t != NULL)
			{
				tabela[j++] = t->valor;
				t = t->prox;
			}
			t2enc_head[i] = NULL;
		}
	}
	
	/*realoca memoria da tabela, reinicializa-a e redefine M*/
	if (M < primo[pos])
		M = primo[pos++];
	t2enc_head = malloc(M * sizeof(Node));

	/*remapeia os valores antigos*/
	for (i = 0; i < j; i++)
	{	
		k = hash(tabela[i]->palavra, M);
		t2enc_head[k] = newNode(tabela[i], t2enc_head[k]);
	}
}




/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ST_t2_insert
 *  Description:  Insere um valor na tabela de simbolos t1 para o cliente. Utiliza
                  metodo de hashing por encadeamento e tem comportamento dinamico.
 * =====================================================================================
 */
void ST_t2_insert(Valor *val)
{
	int i;
	Node *p;
	/*condicao para remapear os dados*/
	if (N > M-1)
	{
		/*reinsert*/
		printf("Remapeando a tabela de simbolos...\n");
		remapeiaTabela();
	}
	i = hash(val->lema, M);
	p = t2enc_head[i];

	/*verifica se ha recorrencia de um valor e insere-o*/
	while (p != NULL && p->valor != NULL)
	{
		if (strcmp(p->valor->lema, val->lema) == 0)
		{
			p->valor = insertVal(p->valor, val);
			break;
		}
		p = p->prox;
	}
	/*caso nao haja "match", insira no proximo node livre*/
	if (p == NULL) 
	{
		t2enc_head[i] = newNode(val, t2enc_head[i]);
		if (isalpha(val->lema[0]))
			L++;
	}
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
	if (strcmp(x->valor->lema, chave) == 0)
		return x->valor;
	return searchRec(x->prox, chave);	
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ST_t2_search
 *  Description:  
 * =====================================================================================
 */
Valor* ST_t2_search(char *chave)
{
	Valor *temp = ST_t1_search(chave);
	if (temp == NULL)
		return NULL;
	return searchRec(t2enc_head[hash(temp->lema, M)], temp->lema);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ST_t2_init
 *  Description:  
 * =====================================================================================
 */
void ST_t2_init()
{
	N = 0;
	M = 193; /*primo mais proximo de 2^7*/
	t2enc_head = malloc(M * sizeof(Node));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  listRec
 *  Description:  Devolve todos os tokens ou palavras encontradas na LLRBT.
                  modo 0: tokens.
				  modo 1: palavras.
 * =====================================================================================
 */
static void listRec(Node *x, char **tokens)
{
	if (isalpha(x->valor->lema[0]))
		tokens[cnt++] = x->valor->lema;
	if (x->prox != NULL)
		listRec(x->prox, tokens);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ST_t2_list
 *  Description:  Devolve lista de tokens ou palavras para o cliente.
                  modo 0: tokens.
				  modo 1: palavras.
 * =====================================================================================
 */

void ST_t2_list(int modo)
{
	char **tokens;
	int i;

	cnt = 0;
	tokens = malloc(N * sizeof(char*));
	for (i = 0; i < M; i++)
	{
		if(t2enc_head[i] != NULL)
			listRec(t2enc_head[i], tokens);
	}
	printf("cnt: %d\n", cnt);
	qsort(tokens, cnt, sizeof(tokens[0]), comparaString);
	
	if (modo == 1)
	{
		for (i = 0; i < cnt; i++)
		{
			printf("%s ", tokens[i]);
			printValor(searchRec(t2enc_head[hash(tokens[i], M)], tokens[i]), NULL, 3);
		}
	}
	else if (modo == 0)
	{
		for (i = 0; i < cnt; i++)
			printf("%s\n", tokens[i]);	
	}
	printf("\n");
	free(tokens);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ST_t2_count
 *  Description:  Devolve o numero de lemas distintos na tabela de simbolos.
 * =====================================================================================
 */
int ST_t2_count()
{
	return L;
}


