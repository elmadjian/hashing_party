/*=================================================*/
/*|||||||||||||||| MAC 323 - EP4 ||||||||||||||||||*/
/*||||| Nome: Carlos Eduardo Leao Elmadjian |||||||*/
/*||||| NUSP: 5685741 |||||||||||||||||||||||||||||*/
/*||||| Arquivo: t2lp.c |||||||||||||||||||||||||||*/
/*=================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "interface.h"

/*========================= Definicao de tipos e structs =============================*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*Node: define um no' (ou entrada) da tabela de simbolos. Guarda uma 
  chave do tipo Valor (uma fila circular sem cabeca de objetos identicos)*/
typedef struct node
{
	Valor *valor;
} Node;


/*========================= Variaveis privadas =======================================*/
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
static Node *t2lp_head;    /*inicio da tabela*/
static int M;			   /*numero de entradas disponiveis na tabela*/
static int N;              /*numero de entradas preenchidas na tabela*/
static int L = 0;          /*numero de lemas distintos*/




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
 *         Name:  remapeiaTabela
 *  Description:  Funcao que cuida do remapeamento dos dados quando N se aproxima
                  de um valor critico. Para isso, a funcao dobra (aproximadamente) o
				  tamanho da tabela atual e faz um novo hash dos dados.
 * =====================================================================================
 */

static void remapeiaTabela()
{
	int i, j = 0, k;
	Node *novo;
	Valor **tabela = malloc(N * sizeof(Valor*));	
	static int pos = 0;
	int primo[21] = {389, 769, 1543, 3079, 6151, 12289, 24593, 49157,
		98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 
		12582917, 25165843, 50331653, 100663319, 201326611, 402653189};


	/*copia valores da tabela atual*/
	for (i = 0; i < M; i++)
	{
		if(t2lp_head[i].valor != NULL)
			tabela[j++] = t2lp_head[i].valor;
	}
	
	/*redefine M e aloca espaco para uma nova tabela, inicializando-a */
	if (M < primo[pos])
		M = primo[pos++];
	novo = malloc(M * sizeof(Node));
	for (i = 0; i < M; i++)
		novo[i].valor = NULL;

	/*remapeia os valores antigos*/
	for (i = 0; i < j; i++)
	{	
		k = hash(tabela[i]->palavra, M);
		while (novo[k].valor != NULL)
			k++;
		novo[k].valor = insertVal(novo[k].valor, tabela[i]);
	}
	t2lp_head = novo;
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
	Node p;
	/*condicao para remapear os dados*/
	if (N == M/2)
		remapeiaTabela();
	i = hash(val->lema, M);
	p = t2lp_head[i];

	/*verifica se ha recorrencia de um valor e insere-o*/
	while (p.valor != NULL)
	{
		if (strcmp(p.valor->lema, val->lema) == 0)
		{
			p.valor = insertVal(p.valor, val);
			break;
		}
		p = t2lp_head[++i];
	}
	/*caso nao haja "match", insira no proximo node livre*/
	if (p.valor == NULL)
	{	
		t2lp_head[i].valor = insertVal(t2lp_head[i].valor, val);
		if (isalpha(val->lema[0]))
			L++;
	}
	N++;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  searchRec
 *  Description:  Funcao de busca recursiva que verifica se uma chave se encontra
                  numa lista de nodes.
 * =====================================================================================
 */
static Valor* searchRec(Node *x, int pos, char *chave)
{
	if (x[pos].valor == NULL) 
		return NULL;
	if (strcmp(x[pos].valor->lema, chave) == 0)
		return x[pos].valor;
	return searchRec(x, pos+1, chave);	
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ST_t2_search
 *  Description:  Funcao de busca para o cliente. Faz  varredura por ocorrencia em
                  T1 para em seguida buscar em T2. Realiza o mapeamento da entrada
                  antes de chamar a busca recursiva.
 * =====================================================================================
 */
Valor* ST_t2_search(char *chave)
{
	Valor *temp = ST_t1_search(chave);
	if (temp != NULL)
		return searchRec(t2lp_head, hash(temp->lema, M), temp->lema);
	return searchRec(t2lp_head, hash(chave, M), chave);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ST_t2_init
 *  Description:  inicializa a tabela de simbolos T2
 * =====================================================================================
 */
void ST_t2_init()
{
	int i;
	N = 0;
	M = 193; /*193 = primo mais proximo de 2^7*/
	t2lp_head = malloc(M * sizeof(Node));
	for (i = 0; i < M; i++)
		t2lp_head[i].valor = NULL;

}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  list
 *  Description:  Devolve o numero de tokens ou palavras encontradas T2, salvando
                  as ocorrencias detectadas.
                  modo 0: tokens.
				  modo 1: palavras.
 * =====================================================================================
 */
static int list(Node *x, char **tokens)
{
	int i, cnt = 0;
	for (i = 0; i < M; i++)
	{
		if (x[i].valor != NULL && isalpha(x[i].valor->lema[0]))
			tokens[cnt++] = x[i].valor->lema;
	}
	return cnt;
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
	int i, cnt;

	tokens = malloc(N * sizeof(char*));
	cnt = list(t2lp_head, tokens);
	qsort(tokens, cnt, sizeof(tokens[0]), comparaString);

	if (modo == 1)
	{
		for (i = 0; i < cnt; i++)
		{
			printf("%s ", tokens[i]);
			printValor(searchRec(t2lp_head, hash(tokens[i], M), tokens[i]), NULL, 3);
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
 *  Description:  Devolve o numero de lemas distintos de T2.
 * =====================================================================================
 */
int ST_t2_count()
{
	return L;	
}
