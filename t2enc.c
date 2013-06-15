/*=================================================*/
/*|||||||||||||||| MAC 323 - EP4 ||||||||||||||||||*/
/*||||| Nome: Carlos Eduardo Leao Elmadjian |||||||*/
/*||||| NUSP: 5685741 |||||||||||||||||||||||||||||*/
/*||||| Arquivo: t2enc.c ||||||||||||||||||||||||||*/
/*=================================================*/

/*========= OBS. ==================================*/
/* Os algoritmos de LLRBT foram quase integralmente
 baseados no livro "Algorithms", de SEDGEWICK, R. & WAYNE, K.,
 e posteriormente adaptados para a linguagem C, 
 ressalvando, e' claro, as particularidades deste EP3 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "LLRBT.h"
#define	RED 1	/*cor vermelha do node*/
#define	BLACK 0	/*cor preta do node*/


/*========= Definicao de tipos e structs ===========*/

/*Sentenca: guarda o id, a frase e a sentenca anotada*/
typedef struct sentenca
{
	char *id;
	char *frase;
	char *info;
} Sentenca;

/*Node: define um no da arvore LLRBT. Guarda uma chave, um
  Valor, ponteiros para os filhos esquerdo e direito, numero
  de nos herdeiros e cor do no (vermelha ou preta)*/
typedef struct node
{
	char *chave;
	Valor *val;
	struct node *esquerda;
	struct node *direita;
	int filhos;
	int cor;
} Node;


/*========= Variaveis privadas ======================*/
static Node *rootPalavra;  /*raiz da arvore de palavras*/
static Node *rootLemma;    /*raiz da arvore de lemas*/
static Sentenca *sentences;/*vetor de sentencas*/
static int cnt;            /*contador para recursao*/


/*========= Implementacao das funcoes ===============*/
/*===================================================*/
/*SECAO 1: funcoes de uso geral                      */
/*SECAO 2: funcoes de manipulacao de Valor           */
/*SECAO 3: funcoes de manipulacao de Sentenca        */
/*SECAO 4: funcoes de manipulacao de LLRBT           */
/*===================================================*/


/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||| SECAO 1 ||||||||||||||||||*/
/*------------------------------------------------------------------------------------*/

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  comparaString
 *  Description:  Funcao de comparacao para o qsort -- *Nao* diferencia maiuscula de
                  minuscula: portanto, obedece ao alfabeto, nao ao codigo ANSI.
 * =====================================================================================
 */
int comparaString(const void *stringA, const void *stringB)
{
	char *A = malloc((strlen(*(char**)stringA)+1) * sizeof(char*));
	char *B = malloc((strlen(*(char**)stringB)+1) * sizeof(char*));
	strcpy(A, *(char**)stringA);
	strcpy(B, *(char**)stringB);
	return strcmp(lowerCase(A), lowerCase(B));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  lowerCase
 *  Description:  Devolve uma string para sua forma minuscula.
 * =====================================================================================
 */
char* lowerCase(char *palavra)
{
	int i;
	for (i = 0; palavra[i] != '\0'; i++)
		palavra[i] = tolower(palavra[i]);
	return palavra;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  hash
 *  Description:  Devolve um codigo (unico - utiliza numeros primos) correspondente 
                  a uma string. Caso algum caractere nao pertenca 'a tabela, devolve
				  zero. 
 * =====================================================================================
 */

int hash(char *opcao)
{
	int i = 0, codigo = 1;
	char c = opcao[i];
	while (c != '\0')
	{
		switch(c)
		{
			case '-': codigo *= 2; break;
			case 'e': codigo *= 3; break;
			case 'a': codigo *= 5; break;
			case 'v': codigo *= 7; break;
			case 'V': codigo *= 11; break;
			case 'F': codigo *= 13;	break;
			case 't': codigo *= 17;	break;
			case 'd': codigo *= 19; break;
			case 'l': codigo *= 23; break;
			case 'L': codigo *= 29; break;
			case 's': codigo *= 31; break;
			default: codigo = 0; break;
		}
		c = opcao[++i];		
	}
	return codigo;
}



/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||| SECAO 2 ||||||||||||||||||*/
/*------------------------------------------------------------------------------------*/

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
	if (ini == NULL)
		return ini = novo;
	p = ini->prox;
	novo->prox = p;
	ini->prox = novo;
	return ini;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  buildVal
 *  Description:  Cria um novo Valor, que contem uma palavra, um lema correspondente,
                  indicadores de posicao inicial e final no texto, e id da sentenca.
 * =====================================================================================
 */
Valor* buildVal(char *palavra, char *lemma, int ini, int fim, int sent_id)
{
	char *pal = malloc((strlen(palavra)+1) * sizeof(char));
	char *lem = malloc((strlen(lemma)+1) * sizeof(char));
	Valor *val = malloc(sizeof *val);
	strcpy(pal, palavra);
	strcpy(lem, lemma);
	val->palavra = pal;
	val->lemma = lem;
	val->loc_ini = ini;
	val->loc_fim = fim;
	val->sentenca = sent_id;
	val->prox = NULL;
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



/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||| SECAO 3 ||||||||||||||||||*/
/*------------------------------------------------------------------------------------*/

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  initSentence
 *  Description:  Inicializa o vetor de sentencas.
 * =====================================================================================
 */
void initSentence(int slots)
{
	sentences = malloc(slots * sizeof(Sentenca));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  insertSentence
 *  Description:  Insere uma sentenca completa (identificador + frase + sentenca anotada)
                  na posicao "pos" do vetor de sentencas.
 * =====================================================================================
 */
void insertSentence(int pos, char *id, char *frase, char *info)
{
	sentences[pos].id = malloc((strlen(id)+1) * sizeof(char));
	sentences[pos].frase = malloc((strlen(frase)+1) * sizeof(char));
	sentences[pos].info = malloc((strlen(info)+1) * sizeof(char));
	strcpy(sentences[pos].id, id);
	strcpy(sentences[pos].frase, frase);
	strcpy(sentences[pos].info, info);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  printSentence
 *  Description:  Recebe um id (chave) da sentenca desejada e imprime o resultado
                  de acordo com o modo escolhido.
				  modo 0: apenas a frase.
				  modo 1: frase + identificador da sentenca.
				  modo 2: frase + identificador da sentenca + sentenca anotada.
 * =====================================================================================
 */
void printSentence(int num, int modo)
{
	switch (modo)
	{
		case 0:	
			printf("---->%s", sentences[num].frase);
			break;
		case 1:
			printf("----> %s |-->%s", sentences[num].id, sentences[num].frase);
			break;
		case 2:
			printf("----> %s |-->%s |--> %s", sentences[num].id, sentences[num].frase, 
				sentences[num].info);
			break;
		default:
			break;	
	}
}



/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||| SECAO 4 ||||||||||||||||||*/
/*------------------------------------------------------------------------------------*/
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  isRed
 *  Description:  Verifica se um no e' vermelho. Caso nao exista, devolve preto.
 * =====================================================================================
 */
int isRed(Node *x)
{
	if (x == NULL)
		return BLACK;
	return x->cor == RED;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  size
 *  Description:  Devolve o numero de filhos de um no (contando com ele mesmo).
 * =====================================================================================
 */
int size(Node *x)
{
	if (x == NULL)
		return 0;
	else
		return x->filhos;
	
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rotateLeft
 *  Description:  Rotaciona a LLRBT para a esquerda.
 * =====================================================================================
 */
Node* rotateLeft(Node *raiz)
{
	Node *temp = raiz->direita;
	raiz->direita = temp->esquerda;
	temp->esquerda = raiz;
	temp->cor = raiz->cor;
	raiz->cor = RED;
	temp->filhos = raiz->filhos;
	raiz->filhos = 1 + size(raiz->esquerda) + size(raiz->direita);

	return temp;	
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  rotateRight
 *  Description:  Rotaciona a LLRBT para a direita.
 * =====================================================================================
 */
Node* rotateRight(Node *raiz)
{
	Node *temp = raiz->esquerda;
	raiz->esquerda = temp->direita;
	temp->direita = raiz;
	temp->cor = raiz->cor;
	raiz->cor = RED;
	temp->filhos = raiz->filhos;
	raiz->filhos = 1 + size(raiz->esquerda) + size(raiz->direita);

	return temp;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  colorFlip
 *  Description:  Transforma o no em vermelho e seus filhos em preto.
 * =====================================================================================
 */

void colorFlip(Node *raiz)
{
	raiz->cor = RED;
	raiz->esquerda->cor = BLACK;
	raiz->direita->cor = BLACK;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  newNode
 *  Description:  Cria um novo no que recebe uma palavra "chave", um Valor e uma cor.
 * =====================================================================================
 */
Node* newNode(char *chave, Valor *val, int cor)
{
	char *key = malloc((strlen(chave)+1) * sizeof(char));
	Node *novo = malloc(sizeof *novo);
	strcpy(key, chave);
	novo->chave = key;
	novo->val = val;
	novo->cor = cor;
	novo->filhos = 1;
	novo->esquerda = novo->direita = NULL;
	return novo;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  insertRec
 *  Description:  Funcao de insercao recursiva em LLRBT.
 * =====================================================================================
 */

Node* insertRec(Node *raiz, char *chave, Valor *val)
{
	/* base da recursao: no vazio */
	if (raiz == NULL) 
		return raiz = newNode(chave, val, RED);
	
	/* compara a palavra de entrada com as chaves */
	if (strcmp(raiz->chave, chave) < 0)
		raiz->esquerda = insertRec(raiz->esquerda, chave, val);
	else if (strcmp(raiz->chave, chave) > 0)
		raiz->direita = insertRec(raiz->direita, chave, val);
	else
		raiz->val = insertVal(raiz->val, val);

	/* faz o balanceamento */
	if (isRed(raiz->direita) && !isRed(raiz->esquerda))
		raiz = rotateLeft(raiz);
	if (isRed(raiz->esquerda) && isRed(raiz->esquerda->esquerda))
		raiz = rotateRight(raiz);
	if (isRed(raiz->esquerda) && isRed(raiz->direita))
		colorFlip(raiz);

	/* contagem dos filhos */
	raiz->filhos = 1 + size(raiz->esquerda) + size(raiz->direita);

	return raiz;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  insertPalST
 *  Description:  Insercao de palavras (apenas) para o cliente.
 * =====================================================================================
 */

void insertPalST(char *chave, Valor *val)
{
	char *pal = malloc((strlen(chave)+1) * sizeof(char));
	strcpy(pal, chave);
	rootPalavra = insertRec(rootPalavra, pal, val);
	rootPalavra->cor = BLACK;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  insertLemST
 *  Description:  Insercao de lemas (apenas) para o cliente.
 * =====================================================================================
 */
void insertLemST(char *chave, Valor *val)
{
	rootLemma = insertRec(rootLemma, chave, val);
	rootLemma->cor = BLACK;
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
	if (strcmp(x->chave, chave) < 0)
		return searchRec(x->esquerda, chave);
	else if (strcmp(x->chave, chave) > 0)
		return searchRec(x->direita, chave);
	else
		return x->val;	
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  searchPalST
 *  Description:  Busca de palavras (apenas) para o cliente.
 * =====================================================================================
 */
Valor* searchPalST(char *chave)
{
	return searchRec(rootPalavra, chave);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  searchLemST
 *  Description:  Busca de lemas (apenas) para o cliente.
 * =====================================================================================
 */
Valor* searchLemST(char *chave)
{
	Valor *temp = searchPalST(chave);
	if (temp == NULL)
		return NULL;
	return searchRec(rootLemma, temp->lemma);
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

