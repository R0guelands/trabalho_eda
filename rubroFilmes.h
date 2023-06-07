#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Estrutura de um nó para os filmes
typedef struct NodeFilms {
    int key;
    char filmName[BUFSIZ];
    int posicao;
    bool isRed;
    struct NodeFilms* parent;
    struct NodeFilms* left;
    struct NodeFilms* right;
} NodeFilms;

// Função auxiliar para criar um novo nó
NodeFilms* createNodeFilms(int key, const char* filmName, int posicao) {
    NodeFilms* newNodeFilms = (NodeFilms*)malloc(sizeof(NodeFilms));
    newNodeFilms->key = key;
    strcpy(newNodeFilms->filmName, filmName);
    newNodeFilms->posicao = posicao;
    newNodeFilms->isRed = true;
    newNodeFilms->parent = NULL;
    newNodeFilms->left = NULL;
    newNodeFilms->right = NULL;
    return newNodeFilms;
}

// Função auxiliar para liberar a memória alocada para os nós
void freeNodeFilms(NodeFilms* node) {
    if (node != NULL) {
        free(node);
    }
}

// Função auxiliar para liberar a memória alocada para a árvore
void freeTreeFilms(NodeFilms* root) {
    if (root != NULL) {
        freeTreeFilms(root->left);
        freeTreeFilms(root->right);
        freeNodeFilms(root);
    }
}

// Função auxiliar para imprimir a árvore usando um percurso em ordem
void printTreeFilms(NodeFilms* root) {
    if (root != NULL) {
        printf("[");
        printTreeFilms(root->left);
        printf(" Key: %d, Filme: %s Posição: %d", root->key, root->filmName, root->posicao);
        printTreeFilms(root->right);
        printf("]");
    }
}

// Função auxiliar para imprimir a árvore em um arquivo .txt usando um percurso em ordem
void printTreeFilmsToFile(NodeFilms* root, FILE* file) {
    if (root != NULL) {
        fprintf(file, "[");
        printTreeFilmsToFile(root->left, file);
        fprintf(file, " Key: %d, Filme: %s Posição: %d", root->key, root->filmName, root->posicao);
        printTreeFilmsToFile(root->right, file);
        fprintf(file, "]");
    }
}

// Função auxiliar para rotacionar um nó para a esquerda
NodeFilms* rotateLeftFilms(NodeFilms* root, NodeFilms* node) {
    NodeFilms* rightChild = node->right;
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
NodeFilms* rotateRightFilms(NodeFilms* root, NodeFilms* node) {
    NodeFilms* leftChild = node->left;
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
void swapColorsFilms(NodeFilms* node1, NodeFilms* node2) {
    bool tempColor = node1->isRed;
    node1->isRed = node2->isRed;
    node2->isRed = tempColor;
}

// Função auxiliar para realizar o balanceamento da árvore após a inserção
NodeFilms* fixInsertionFilms(NodeFilms* root, NodeFilms* node) {
    while (node != root && node->parent->isRed) {
        if (node->parent == node->parent->parent->left) {
            NodeFilms* uncle = node->parent->parent->right;
            if (uncle != NULL && uncle->isRed) {
                node->parent->isRed = false;
                uncle->isRed = false;
                node->parent->parent->isRed = true;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    root = rotateLeftFilms(root, node);
                }
                node->parent->isRed = false;
                node->parent->parent->isRed = true;
                root = rotateRightFilms(root, node->parent->parent);
            }
        } else {
            NodeFilms* uncle = node->parent->parent->left;
            if (uncle != NULL && uncle->isRed) {
                node->parent->isRed = false;
                uncle->isRed = false;
                node->parent->parent->isRed = true;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    root = rotateRightFilms(root, node);
                }
                node->parent->isRed = false;
                node->parent->parent->isRed = true;
                root = rotateLeftFilms(root, node->parent->parent);
            }
        }
    }
    root->isRed = false;
    return root;
}

// Função para inserir um novo nó na árvore
NodeFilms* insertNodeFilms(NodeFilms* root, int key, const char* filmName, int posicao) {
    NodeFilms* newNodeFilms = createNodeFilms(key, filmName, posicao);

    NodeFilms* parent = NULL;
    NodeFilms* current = root;
    while (current != NULL) {
        parent = current;
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    newNodeFilms->parent = parent;
    if (parent == NULL) {
        root = newNodeFilms;
    } else if (key < parent->key) {
        parent->left = newNodeFilms;
    } else {
        parent->right = newNodeFilms;
    }

    root = fixInsertionFilms(root, newNodeFilms);
    return root;
}

// Função auxiliar para encontrar o nó com a chave especificada
NodeFilms* searchNodeFilms(NodeFilms* root, int key) {
    NodeFilms* current = root;
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
NodeFilms* findMinimumFilms(NodeFilms* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Função auxiliar para realizar o balanceamento da árvore após a exclusão
NodeFilms* fixDeletionFilms(NodeFilms* root, NodeFilms* node) {
    while (node != root && !node->isRed) {
        if (node == node->parent->left) {
            NodeFilms* sibling = node->parent->right;
            if (sibling->isRed) {
                sibling->isRed = false;
                node->parent->isRed = true;
                root = rotateLeftFilms(root, node->parent);
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
                    root = rotateRightFilms(root, sibling);
                    sibling = node->parent->right;
                }
                sibling->isRed = node->parent->isRed;
                node->parent->isRed = false;
                sibling->right->isRed = false;
                root = rotateLeftFilms(root, node->parent);
                node = root;
            }
        } else {
            NodeFilms* sibling = node->parent->left;
            if (sibling->isRed) {
                sibling->isRed = false;
                node->parent->isRed = true;
                root = rotateRightFilms(root, node->parent);
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
                    root = rotateLeftFilms(root, sibling);
                    sibling = node->parent->left;
                }
                sibling->isRed = node->parent->isRed;
                node->parent->isRed = false;
                sibling->left->isRed = false;
                root = rotateRightFilms(root, node->parent);
                node = root;
            }
        }
    }
    node->isRed = false;
    return root;
}

// Função para deletar um nó da árvore
NodeFilms* deleteNodeFilms(NodeFilms* root, int key) {
    NodeFilms* node = searchNodeFilms(root, key);
    if (node == NULL) {
        return root;
    }

    NodeFilms* child = NULL;
    if (node->left == NULL || node->right == NULL) {
        child = node;
    } else {
        child = findMinimumFilms(node->right);
    }

    NodeFilms* sibling = NULL;
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
        strcpy(node->filmName, child->filmName);
        node->posicao = child->posicao;
    }

    root = fixDeletionFilms(root, child);

    freeNodeFilms(child);
    return root;
}

void printNodeFilms(NodeFilms* node) {
    printf(" Key: %d, Filme: %s Posição: %d", node->key, node->filmName, node->posicao);
    printf("\n");
}