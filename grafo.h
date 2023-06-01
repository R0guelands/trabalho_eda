#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Vertice {
    int id;
    char* nomeFilme;
    struct Vertice* prox;
    struct Aresta* aresta;
} Vertice;

typedef struct Aresta {
    struct Vertice* vertice;
    struct Aresta* prox;
} Aresta;

typedef struct Grafo {
    Vertice* vertices;
} Grafo;

Grafo* criarGrafo() {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->vertices = NULL;
    return grafo;
}

Vertice* criarVertice(int id, const char* nomeFilme) {
    Vertice* vertice = (Vertice*)malloc(sizeof(Vertice));
    vertice->id = id;
    vertice->nomeFilme = strdup(nomeFilme);
    vertice->prox = NULL;
    vertice->aresta = NULL;
    return vertice;
}

void inserirVertice(Grafo* grafo, int id, const char* nomeFilme) {
    Vertice* novoVertice = criarVertice(id, nomeFilme);
    if (grafo->vertices == NULL) {
        grafo->vertices = novoVertice;
    } else {
        Vertice* atual = grafo->vertices;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novoVertice;
    }
}

void deletarGrafo(Grafo* grafo) {
    Vertice* vertice = grafo->vertices;
    while (vertice != NULL) {
        Aresta* aresta = vertice->aresta;
        while (aresta != NULL) {
            Aresta* proximaAresta = aresta->prox;
            free(aresta);
            aresta = proximaAresta;
        }
        Vertice* proximoVertice = vertice->prox;
        free(vertice->nomeFilme);
        free(vertice);
        vertice = proximoVertice;
    }
    free(grafo);
}

void conectarVertices(Grafo* grafo, int idVertice1, int idVertice2) {
    Vertice* vertice1 = grafo->vertices;
    Vertice* vertice2 = grafo->vertices;
    while (vertice1 != NULL && vertice1->id != idVertice1) {
        vertice1 = vertice1->prox;
    }
    while (vertice2 != NULL && vertice2->id != idVertice2) {
        vertice2 = vertice2->prox;
    }
    if (vertice1 != NULL && vertice2 != NULL) {
        Aresta* novaAresta1 = (Aresta*)malloc(sizeof(Aresta));
        novaAresta1->vertice = vertice2;
        novaAresta1->prox = vertice1->aresta;
        vertice1->aresta = novaAresta1;

        Aresta* novaAresta2 = (Aresta*)malloc(sizeof(Aresta));
        novaAresta2->vertice = vertice1;
        novaAresta2->prox = vertice2->aresta;
        vertice2->aresta = novaAresta2;
    }
}

void visitarVertice(FILE* arquivoDot, Vertice* vertice) {
    fprintf(arquivoDot, "    %d [label=\"%s\"];\n", vertice->id, vertice->nomeFilme);
    Aresta* aresta = vertice->aresta;
    while (aresta != NULL) {
        fprintf(arquivoDot, "    %d -- %d;\n", vertice->id, aresta->vertice->id);
        aresta = aresta->prox;
    }
}

void visitarGrafo(Grafo** grafos, int numGrafos) {
    FILE* arquivoDot = fopen("grafo.dot", "w");
    if (arquivoDot == NULL) {
        printf("Erro ao criar arquivo DOT\n");
        return;
    }

    fprintf(arquivoDot, "graph G {\n");
    
    for (int i = 0; i < numGrafos; i++) {
        Vertice* vertice = grafos[i]->vertices;
        while (vertice != NULL) {
            visitarVertice(arquivoDot, vertice);
            vertice = vertice->prox;
        }
    }
    
    fprintf(arquivoDot, "}\n");

    fclose(arquivoDot);
}

void deletarGrafos(Grafo** grafos, int numGrafos) {
    for (int i = 0; i < numGrafos; i++) {
        deletarGrafo(grafos[i]);
    }
    free(grafos);
}