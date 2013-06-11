/*=================================================*/
/*|||||||||||||||| MAC 323 - EP3 ||||||||||||||||||*/
/*||||| Nome: Carlos Eduardo Leao Elmadjian |||||||*/
/*||||| NUSP: 5685741 |||||||||||||||||||||||||||||*/
/*||||| Arquivo: LLRBT.h ||||||||||||||||||||||||||*/
/*=================================================*/

/*====== Definicao de tipos e structs para o cliente =======*/

/*Valor: guarda uma palavra, o lema correspondente, a posicao
  inicial no texto, a posicao final, o id da sentenca e um
  ponteiro para uma proxima palavra de valor equivalente*/
typedef struct valor
{
	char *palavra;
	char *lemma;
	int loc_ini;
	int loc_fim;
	int sentenca;
	struct valor *prox;
} Valor;


/*====== prototipos das funcoes para o cliente ============ */

/*manipulacao de valores (palavras, lemas, posicao das palavras)*/
Valor* buildVal(char *palavra, char *lemma, int ini, int fim, int sent_id);
void printValorPal(Valor *val, char *palavra, int modo);
void printValorLem(Valor *val, int modo);

/*manipulacao de tabelas de simbolos*/
void insertPalST(char *chave, Valor *val);
void insertLemST(char *chave, Valor *val);
Valor* searchPalST(char *chave);
Valor* searchLemST(char *chave);
void chavePalST(int modo);
void chaveLemST(int modo);
int contaValDistST(int modo);

/*manipulacao de sentencas*/
void initSentence(int slots);
void insertSentence(int pos, char *id, char *frase, char *info);
void printSentence(int num, int modo);

/*funcoes de uso geral*/
char* lowerCase(char *palavra);
int hash(char *opcao);

