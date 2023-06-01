#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Estrutura de um nó
typedef struct Node {
    int key;
    char actorName[50];
    int* movies;
    int numMovies;
    bool isRed;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
} Node;

// Função auxiliar para criar um novo nó
Node* createNode(int key, const char* actorName) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    strcpy(newNode->actorName, actorName);
    newNode->movies = NULL;
    newNode->numMovies = 0;
    newNode->isRed = true;
    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Função auxiliar para liberar a memória alocada para os nós
void freeNode(Node* node) {
    if (node != NULL) {
        free(node->movies);
        free(node);
    }
}

// Função auxiliar para liberar a memória alocada para a árvore
void freeTree(Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        freeNode(root);
    }
}

// Função auxiliar para imprimir a árvore usando um percurso em ordem
void printTree(Node* root) {
    if (root != NULL) {
        printf("[");
        printTree(root->left);
        printf(" Key: %d, Actor: %s ", root->key, root->actorName);
        printTree(root->right);
        printf("]");
    }
}

void printTreeToFile(Node* root, FILE* file) {
    if (root != NULL) {
        fprintf(file, "[");
        printTreeToFile(root->left, file);
        fprintf(file, " Key: %d, Actor: %s ", root->key, root->actorName);
        printTreeToFile(root->right, file);
        fprintf(file, "]");
    }
}

// Função auxiliar para rotacionar um nó para a esquerda
Node* rotateLeft(Node* root, Node* node) {
    Node* rightChild = node->right;
    node->right = rightChild->left;
    if (rightChild->left != NULL) {
        rightChild->left->parent = node;
    }
    rightChild->parent = node->parent;
    if (node->parent == NULL) {
        root = rightChild;
    } else if (node == node->parent->left) {
        node->parent->left = rightChild;
    } else {
        node->parent->right = rightChild;
    }
    rightChild->left = node;
    node->parent = rightChild;
    return root;
}

// Função auxiliar para rotacionar um nó para a direita
Node* rotateRight(Node* root, Node* node) {
    Node* leftChild = node->left;
    node->left = leftChild->right;
    if (leftChild->right != NULL) {
        leftChild->right->parent = node;
    }
    leftChild->parent = node->parent;
    if (node->parent == NULL) {
        root = leftChild;
    } else if (node == node->parent->left) {
        node->parent->left = leftChild;
    } else {
        node->parent->right = leftChild;
    }
    leftChild->right = node;
    node->parent = leftChild;
    return root;
}

// Função auxiliar para realizar a troca de cor dos nós
void swapColors(Node* node1, Node* node2) {
    bool tempColor = node1->isRed;
    node1->isRed = node2->isRed;
    node2->isRed = tempColor;
}

// Função auxiliar para realizar o balanceamento da árvore após a inserção
Node* fixInsertion(Node* root, Node* node) {
    while (node != root && node->parent->isRed) {
        if (node->parent == node->parent->parent->left) {
            Node* uncle = node->parent->parent->right;
            if (uncle != NULL && uncle->isRed) {
                node->parent->isRed = false;
                uncle->isRed = false;
                node->parent->parent->isRed = true;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    root = rotateLeft(root, node);
                }
                node->parent->isRed = false;
                node->parent->parent->isRed = true;
                root = rotateRight(root, node->parent->parent);
            }
        } else {
            Node* uncle = node->parent->parent->left;
            if (uncle != NULL && uncle->isRed) {
                node->parent->isRed = false;
                uncle->isRed = false;
                node->parent->parent->isRed = true;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    root = rotateRight(root, node);
                }
                node->parent->isRed = false;
                node->parent->parent->isRed = true;
                root = rotateLeft(root, node->parent->parent);
            }
        }
    }
    root->isRed = false;
    return root;
}

// Função para inserir um novo nó na árvore
Node* insertNode(Node* root, int key, const char* actorName, const int* movies, int numMovies) {
    Node* newNode = createNode(key, actorName);
    newNode->movies = (int*)malloc(numMovies * sizeof(int));
    memcpy(newNode->movies, movies, numMovies * sizeof(int));
    newNode->numMovies = numMovies;

    Node* parent = NULL;
    Node* current = root;
    while (current != NULL) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    newNode->parent = parent;
    if (parent == NULL) {
        root = newNode;
    } else if (key < parent->key) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    root = fixInsertion(root, newNode);
    return root;
}

// Função auxiliar para encontrar o nó com a chave especificada
Node* searchNode(Node* root, int key) {
    Node* current = root;
    while (current != NULL && current->key != key) {
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return current;
}

// Função auxiliar para encontrar o nó mínimo a partir do nó especificado
Node* findMinimum(Node* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Função auxiliar para realizar o balanceamento da árvore após a exclusão
Node* fixDeletion(Node* root, Node* node) {
    while (node != root && !node->isRed) {
        if (node == node->parent->left) {
            Node* sibling = node->parent->right;
            if (sibling->isRed) {
                sibling->isRed = false;
                node->parent->isRed = true;
                root = rotateLeft(root, node->parent);
                sibling = node->parent->right;
            }
            if ((!sibling->left || !sibling->left->isRed) &&
                (!sibling->right || !sibling->right->isRed)) {
                sibling->isRed = true;
                node = node->parent;
            } else {
                if (!sibling->right || !sibling->right->isRed) {
                    sibling->left->isRed = false;
                    sibling->isRed = true;
                    root = rotateRight(root, sibling);
                    sibling = node->parent->right;
                }
                sibling->isRed = node->parent->isRed;
                node->parent->isRed = false;
                sibling->right->isRed = false;
                root = rotateLeft(root, node->parent);
                node = root;
            }
        } else {
            Node* sibling = node->parent->left;
            if (sibling->isRed) {
                sibling->isRed = false;
                node->parent->isRed = true;
                root = rotateRight(root, node->parent);
                sibling = node->parent->left;
            }
            if ((!sibling->left || !sibling->left->isRed) &&
                (!sibling->right || !sibling->right->isRed)) {
                sibling->isRed = true;
                node = node->parent;
            } else {
                if (!sibling->left || !sibling->left->isRed) {
                    sibling->right->isRed = false;
                    sibling->isRed = true;
                    root = rotateLeft(root, sibling);
                    sibling = node->parent->left;
                }
                sibling->isRed = node->parent->isRed;
                node->parent->isRed = false;
                sibling->left->isRed = false;
                root = rotateRight(root, node->parent);
                node = root;
            }
        }
    }
    node->isRed = false;
    return root;
}

// Função para deletar um nó da árvore
Node* deleteNode(Node* root, int key) {
    Node* node = searchNode(root, key);
    if (node == NULL) {
        return root;
    }

    Node* child = NULL;
    if (node->left == NULL || node->right == NULL) {
        child = node;
    } else {
        child = findMinimum(node->right);
    }

    Node* sibling = NULL;
    if (child->left != NULL) {
        sibling = child->left;
    } else {
        sibling = child->right;
    }

    if (sibling != NULL) {
        sibling->parent = child->parent;
    }

    if (child->parent == NULL) {
        root = sibling;
    } else if (child == child->parent->left) {
        child->parent->left = sibling;
    } else {
        child->parent->right = sibling;
    }

    if (child != node) {
        node->key = child->key;
        strcpy(node->actorName, child->actorName);
        node->movies = child->movies;
        node->numMovies = child->numMovies;
        child->movies = NULL; // Set child's movies to NULL to prevent double freeing
    }

    root = fixDeletion(root, child);

    freeNode(child);
    return root;
}



void printNode(Node* node) {
    printf("Key: %d, Actor: %s\n", node->key, node->actorName);
    printf("Movies: ");
    for (int i = 0; i < node->numMovies; i++) {
        printf("%d ", node->movies[i]);
    }
    printf("\n");
}