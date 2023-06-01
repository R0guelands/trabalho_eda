#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "rubro.h"
#include "nomes.h"
#include "grafo.h"

int main() {
    Node* root = NULL;
    int numLinesToRead = 3; 

    readTSVFile("data.tsv", &root, numLinesToRead);

    FILE* outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Erro ao abrir o arquivo de saída\n");
        return 1;
    }

    printTreeToFile(root, outputFile);

    fclose(outputFile);

    freeTree(root);

    return 0;
}




