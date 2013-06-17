/*=================================================*/
/*|||||||||||||||| MAC 323 - EP4 ||||||||||||||||||*/
/*||||| Nome: Carlos Eduardo Leao Elmadjian |||||||*/
/*||||| NUSP: 5685741 |||||||||||||||||||||||||||||*/
/*||||| Arquivo: interface.h ||||||||||||||||||||||*/
/*=================================================*/

/*====== Definicao de tipos e structs para o cliente =======++++++++++++++++++++ */

/*Valor: guarda uma palavra, o lema correspondente, o id da 
  sentenca e ponteiros para o objeto anterior e o proximo de 
  valor equivalente*/
typedef struct valor
{
	char *palavra;
	char *lema;
	int sentenca;
	struct valor *ant;
	struct valor *prox;
} Valor;


/*====== prototipos das funcoes para o cliente ============++++++++++++++++++++ */

/*funcoes de manipulacao de tabelas de simbolos*/
void ST_t1_init(); 
void ST_t2_init();
int ST_t1_count(int modo);
int ST_t2_count();
void ST_t1_insert(); 
void ST_t2_insert();
Valor* ST_t1_search();
Valor* ST_t2_search();
void ST_t1_list(int modo);
void ST_t2_list(int modo);

/*manipulacao de valores (palavras, lemas, posicao da sentenca)*/
Valor* buildVal(char *palavra, char *lema, int sent_id);
void printValor(Valor *val, FILE* arquivo, int modo);

/*manipulacao de sentencas*/
void initSentence(int slots);
void insertSentence(int pos, int *id, int *frase, int *info);

/*funcoes de uso geral*/
int comparaString(const void *stringA, const void *stringB);
char* lowerCase(char *palavra);
int hashOption(char *opcao);
int hash(char *palavra, int tableSize);


/*====== breve descricao das funcoes =======================++++++++++++++++++++ */
/*ST_t1_init        inicializa a tabela T1                                       */
/*ST_t2_init        inicializa a tabela T2                                       */
/*ST_t1_count       conta elementos de T1 (tokens, palavras, elementos distintos)*/
/*ST_t2_count       conta lemas distintos em T2                                  */
/*ST_t1_insert      insere uma palavra do tipo Valor em T1                       */
/*ST_t2_insert      insere um lema do tipo Valor em T2                           */
/*ST_t1_search      busca uma palavra em T1                                      */
/*ST_t2_search      busca um lema em T2                                          */
/*ST_t1_list        lista elementos (tokens, palavras) de T1                     */
/*ST_t2_list        lista lemas (com ou sem derivacoes) de T2                    */

/*buildVal          constroi um elemento do tipo Valor                           */
/*printValor        imprime uma sentenca (completa ou nao) de um Valor val       */

/*initSentence      inicializa um vetor de sentencas                             */
/*insertSentence    insere dados de endereco das sentencas                       */

/*comparaString     funcao comparadora para o qsort                              */
/*lowerCase         transforma uma string em minuscula                           */
/*hashOption        retorna um codigo correspondente a uma opcao                 */
/*hash              mapeia um endereco em funcao de uma string                   */
/*==========================================================++++++++++++++++++++ */
