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
} Valor;


/*====== prototipos das funcoes para o cliente ============ */

void ST_t1_init(); /*falta fazer*/
void ST_t2_init(); /*falta fazer*/
int ST_t1_count();
int ST_t2_count(); /*falta fazer*/
void ST_t1_insert(); 
void ST_t2_insert(); /*falta fazer*/
Valor* ST_t1_search();
Valor* ST_t2_search(); /*falta fazer*/



/*manipulacao de valores (palavras, lemas, posicao da sentenca)*/
Valor* buildVal(char *palavra, char *lema, int sent_id);
void printValorPal(Valor *val, char *palavra, int modo);
void printValorLem(Valor *val, int modo);

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
void printSentence(int num, int modo);

/*funcoes de uso geral*/
char* lowerCase(char *palavra);
int hash(char *opcao);

