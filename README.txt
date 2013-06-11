/*=================================================*/
/*|||||||||||||||| MAC 323 - EP3 ||||||||||||||||||*/
/*||||| Nome: Carlos Eduardo Leao Elmadjian |||||||*/
/*||||| NUSP: 5685741 |||||||||||||||||||||||||||||*/
/*||||| Arquivo: README.txt |||||||||||||||||||||||*/
/*=================================================*/

1) Este EP3 e' composto pelos seguintes arquivos:
   - README.txt (este arquivo);
   - main.c (codigo-fonte do programa principal);
   - LLRBT.c (codigo-fonte do modulo de Red-Black tree e funcoes relacionada);
   - LLRBT.h (interface do modulo para o cliente);
   - makefile (arquivo de conf. de compilacao);
   - script.sh (script para converter um texto pelo coreNLP);
   - sample.txt.out (arquivo de texto para teste).

2) Para compilar e rodar o programa ep3, digite no shell:
   > make && ./ep3 -f<nome_do_arquivo>

3) Estruturas de dados:
   - Arvores rubro-negras esquerdistas:
     + Para este EP3 foi utilizada como tabela de simbolos as ARNEs ou LLRBT (ingles).
     + As LLRBT comportam-se como arvores 2-3 (ou seja, estao sempre balanceadas), com
       a vantagem de podermos executar tarefas rotineiras de ST, e.g. buscas, como 
       se fossem arvores binarias, uma vez que cada no' da' origem a apenas dois descendentes,
       um menor e um maior.
     + Para manter uma estrutura hibrida entre arvores 2-3 e arvores binarias, as LLRBT
       utilizam cores para diferenciar os nos, de modo que o balanceamento e' garantido
       mantendo-se a mesma distancia negra para a raiz (tanto por meio de rotacoes
       quanto por mudanca de cores).
     + struct utilizada por Node (LLRBT) neste EP:

       struct node
       {
          char *chave;           -> identificador do item ou chave de um no' da arvore
          Valor *val;            -> guarda uma lista de valores correspondente 'a chave
          struct node *esquerda; -> descendente menor 
          struct node *direita;  -> descendente maior
          int filhos;            -> numero de descendentes do no' atual
          int cor;               -> cor do no'
	} Node;		

    - Listas ligadas:
      + Este EP utiliza uma implementacao de fila com uma estrutura de lista ligada sem cabeca.
      + Nessa fila, armazenamos itens do tipo abstrato Valor, que guarda caracteristicas de
        cada palavra extraida do texto de entrada. O ponteiro para o inicio dessa fila
        fica guardado dentro da LLRBT, de acordo com a chave correspondente. Por exemplo:
        para a chave "casa", podemos encontrar uma fila do tipo Valor em que elementos possiveis
        sao "Casa", "CASA", "casa"...
      + struct utilizada por Valor neste EP:

        struct valor
        {
           char *palavra;        -> armazena uma palavra correspondente a uma chave da arvore
           char *lemma;          -> armazena o lema da palavra
           int loc_ini;          -> indice para a posicao inicial da palavra no texto
           int loc_fim;          -> indice para a posicao final da palavra no texto
           int sentenca;         -> indice para um vetor de sentencas indexado por chaves
           struct valor *prox;   -> proxima palavra que pertence 'a mesma chave
         } Valor;

    - Vetor indexado por chaves:
      + Este EP possui um vetor de sentencas indexado por chaves, em que cada entrada do vetor
        corresponde a um conjunto de informacoes pertencentes a um tipo abstrato Sentenca.
      + A opcao por um vetor indexado por chaves se da em funcao da sua eficiencia, mas ao
        custo de (possivelmente) uma grande complexidade de espaco para alguns casos. Em testes
        preliminares, para processar textos verdadeiramente grandes, o EP3 chegou a consumir 
        dezenas de MB de RAM.
      + O tipo abstrato Sentenca nao apresenta uma chave em si, mas apenas tres campos definidos
        pela struct abaixo:

        struct sentenca
        {
           char *id;             -> identificador da sentenca dado pelo coreNLP
           char *frase;          -> a sentenca em si, uma string que armazena a frase
           char *info;           -> a sentenca anotada dada pelo coreNLP
        } Sentenca;
     

4) ** Observacoes importantes **:

- O caractere "carriage return" (^M):

	O programa coreNLP devolve um arquivo que possui um caractere especial de termino
	de linha (^M). Optei por nao concatenar as frases quebradas, isto e', salvando-as
	como unica sentenca, mas mantendo sua formatacao (nem sempre agradavel) original.

- Case sensitive: maiusculas e minusculas

	Para a arvore que guarda as palavras, considerei que seria mais adequado que as chaves
	contivessem apenas palavras em letras minusculas -- tanto para padronizar o processo
	de comparacao quanto para uniformidade da tabela. Uma vez que o enunciado nao e'
	explicito sobre a questao, essa me pareceu a solucao mais razoavel. Ja para a arvore
	de lemas, por entender que os lemas sao justamente unicos, nao havia essa necessidade
	de uniformizar um formato determinante de chave.

- Sentencas armazenadas separadamente:

	Optei por armazenar informacoes relativas a sentencas em um vetor indexado por chaves,
	uma vez que seria redundante armazenar a mesma sentenca varias vezes para entradas
	e arvores diferentes. Desse modo, com as arvores armazenando apenas um ponteiro para
	as sentencas -- e nao as sentencas em si -- faz-se um uso mais racional de complexidade 
	de espaco sem perda de eficiencia.

- Por que um vetor indexado por chaves e nao uma lista ligada:

	Optei por um vetor indexado por chaves por uma questao de eficiencia em funcao do
	tempo de execucao. E' evidente que para um usuario que deseja realizar no programa uma 
	unica busca, armazenar as sentencas em uma lista ligada parece bastante razoavel, 
	visto que isso dispensaria a necessidade de	saber de antemao quanto espaco teriamos que
   	reservar para um vetor de sentencas.
	Mas essa e' uma presuncao um tanto ousada. Por isso, optei por um pre-processamento das 
	linhas do arquivo para determinar o tamanho final de um vetor de sentencas. O custo
	desse processo e' necessariamente O(n), o que e' equivalente ao pior caso de uma busca
	numa lista ligada. No entanto, a busca pela sentenca se da em O(1) -- sempre --, o que e' 
	muito superior 'a busca numa lista ligada (O(n)), sobretudo em textos grandes.
	Mesmo que o usuario faca uma unica busca, o custo do pre-processamento e' bastante razoavel.
