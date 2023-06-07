#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Estrutura para representar uma aresta do grafo
typedef struct Aresta {
    int indice;                  // Vértice de destino da aresta
    struct Aresta* proxima;      // Próxima aresta na lista encadeada
} Aresta;

// Estrutura para representar um filme
typedef struct {
    int id;                      // Identificador do filme
    char nome[BUFSIZ];              // Nome do filme
    int posicao;                 // Posição do filme no vetor
    Aresta* arestas;             // Lista de arestas (conexões) do filme
} Filme;

// Estrutura para representar a lista de filmes
typedef struct {
    Filme* filmes;               // Vetor de filmes
    int tamanho;                 // Tamanho atual do vetor
    int capacidade;              // Capacidade total do vetor
} ListaFilmes;

// Estrutura para representar o grafo
typedef struct {
    ListaFilmes listaFilmes;     // Lista de filmes do grafo
} Grafo;

// Função para inicializar a lista de filmes
void inicializarListaFilmes(ListaFilmes* listaFilmes) {
    listaFilmes->filmes = NULL;
    listaFilmes->tamanho = 0;    // Variável de controle do tamanho atual do vetor
    listaFilmes->capacidade = 0; // Variável de controle da capacidade atual do vetor, para verificar se necessita aumentar
}

// Função para adicionar um filme à lista de filmes
void adicionarFilme(ListaFilmes* listaFilmes, int id, const char* nome) {
    // Verifica se é necessário redimensionar o vetor de filmes
    if (listaFilmes->tamanho >= listaFilmes->capacidade) {
        int novaCapacidade = (listaFilmes->capacidade == 0) ? 1 : listaFilmes->capacidade * 2; // Caso tenha estourado o vetor, irá dobrar o tamanho do vetor
        Filme* novosFilmes = (Filme*)realloc(listaFilmes->filmes, novaCapacidade * sizeof(Filme)); // Dobra a capacidade do vetor
        if (novosFilmes == NULL) {
            printf("Erro ao alocar memória.\n");
            return;
        }
        listaFilmes->filmes = novosFilmes;       // Atualiza a lista de filmes com a capacidade atualizada
        listaFilmes->capacidade = novaCapacidade; // Atualiza a variável de controle de capacidade do vetor
    }

    // Adiciona o novo filme ao final da lista
    Filme* novoFilme = &(listaFilmes->filmes[listaFilmes->tamanho]);
    novoFilme->id = id;                      // Adiciona o ID do filme no vetor de filmes
    strcpy(novoFilme->nome, nome);           // Adiciona o nome do filme no vetor de filmes
    novoFilme->posicao = listaFilmes->tamanho; // Adiciona a posição do filme no vetor de filmes, para facilitar a busca
    novoFilme->arestas = NULL;               // Inicializa a aresta de conexão do filme no vetor de filmes

    listaFilmes->tamanho++;                  // Atualiza a variável de controle de tamanho do vetor
}

// Função para buscar um filme na lista de filmes pelo ID
Filme* buscarFilme(NodeFilms* rootFilms, ListaFilmes* listaFilmes, int id) {
    // Busca o filme na arvore de filmes
    NodeFilms* node = searchNodeFilms(rootFilms, id);
    if (node == NULL) {
        return NULL; // Caso o filme não seja encontrado, retorna NULL
    } 

    // Verifica se a posição está dentro do intervalo válido com ajuda da variável de controle do vetor
    if (node->posicao < 1 || node->posicao > listaFilmes->tamanho) {
        return NULL;
    }

    // Obtém o filme diretamente pela posição no vetor, sem ter que percorrer uma lista inteira para encontrar o filme
    Filme* filme = &(listaFilmes->filmes[node->posicao]);
    // printf("          [buscarFilme] listaFilmes->filmes[id]: %d  posição: %d\n", filme->id, filme->posicao);
    if (filme->id == id) {
        return filme; // Retorna as informações do filme
        // printf(" TESTE ENCONTROU FILME\n");
    }

    // printf(" TESTE NAO ENCONTROU FILME\n");
    return NULL; // Caso não encontre o filme, retorna NULL
}

// Função para inserir uma nova aresta na lista de arestas de um filme
void inserirAresta(Filme* filme, int indice) {
    // Cria uma nova aresta
    Aresta* novaAresta = (Aresta*)malloc(sizeof(Aresta));
    novaAresta->indice = indice;                // Irá salvar a posição do filme no vetor de filmes
    novaAresta->proxima = filme->arestas;       // Armazena a lista de arestas atual no próxima da aresta inserida
    filme->arestas = novaAresta;                // Define a nova aresta como a primeira da lista de arestas do filme
}

// Função para buscar filmes na lista e conectá-los com outros filmes do grafo
void buscarConectarFilmes(Grafo* grafo, NodeFilms* rootFilms , ListaFilmes* listaFilmes, int* idsFilmes, int tamanhoLista, FILE* file) {
    // Percorre os IDs dos filmes a serem buscados
    for (int i = 0; i < tamanhoLista; i++) {
        // Busca o filme na lista de filmes
        Filme* filme = buscarFilme(rootFilms, listaFilmes, idsFilmes[i]);
        if (filme != NULL) {
            // printf("\n[buscarConectarFilmes] ID: %d Filme: %s Posição: %d\n", filme->id, filme->nome, filme->posicao);
            // Conecta o filme com os demais filmes da lista
            for (int j = 0; j < tamanhoLista; j++) { // Percorre os IDs dos filmes a serem conectados com o filme buscado da vez
                if (i != j) { // Desconsidera o filme que está sendo verificado para não conectá-lo consigo mesmo
                    // Busca o filme relacionado na lista de filmes
                    Filme* filmeRelacionado = buscarFilme(rootFilms, listaFilmes, idsFilmes[j]);
                    if (filmeRelacionado != NULL) { // Caso tenha encontrado o filme na lista de filmes
                        // Insere uma aresta entre os filmes
                        // printf("      CONEXAO [buscarConectarFilmes] ID: %d Filme: %s Posição: %d\n", filmeRelacionado->id, filmeRelacionado->nome, filmeRelacionado->posicao);
                        inserirAresta(filme, filmeRelacionado->posicao);
                    }
                }
            }
        } else{
            fprintf(file, "Filme %d\n", idsFilmes[i]); // Se não encontrou plota no arquivo de filmes não encontrados
        }
    }
}

// Função que irá realizar a visita de todos os atores inseridos na arvore e realizar as conexões dos filmes no grafo
void visitActorsTree(Node* node, NodeFilms* nodeFilms, Grafo* grafo, FILE* file) {
    if (node != NULL && grafo != NULL) {
        // Visita o nó da subárvore esquerda
        visitActorsTree(node->left, nodeFilms, grafo, file);

        // Obtém o ator e seus filmes do nó atual
        int* filmes = node->movies;
        int numFilmes = node->numMovies;

        // Se o ator realizou mais que um filme realiza as conexões necessárias
        if (numFilmes > 1) {
            // Chama a função buscarConectarFilmes para conectar os filmes do ator
            buscarConectarFilmes(grafo, nodeFilms, &(grafo->listaFilmes), filmes, numFilmes, file);
        }
        // Visita o nó da subárvore direita
        visitActorsTree(node->right, nodeFilms, grafo, file);
    }
}
// Função para imprimir o grafo em formato DOT (Graphviz)
void imprimirGrafoDot(Grafo* grafo) {
    ListaFilmes* listaFilmes = &(grafo->listaFilmes);
    FILE *arquivo = fopen("grafo.dot", "w");

    fprintf(arquivo, "graph {\n");
    for (int i = 0; i < listaFilmes->tamanho; i++) {
        Filme* filme = &(listaFilmes->filmes[i]);

        Aresta* aresta = filme->arestas;
        while (aresta != NULL) {
            Filme* filmeDestino = &(listaFilmes->filmes[aresta->indice]);
            fprintf(arquivo, "\"%s\" -- \"%s\"\n", filme->nome, filmeDestino->nome);
            aresta = aresta->proxima;
        }
    }
    fprintf(arquivo, "}");

    fclose(arquivo);
}

// Função para imprimir o grafo
void imprimirGrafoOutput(Grafo* grafo) {
    FILE *arquivo = fopen("outputGrafo.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return; // ou tratar o erro adequadamente
    }
    ListaFilmes* listaFilmes = &(grafo->listaFilmes);
    fprintf(arquivo, "-------- GRAFO --------------\n");
    for (int i = 0; i < listaFilmes->tamanho; i++) {
        Filme* filme = &(listaFilmes->filmes[i]);
        fprintf(arquivo, "ID: %d, Nome: %s\n", filme->id, filme->nome);

        // Percorre a lista de arestas do filme e imprime os filmes conectados
        Aresta* aresta = filme->arestas;
        while (aresta != NULL) {
            Filme* filmeDestino = &(listaFilmes->filmes[aresta->indice]);
            fprintf(arquivo, "    Conectado com: %s\n", filmeDestino->nome);
            aresta = aresta->proxima;
        }
    }
    fprintf(arquivo, "-------- FIM GRAFO --------------");
    fclose(arquivo);
}

// Função responsável para percorrer o arquivo TSV e armazenar os filmes
void readTSVFileFilmes(const char* filename, Grafo* grafo, NodeFilms** root, int numLines) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char line[BUFSIZ];

    // Ignorar a primeira linha (cabeçalho)
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Erro ao ler o arquivo.\n");
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL ) { //&& numLines > 0
        char* token = strtok(line, "\t");
        int column = 0;
        int id;
        int posicaoFilme;
        char movieName[BUFSIZ];
        int validaRegistro = 1;

        // Percorre as 3 primeiras colunas do dataset
        while (token != NULL && column < 3) {
            if (column == 0) {
                id = atoi(token + 2);// Ignorar os primeiros 2 caracteres 'nm'
                // printf(" ID: %d\n", id);
            } else if (column == 1) {
                // Valida apenas dados que obtenham a segunda coluna com o valor 'movie'
                if (token != NULL && strcmp(token, "movie") != 0) {
                    validaRegistro = 0; // Invalida o registro
                }
                // printf(" titleType: %s\n", token);
            } else if (column == 2) {
                strcpy(movieName, token);
                // printf(" Movie: %s\n\n", movieName);
            }
            token = strtok(NULL, "\t");
            column++;
        }

        // Insere o filme no grafo, apenas se for um registro valido
        if (validaRegistro == 1){
            adicionarFilme(&(grafo->listaFilmes), id, movieName);
            posicaoFilme = grafo->listaFilmes.tamanho - 1; 
            *root = insertNodeFilms(*root, id, movieName, posicaoFilme);
            // printf("Tamanho lista filme: %d\n", grafo->listaFilmes.tamanho);
        }

        // numLines--;
    }
    // printf("Ultimo Filme: %s\n", grafo->listaFilmes.filmes[grafo->listaFilmes.tamanho-1].nome);
    // printf("----------- FIM LEITURA DO ARQUIVO ---------\n");
    fclose(file);
}
