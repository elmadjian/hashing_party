/*=================================================*/
/*|||||||||||||||| MAC 323 - EP4 ||||||||||||||||||*/
/*||||| Nome: Carlos Eduardo Leao Elmadjian |||||||*/
/*||||| NUSP: 5685741 |||||||||||||||||||||||||||||*/
/*||||| Arquivo: interface.h ||||||||||||||||||||||*/
/*=================================================*/

/*====== Definicao de tipos e structs para o cliente =======*/

/*Valor: guarda uma palavra, o lema correspondente, a posicao
  inicial no texto, a posicao final, o id da sentenca e um
  ponteiro para uma proxima palavra de valor equivalente*/
typedef struct valor
{
	char *palavra;
	char *lema;
	int sentenca;
	struct valor *ant;
	struct valor *prox;
} Valor;


/*====== prototipos das funcoes para o cliente ============ */

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
/*void printValorLem(Node *val, int modo);*/

/*manipulacao de tabelas de simbolos*/
/*void insertPalST(char *chave, Valor *val);
void insertLemST(char *chave, Valor *val);
Valor* searchPalST(char *chave);
Valor* searchLemST(char *chave);
void chavePalST(int modo);
void chaveLemST(int modo);
int contaValDistST(int modo);*/

/*manipulacao de sentencas*/
void initSentence(int slots);
void insertSentence(int pos, int *id, int *frase, int *info);
void printSentence(int num, int modo, FILE* arquivo);

/*funcoes de uso geral*/
int comparaString(const void *stringA, const void *stringB);
char* lowerCase(char *palavra);
int hashOption(char *opcao);
int hash(char *palavra, int tableSize);
