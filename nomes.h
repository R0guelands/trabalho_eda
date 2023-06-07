#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char** tokenizeString(const char* str, const char* delimiter) {
    char** tokens = NULL;
    int numTokens = 0;

    char* copy = strdup(str);
    char* token = strtok(copy, delimiter);

    while (token != NULL) {
        tokens = realloc(tokens, (numTokens + 1) * sizeof(char*));
        tokens[numTokens] = strdup(token);
        numTokens++;

        token = strtok(NULL, delimiter);
    }

    tokens = realloc(tokens, (numTokens + 1) * sizeof(char*));
    tokens[numTokens] = NULL;

    free(copy);

    return tokens;
}

int countTokens(const char* str, const char* delimiter) {
    int count = 0;

    char* copy = strdup(str);
    char* token = strtok(copy, delimiter);

    while (token != NULL) {
        count++;
        token = strtok(NULL, delimiter);
    }

    free(copy);

    return count;
}

void readTSVFile(const char* filename, Node** root, int numLines) {
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

    while (fgets(line, sizeof(line), file) != NULL) { //&& numLines > 0
        char* token = strtok(line, "\t");
        int column = 0;
        int key;
        char actorName[BUFSIZ];
        int* moviesArray = NULL;
        int numMovies = 0;

        while (token != NULL && column < 6) {
            if (column == 0) {
                key = atoi(token + 2); // Ignorar os primeiros 2 caracteres 'nm'
            } else if (column == 1) {
                strcpy(actorName, token);
            } else if (column == 5) {
                char** movieTokens = tokenizeString(token, ",");
                numMovies = countTokens(token, ",");
                moviesArray = (int*)malloc(numMovies * sizeof(int));

                for (int i = 0; i < numMovies; i++) {
                    moviesArray[i] = atoi(movieTokens[i] + 2); // Ignorar os primeiros 2 caracteres 'tt'
                }

                // Liberar os tokens intermediários
                for (int i = 0; i < numMovies; i++) {
                    free(movieTokens[i]);
                }
                free(movieTokens);
            }

            token = strtok(NULL, "\t");
            column++;
        }

        *root = insertNode(*root, key, actorName, moviesArray, numMovies);

        free(moviesArray);
        // numLines--;
    }

    fclose(file);
}
