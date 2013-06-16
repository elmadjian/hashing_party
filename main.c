/*=================================================*/
/*|||||||||||||||| MAC 323 - EP3 ||||||||||||||||||*/
/*||||| Nome: Carlos Eduardo Leao Elmadjian |||||||*/
/*||||| NUSP: 5685741 |||||||||||||||||||||||||||||*/
/*||||| Arquivo: main.c |||||||||||||||||||||||||||*/
/*=================================================*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "interface.h"
#define BUFFER 65536


int main(int argc, char **argv)
{
	/*============= Variaveis =========================================*/
	/*                                                                 */
    /* token -> linha temporaria para ser concatenada a uma frase      */
	/* filname -> guarda o nome do arquivo aberto pelo usuario         */
	/* primPalavra -> utilizada para guardar uma palavra qualquer      */
	/* id -> guarda o identificador da sentenca                        */
	/* copia -> copia temporaria de uma palavra                        */
	/* frase -> armazena apenas a frase de uma sentenca                */
	/* linha -> armazena temporariamente uma linha do STREAM           */
	/* temp -> guarda os dados retirados de um token                   */
	/* i, j, k -> contadores                                           */
	/* Nsnt -> guarda o numero total de sentencas                      */
	/* Ntkn -> guarda o numero total de tokens                         */
	/* Npal -> guarda o numero total de palavras                       */
	/* posicao ->        */
	/* arquivo -> descritor de filename (STREAM)                       */
	/*                                                                 */
	/*=================================================================*/
	char *token;
	char filename[32];
	char opcao[32];
	char primPalavra[256];
	/*char id[256];*/
	char copia[256];
	/*char frase[BUFFER];*/
	char linha[BUFFER];
	char temp[2][256];
	int i, j, k, Nsnt, Ntkn = 0, Npal = 0;
	int id[2];
	int frase[2];
	int info[2];
	FILE *arquivo;

	/*==== Testando parametros de entrada ===*/
	if (argc <= 1)
	{
		printf(">> Utilize: <%s> -f<nome_do_arquivo> para executar o programa\n\n", argv[0]);
		exit (EXIT_FAILURE);
	}
	else
		sscanf(argv[1], "%*[-f]%s", filename); 

	/*==== Checando integridade do arquivo ===*/
	arquivo = fopen(filename, "rb");
	if (arquivo == NULL)
	{
		fprintf(stderr, ">> ERRO: nao consegui abrir o arquivo %s\n", filename);
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "\n>> Abrindo o arquivo < %s >...\n\n", filename);
	
	/*==== Adquirindo o numero total de sentencas ===*/
	while (fgets(linha, BUFFER, arquivo) != NULL)
	{
		sscanf(linha, "%s #%d", primPalavra, &i);
		if (strcmp(primPalavra, "Sentence") == 0)
			Nsnt = i;
	}
	/* inicializando o vetor de sentencas e as STs */
	initSentence(Nsnt);
	ST_t1_init();
	ST_t2_init();
	
	/*==== Adquirindo informacoes para a tabela de simbolos ===*/
	rewind(arquivo);
	/*arquivo = fopen(filename, "r");*/
	while (fgets(linha, BUFFER, arquivo) != NULL)
	{
		/*limpando primPalavra*/
		strcpy(primPalavra, " ");

		/* extraindo informacoes relativas a sentencas */
		sscanf(linha, "%s #%d (%d", primPalavra, &i, &j);
		if (strcmp(primPalavra, "Sentence") == 0)
		{
			Ntkn += j; /*atualiza numero de tokens*/
			
			/* encontrando o id da sentenca */
			id[1] = ftell(arquivo);
			id[0] = id[1] - strlen(linha);
	
			/* encontrando a frase da sentenca */
			fgets(linha, BUFFER, arquivo);
			sscanf(linha, "%s", copia);
			frase[0] = ftell(arquivo) - strlen(linha);
			while (strcmp(copia, "[Text=") != 0)
			{
				frase[1] = ftell(arquivo);
				fgets(linha, BUFFER, arquivo);
				sscanf(linha, "%s", copia);
				copia[6] = '\0';
			}
			/* encontrando a sentenca anotada */
			info[1] = ftell(arquivo);
			info[0] = info[1] - strlen(linha);

			/* inserindo a sentenca completa no vetor */ 
			insertSentence(i-1, id, frase, info);
		}

		/*==== Construindo chaves da tabela de simbolos ===*/
		if (linha[0] == '[')
		{
			k = 0;
			token = strtok(linha, " ");
			while (token != NULL && strcmp(token, "\n") != 0) 
			{
				k++;
				sscanf(token, "%*[^=]=%[^]]", primPalavra);
				/* o mesmo perfil de informacao se repete em mod 5 */
				switch(k % 5)
				{
					case 1: /* palavra */
						strcpy(temp[0], primPalavra);
						if (isalpha(primPalavra[0]))
							Npal++;
						break;
					case 0: /* lema */
						strcpy(temp[1], primPalavra);
						break;
					default: 
						break;
				}

				/* salvando as informacoes nas tabelas de palavra(t1) e lema(t2) */
				if (k % 5 == 0) 
				{
					ST_t1_insert(buildVal(temp[0], temp[1], i-1));
					ST_t2_insert(buildVal(temp[0], temp[1], i-1));
				}
				token = strtok(NULL, " ");
			}
		}
	}
	k = 1; /* para garantir... */

	/*==== MENU ===*/
	while (k != 0)
	{
		printf("tokens: %d,  palavras: %d\n", Ntkn, Npal);
		strcpy(primPalavra, " ");
		printf("================================\n"
			   "|             MENU             |\n"
			   "| -e: busca exata              |\n"
			   "| -a: busca lema               |\n"
			   "| -{e,a}v: busca exata + id    |\n"
			   "| -{e,a}V: -{e,a}v + info      |\n"
			   "| -t: tokens (ordenados)       |\n"
			   "| -d: palavras (ordenadas)     |\n"
			   "| -l: lemas (ordenados)        |\n"
			   "| -L: -l + derivacoes          |\n"
			   "| -s: estatisticas             |\n"
			   "| -F: sair do programa         |\n"
			   "================================\n"
			   "sua opcao: ");
		/* recebe opcao do usuario */
		fgets(linha, 256, stdin);
		sscanf(linha, "%s %s", opcao, primPalavra);
	
		/* faz o hash do id devolvendo um composto de numeros primos (id unico) para o switch */
		if (strcmp(primPalavra, " ") != 0)
		{
			switch(hashOption(opcao))
			{
				case 6: /*opcao -e*/
					printValor(ST_t1_search(primPalavra), arquivo, 0);
					break;
				case 10: /*opcao -a*/
					printValor(ST_t2_search(primPalavra), arquivo, 0);
					break;
				case 42: /*opcao -ev*/
					printValor(ST_t1_search(primPalavra), arquivo, 1);
					break;
				case 66: /*opcao -eV*/
					printValor(ST_t1_search(primPalavra), arquivo, 2);
					break;
				case 70: /*opcao -av*/
					printValor(ST_t2_search(primPalavra), arquivo, 1);
					break;
				case 110: /*opcao -aV*/
					printValor(ST_t2_search(primPalavra), arquivo, 2);
					break;
				default:
					printf("Desculpe, nao entendi...\n");
					break;			
			}
		}
		/* hash das opcoes que nao dependem de palavra */
		else if (strcmp(primPalavra, " ") == 0)
		{
			switch(hashOption(opcao))
			{
				case 26: /*sair do programa*/
					k = 0;
					break;
				case 34: /*opcao -t*/
					ST_t1_list(0);
					break;
				case 38: /*opcao -d*/
					ST_t1_list(1);
					break;
				case 46: /*opcao -l*/
					ST_t2_list(0);
					break;
				case 58: /*opcao -L*/
					ST_t2_list(1);
					break;
				case 62: /*opcao -s*/
					printf("-----------------------------------\n"
						   "|          Estatisticas:          |\n"
						   "-----------------------------------\n"
						   "-> Numero de sentencas: %d\n"
						   "-> Numero de tokens: %d\n"
						   "-> Numero de palavras: %d\n"
						   "-> Total de tokens distintos: %d\n"
						   "-> Total de palavras distintas: %d\n"
						   "-> Total de lemas distintos: %d\n"
						   "-----------------------------------\n\n",
						   Nsnt, 
						   ST_t1_count(0), 
						   ST_t1_count(1),
						   ST_t1_count(2),
						   ST_t1_count(3),
						   ST_t2_count());
					break;
				default:
					printf("Desculpe, nao entendi...\n");
					break;
			}
		}	
		else
			printf("Desculpe, nao entendi...\n");
	}
	/*Saindo... do programa*/
	fclose(arquivo);
	printf("\n>> Saindo...\n\n");
	return 0;
}
