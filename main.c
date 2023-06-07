#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>       // for clock_t, clock(), CLOCKS_PER_SEC
#include "rubroFilmes.h"
#include "rubro.h"
#include "filmes.h"
#include "nomes.h"

int main() {
    Node* root = NULL;
    int numLinesToRead = 4000;
    double time_spent = 0.0; // Variavel para marcar o tempo de execução do programa
    clock_t begin = clock();

    printf("-----------------------------------------------------------\n");
    printf("Lendo os dados dos atores e Criando Árvore Red Black...\n");
    readTSVFile("data.tsv", &root, numLinesToRead); // Percorre o arquivo dos atores e guarda os atores na arvore

    FILE* outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Erro ao abrir o arquivo de saída\n");
        return 1;
    }

    printTreeToFile(root, outputFile); // Visita a arvore e salva os nós em um arquivo .txt
    fclose(outputFile);
    
    printf("Fim da leitura e criação da Árvore\n");
    printf("-----------------------------------------------------------\n\n");
    
    // ----------------------------------------------------------------------------------

    Grafo grafo;
    NodeFilms* rootFilms = NULL;
    inicializarListaFilmes(&(grafo.listaFilmes)); // Inicializa a lista de filmes
    
    printf("-----------------------------------------------------------\n");
    printf("Inicio da leitura dos Filems e criação do grafo e arvore de filmes...\n");
    readTSVFileFilmes("title.tsv", &grafo, &rootFilms, numLinesToRead); // Percorre o arquivo de filmes e armazena os dados dos filmes
    
    FILE* outputFileFilme = fopen("outputArvoreFilme.txt", "w");
    if (outputFileFilme == NULL) {
        printf("Erro ao abrir o arquivo de saída\n");
        return 1;
    }

    printTreeFilmsToFile(rootFilms, outputFileFilme); // Visita a arvore de filmes e salva os nós em um arquivo .txt
    fclose(outputFileFilme);
    
    printf("Fim da leitura e criação do grafo e arvore de fimes\n");
    printf("-----------------------------------------------------------\n\n");

    printf("-----------------------------------------------------------\n");
    printf("Inicio da visita e relacionamento dos filmes dos atores...\n");

    FILE* output_filmes_nao_encontrados = fopen("filmes_nao_encontrados.txt", "w");
    if (output_filmes_nao_encontrados == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    fprintf(output_filmes_nao_encontrados, "----- FILMES NÃO ENCONTRADOS --------\n");
    visitActorsTree(root, rootFilms, &grafo, output_filmes_nao_encontrados); // Visita a árvore e realiza as conexões dos filmes de cada ator inserido
    fclose(output_filmes_nao_encontrados);
    
    printf("\nFim da visita e relacionamento dos atores\n");
    printf("-----------------------------------------------------------\n\n");

    imprimirGrafoOutput(&grafo);
    imprimirGrafoDot(&grafo);
    
    printf("-----------------------------------------------------------\n");
    printf("Inicio do liberamento da memoria\n");
    freeTree(root);
    freeTreeFilms(rootFilms);
    printf("\nFim do liberamento da memoria\n");
    printf("-----------------------------------------------------------\n\n");

    printf("-----------------------------------------------------------\n\n");
    clock_t end = clock(); // Marca o fim da execução
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Tempo de execuçãp: %f segundos\n", time_spent);
    printf("-----------------------------------------------------------\n");
    printf("Termino da execução\n");
    printf("-----------------------------------------------------------\n\n");
    return 0;
}




