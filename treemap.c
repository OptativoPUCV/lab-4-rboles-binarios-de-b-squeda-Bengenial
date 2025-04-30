#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *map = (TreeMap *) malloc(sizeof(TreeMap));
    if (map == NULL) return NULL;

    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;


    return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree->root == NULL) {
        // Si el árbol está vacío, se crea la raíz
        tree->root = createTreeNode(key, value);
        tree->current = tree->root;
        return;
    }

    TreeNode *node = tree->root;
    TreeNode *parent = NULL;

    // Búsqueda para encontrar la posición donde insertar
    while (node != NULL) {
        if (is_equal(tree, key, node->pair->key)) {
            // Si la clave ya existe, no se inserta nada
            return;
        }

        parent = node;

        if (tree->lower_than(key, node->pair->key)) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    // Crear nuevo nodo
    TreeNode *newNode = createTreeNode(key, value);
    newNode->parent = parent;

    // Insertar nuevo nodo en la posición correcta
    if (tree->lower_than(key, parent->pair->key)) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    // Actualizar el current
    tree->current = newNode;
}   

TreeNode * minimum(TreeNode * x){

    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    
    TreeNode *nodo = tree->root;
    //recorrer el arbol
    while (nodo != NULL){
        if(is_equal(tree, key, nodo->pair->key)){
            tree->current = nodo;
            return nodo->pair;
        }
        if (tree->lower_than(key, nodo->pair->key)){
            nodo = nodo->left;
        }
        else{
            nodo = nodo->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
