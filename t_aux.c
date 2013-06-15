/*=================================================*/
/*|||||||||||||||| MAC 323 - EP4 ||||||||||||||||||*/
/*||||| Nome: Carlos Eduardo Leao Elmadjian |||||||*/
/*||||| NUSP: 5685741 |||||||||||||||||||||||||||||*/
/*||||| Arquivo: t_aux.c ||||||||||||||||||||||||||*/
/*=================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include "interface.h"

/*========= Definicao de tipos e structs ===========*/

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
	char minscula[32];
	for (i = 0; palavra[i] != '\0'; i++)
		minuscula[i] = tolower(palavra[i]);
	return minuscula;
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
void insertSentence(int pos, int *id, int *frase, int *info)
{
	sentences[pos].id[0] = id[0];
	sentences[pos].id[1] = id[1];		
	sentences[pos].frase[0] = frase[0];
	sentences[pos].frase[1] = frase[1];
	sentences[pos].info[0] = info[0];
	sentences[pos].info[1] = info[1];
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
void printSentence(int num, int modo, FILE* arquivo)
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
/* funcao SUPER-HASH */
#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

uint32_t SuperFastHash (const char * data, int len) 
{
	uint32_t hash = len, tmp;
	int rem;

    if (len <= 0 || data == NULL) return 0;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for (;len > 0; len--) 
	{
        hash  += get16bits (data);
        tmp    = (get16bits (data+2) << 11) ^ hash;
        hash   = (hash << 16) ^ tmp;
        data  += 2*sizeof (uint16_t);
        hash  += hash >> 11;
    }

    /* Handle end cases */
    switch (rem) {
        case 3: hash += get16bits (data);
                hash ^= hash << 16;
                hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
                hash += hash >> 11;
                break;
        case 2: hash += get16bits (data);
                hash ^= hash << 11;
                hash += hash >> 17;
                break;
        case 1: hash += (signed char)*data;
                hash ^= hash << 10;
                hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}
