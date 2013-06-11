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
#include "LLRBT.h"
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
	/* arquivo -> descritor de filename (STREAM)                       */
	/*                                                                 */
	/*=================================================================*/
	char *token;
	char filename[32];
	char primPalavra[256];
	char id[256];
	char copia[256];
	char frase[BUFFER];
	char linha[BUFFER];
	char temp[4][256];
	int i, j, k, Nsnt, Ntkn = 0, Npal = 0;
	FILE *arquivo;

	/*==== Testando parêmtros de entrada ===*/
	if (argc <= 1)
	{
		printf(">>Utilize: <%s> -f<nome_do_arquivo> para executar o programa\n\n", argv[0]);
		exit (EXIT_FAILURE);
	}
	else
		sscanf(argv[1], "%*[-f]%s", filename); 

	/*==== Checando integridade do arquivo ===*/
	arquivo = fopen(filename, "r");
	if (arquivo == NULL)
	{
		fprintf(stderr, ">>ERRO: nao consegui abrir o arquivo %s\n", filename);
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "\n>>Abrindo o arquivo < %s >...\n\n", filename);
	
	/*==== Adquirindo o numero total de sentencas ===*/
	while (fgets(linha, BUFFER, arquivo) != NULL)
	{
		sscanf(linha, "%s #%d", primPalavra, &i);
		if (strcmp(primPalavra, "Sentence") == 0)
			Nsnt = i;
	}
	fclose(arquivo);
	/* inicializando o vetor de sentencas */
	initSentence(Nsnt);
	
	/*==== Adquirindo informacoes para a tabela de simbolos ===*/
	arquivo = fopen(filename, "r");
	while (fgets(linha, BUFFER, arquivo))
	{
		strcpy(primPalavra, " ");

		/* extraindo informacoes relativas a sentencas */
		sscanf(linha, "%s #%d (%d", primPalavra, &i, &j);
		if (strcmp(primPalavra, "Sentence") == 0)
		{
			Ntkn += j;
			strcpy(id, linha); /* salvando o id da sentenca */
			strcpy(frase, " ");
			fgets(linha, BUFFER, arquivo);
			sscanf(linha, "%s", copia);	

			/* salvando a frase da sentenca */
			while (strcmp(copia, "[Text=") != 0)
			{
				strcat(frase, linha);
				strcat(frase, " ");
				fgets(linha, BUFFER, arquivo);
				sscanf(linha, "%s", copia);
				copia[6] = '\0';
			}
			frase[strlen(frase)-1] = '\0';

			/* inserindo a sentenca completa no vetor 
			   sendo linha -> sentenca anotada */
			insertSentence(i-1, id, frase, linha);
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
					case 2: /* caractere de posicao inicial */
						strcpy(temp[1], primPalavra); 
						break;
					case 3: /* caractere de posicao final */
						strcpy(temp[2], primPalavra); 
						break;
					case 0: /* lema */
						strcpy(temp[3], primPalavra);
						break;
					default: 
						break;
				}

				/* salvando as informacoes nas tabelas de palavra e lema */
				if (k % 5 == 0) 
				{
					strcpy(copia, " ");
					strcpy(copia, temp[0]);
					insertPalST(lowerCase(copia), buildVal(temp[0], temp[3], 
								atoi(temp[1]), atoi(temp[2]), i-1));
					insertLemST(temp[3], buildVal(temp[0], temp[3], atoi(temp[1]), 
								atoi(temp[2]), i-1));
				}
				token = strtok(NULL, " ");
			}

		}
	}
	fclose(arquivo);
	k = 1; /* para garantir... */

	/*==== MENU ===*/
	while (k != 0)
	{
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
		sscanf(linha, "%s %s", id, primPalavra);
	
		/* faz o hash do id devolvendo um composto de numeros primos (id unico) para o switch */
		if (strcmp(primPalavra, " ") != 0)
		{
			switch(hash(id))
			{
				case 6: /*opcao -e*/
					strcpy(copia, primPalavra);
					printValorPal(searchPalST(lowerCase(copia)), primPalavra, 0);
					break;
				case 10: /*opcao -a*/
					strcpy(copia, primPalavra);
					printValorLem(searchLemST(lowerCase(copia)), 0);
					break;
				case 42: /*opcao -ev*/
					strcpy(copia, primPalavra);
					printValorPal(searchPalST(lowerCase(copia)), primPalavra, 1);
					break;
				case 66: /*opcao -eV*/
					strcpy(copia, primPalavra);
					printValorPal(searchPalST(lowerCase(copia)), primPalavra, 2);
					break;
				case 70: /*opcao -av*/
					strcpy(copia, primPalavra);
					printValorLem(searchLemST(lowerCase(copia)), 1);
					break;
				case 110: /*opcao -aV*/
					strcpy(copia, primPalavra);
					printValorLem(searchLemST(lowerCase(copia)), 2);
					break;
				default:
					printf("Desculpe, nao entendi...\n");
					break;			
			}
		}
		/* hash das opcoes que nao dependem de palavra */
		else if (strcmp(primPalavra, " ") == 0)
		{
			switch(hash(id))
			{
				case 26: /*sair do programa*/
					k = 0;
					break;
				case 34: /*opcao -t*/
					chavePalST(0);
					break;
				case 38: /*opcao -d*/
					chavePalST(1);
					break;
				case 46: /*opcao -l*/
					chaveLemST(0);
					break;
				case 58: /*opcao -L*/
					chaveLemST(1);
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
						   Nsnt, Ntkn, Npal,
						   contaValDistST(0),
						   contaValDistST(1),
						   contaValDistST(2));
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
	printf("\n>>Saindo...\n\n");
	return 0;
}
