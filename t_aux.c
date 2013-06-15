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
#define BUFFER 65536





/*Sentenca: guarda o id, a frase e a sentenca anotada (info)*/
typedef struct sentenca
{
	int id[2];
	int frase[2];
	int info[2];
} Sentenca;

/*========= Variaveis privadas ======================*/
static Sentenca *sentences; /*guarda posicoes do id, frase e sent. anotada no STREAM*/ 

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

int hashOption(char *opcao)
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
	val->prox = val->ant = val;
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
void printValorPal(Valor *val, FILE* arquivo, int modo)
{
	Valor *t = val;
	int frase_id = -1;

	/*caso a chave nao exista*/
	if (t == NULL)
		printf("Nenhum resultado encontrado\n");
	/*verificando as ocorrencias na fila e imprimindo*/
	do
	{
		if (t->sentenca != frase_id)
		{
			printSentence(t->sentenca, modo, arquivo);
			printf("\n");
		}
		frase_id = t->sentenca;
		t = t->prox;
	} while (t != val);
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
void printValorLem(Valor *val, FILE* arquivo, int modo)
{
	char **pal;
	int i = 0, j, k, frase_id = -1;
	Valor *t = val;

	/*caso a chave nao exista*/
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
		/*criando vetor de candidatos para comparacao*/
		pal = malloc(i * sizeof(char*));
		j = 0;
		printf(" -->");
		/*procurando derivacoes do lema*/
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
				printSentence(t->sentenca, modo, arquivo);
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
	char resultado[BUFFER];

	switch (modo)
	{
		case 0:
			fseek(arquivo, sentences[num].frase[0], SEEK_SET);
			printf("---->");
			while(ftell(arquivo) < sentences[num].frase[1])
			{
				fgets(resultado, BUFFER, arquivo);
				printf("%s", resultado);
			}
			break;
		case 1:
			fseek(arquivo, sentences[num].id[0], SEEK_SET);
			fgets(resultado, BUFFER, arquivo);
			printf("---->%s", resultado);
			fseek(arquivo, sentences[num].frase[0], SEEK_SET);	
			printf("  |-->");
			while(ftell(arquivo) < sentences[num].frase[1])
			{
				fgets(resultado, BUFFER, arquivo);
				printf("%s", resultado);
			}
			break;
		case 2:
			fseek(arquivo, sentences[num].id[0], SEEK_SET);
			fgets(resultado, BUFFER, arquivo);
			printf("---->%s", resultado);
			fseek(arquivo, sentences[num].frase[0], SEEK_SET);	
			printf("  |-->");
			while(ftell(arquivo) < sentences[num].frase[1])
			{
				fgets(resultado, BUFFER, arquivo);
				printf("%s", resultado);
			}
			fseek(arquivo, sentences[num].info[0], SEEK_SET);
			fgets(resultado, BUFFER, arquivo);
			printf("  |--> %s", resultado);
		default:
			break;	
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  hash
 *  Description:  
 * =====================================================================================
 */

int hash(char *palavra, int tableSize)
{   
	int h = 0, a = 127;
	while (*palavra != '\0')
	{
		h = (a * h + *palavra)%tableSize;
		palavra++;
	}
	return h;
}
