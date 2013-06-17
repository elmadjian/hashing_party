/*=================================================*/
/*|||||||||||||||| MAC 323 - EP4 ||||||||||||||||||*/
/*||||| Nome: Carlos Eduardo Leao Elmadjian |||||||*/
/*||||| NUSP: 5685741 |||||||||||||||||||||||||||||*/
/*||||| Arquivo: README.txt |||||||||||||||||||||||*/
/*=================================================*/

1) Este EP4 e' composto pelos seguintes arquivos:
   - README.txt (este arquivo);
   - main.c (codigo-fonte do programa principal);
   - t1enc.c (codigo-fonte da tabela T1 (res. de colisoes por encadeamento));
   - 12enc.c (codigo-fonte da tabela T2 (res. de colisoes por encadeamento)); 
   - t1lp.c (codigo-fonte da tabela T1 (res. de colisoes por linear probing));
   - t2lp.c (codigo-fonte da tabela T2 (res. de colisoes por linear probing));
   - interface.h (interface que unifica todo o que esta disponivel para o cliente);
   - makefile (arquivo de conf. de compilacao);
   - script.sh (script para converter um texto pelo coreNLP);
   - sample.txt.out (arquivo de texto para teste).

2) Para compilar e rodar as quatro variantes do programa ep4, digite no shell:
   > make ep4encenc && ./ep4 -f<nome_do_arquivo>
   > make ep4lpenc && ./ep4 -f<nome_do_arquivo>
   > make ep4enclp && ./ep4 -f<nome_do_arquivo>
   > make ep4lplp && ./ep4 -f<nome_do_arquivo>
   ----------------------------------------------------
   Se desejar remover os objetos de compilacao, execute:
   > make clean

3) Estruturas de dados:
   - Tabelas de hashing:
     + Para este EP4 foram utilizadas as chamadas tabelas de hashing como tabelas de simbolos.
	 + O mapeamento feito nos quatro modulos diferentes exigidos pelo enunciado foi feito
       com uma mesma funcao de hashing.
     + As tabelas T1 e T2 que resolvem conflitos por encademento fazem uso de uma lista ligada
       que aglutina as colisoes de mapeamentos coincidentes. Dentro de cada no' dessa lista,
	   ha um ponteiro para uma fila circular sem cabeca, que armazena entradas identicas.
	   Portanto, palavras distintas que colidem pertencem 'a mesma lista ligada de nodes, 
	   mas nao 'a mesma fila de objetos identicos.
     + As tabelas T1 e T2 que resolvem conflitos por linear probing tambem utilizam a
       a estrutura da fila para armazenar objetos redundantes. Contudo, strings distintas
	   mapeadas para o mesmo endereco sao armazenadas na proxima posicao vazia do vetor, isto e',
       realiza-se o probing linear para objetos distintos que colidem.
     + structs utilizadas por Node neste EP:

	   caso a) encadeamento:
       struct node
       {
          Valor *val;            -> guarda uma lista de valores identicos
          struct node *prox;     -> proximo item com colisao na lista
	   } Node;

       caso b) linear probing:
       struct node
       {
          Valor *val;            -> guarda uma lista de valores identicos
	   } Node;

   

    - Filas circulares:
      + Este EP utiliza uma implementacao de fila circular, com uma estrutura de lista ligada 
        sem cabeca.
      + Nessa fila, armazenamos itens do tipo abstrato Valor, que guarda caracteristicas de
        cada palavra extraida do texto de entrada. O ponteiro para o inicio dessa fila
        fica guardado dentro de um Node, de acordo com a chave correspondente. 
      + struct utilizada por Valor neste EP:

        struct valor
        {
           char *palavra;        -> armazena uma palavra correspondente a uma chave da arvore
           char *lemma;          -> armazena o lema da palavra
           int sentenca;         -> indice para um vetor de sentencas indexado por chaves
           struct valor *ant     -> palavra anterior que pertence 'a mesma chave
           struct valor *prox;   -> proxima palavra que pertence 'a mesma chave
         } Valor;

    - Vetor indexado por chaves:
      + Este EP possui um vetor de sentencas indexado por chaves, em que cada entrada do vetor
        corresponde a um conjunto de informacoes pertencentes a um tipo abstrato Sentenca.
      + Cada elemento da struct do tipo Sentenca armazena dois ponteiros: um para o inicio e
        outro para o fim da ocorrencia desejada num STREAM (id, frase ou sentenca anotada).
      + O tipo abstrato Sentenca nao apresenta uma chave em si, mas apenas tres campos definidos
        pela struct abaixo:

        struct sentenca
        {
           int id[2];             -> identificador da sentenca dado pelo coreNLP
           int frase[2];          -> a sentenca em si, uma string que armazena a frase
           int info[2];           -> a sentenca anotada dada pelo coreNLP
        } Sentenca;
     

4) ** Observacoes importantes **:

- Case sensitive: maiusculas e minusculas

    Dado que no forum da disciplina o professor determinou que palavras iguas diferenciadas
    apenas por maiusculas deveriam ser tradadas como casos distintos, descontinuei a abordagem
    feita no EP3, em que nao fazia distincao de palavras maiusculas de minusculas.	
	
- Ponteiros para sentencas:

	Em funcao de o enunciado ter determinado desta vez que um arquivo de texto esperado pode
    ser realmente grande (digamos, nao apenas um livro gigantesco, mas 100 desses), salvar
    o texto correspondente a uma determinada palavra (como foi feito no EP3) se tornou inviavel
    do ponto de vista da complexidade de espaco. Neste caso, optei por armazenar apenas um
    inteiro correspondente 'a posicao no STREAM de um arquivo de texto aberto.
