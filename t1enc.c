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
/*Node: define um no' (ou entrada) da tabela de simbolos. Guarda uma chave do
  do tipo Valor (uma fila contendo os mesmos objetos) e um ponteiro para
  a chave seguinte */
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
 *  Description:  Insere um Valor novo numa fila circular sem cabeca duplamente
                  encadeada (para poder exibir os resultados em ordem).
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
 *  Description:  Cria um novo no' que recebe um Valor *val e o endereco para
                  o proximo no'.
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
				  tamanho da tabela atual e faz um novo hash dos dados.
 * =====================================================================================
 */

static void remapeiaTabela()
{
	int i, j = 0, k;
	Node *t, **novo;
	Valor **tabela = malloc(N * sizeof(Valor*));	
	static int pos = 0;
	int primo[21] = {389, 769, 1543, 3079, 6151, 12289, 24593, 49157,
		98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 
		12582917, 25165843, 50331653, 100663319, 201326611, 402653189};
	
	/*copia endereco dos valores da tabela atual*/
	for (i = 0; i < M; i++)
	{
		if(t1enc_head[i] != NULL)
		{
			t = t1enc_head[i];
			while (t != NULL)
			{
				tabela[j++] = t->valor;
				t = t->prox;
			}
		}
	}
	
	/*redefine M e aloca espaco para uma nova tabela*/
	if (M < primo[pos])
		M = primo[pos++];
	novo = malloc(M * sizeof(Node));

	/*remapeia os valores antigos*/
	for (i = 0; i < j; i++)
	{	
		k = hash(tabela[i]->palavra, M);
		novo[k] = newNode(tabela[i], novo[k]);
	}
	t1enc_head = novo;
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ST_t1_insert
 *  Description:  Insere um valor na tabela de simbolos T1 para o cliente. Utiliza
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
 *  Description:  Funcao de busca recursiva que verifica se uma chave se encontra
                  numa lista de nodes.
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
 *  Description:  Funcao de busca para o cliente. Faz o mapeamento da entrada
                  antes de chamar a busca recursiva.
 * =====================================================================================
 */
Valor* ST_t1_search(char *chave)
{
	return searchRec(t1enc_head[hash(chave, M)], chave);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ST_t1_init
 *  Description:  inicializa a tabela de simbolos T1
 * =====================================================================================
 */
void ST_t1_init()
{
	N = 0;
	M = 193; /*193 = primo mais proximo de 2^7*/
	t1enc_head = malloc(M * sizeof(Node));
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  listRec
 *  Description:  Salva o endereco de todos os tokens ou palavras encontradas na T1.
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
