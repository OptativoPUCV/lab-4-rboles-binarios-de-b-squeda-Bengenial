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
    if (tree->root == NULL){ //si el arbol esta vacio 
        tree->root = createTreeNode(key, value);
        tree->current = tree->root;
        return;
    }

    //buscar la posicion
    
    if (searchTreeMap(tree, key) != NULL) return; //key repetida

    TreeNode *nuevo = createTreeNode(key,value);
    TreeNode *padre = tree->current;

    nuevo->parent = padre;

    if(tree->lower_than(key, padre->pair->key)){
        padre->left = nuevo;
    }
    else{
        padre->right = nuevo;
    }
    
    tree->current = nuevo;
}   

TreeNode * minimum(TreeNode * x){
    TreeNode *nodo = x;
    if(nodo->left == NULL) return x;

    return minimum(nodo->left);

}


void removeNode(TreeMap * tree, TreeNode* node) {

    //sin hijos
    if (node->left == NULL && node->right == NULL){
        if (node->parent == NULL){
            node = NULL; // raiz
        }
        if (node->parent->left == node){
            node->left = NULL;
        }
        else{
            node->right = NULL;
        }
        free(node);
        return;
    }

    //con un hijo
    if(node->left == NULL || node->right == NULL){
        TreeNode *hijo;
        if (node->left != NULL) hijo = node->left;
        else hijo = node->right;

        if (node->parent == NULL){
            tree->root = hijo;
        }
        if(node->parent->left == node){
            node->parent->left = hijo;
        }
        else{
            node->parent->right = hijo;
        }
        hijo->parent = node->parent;
        free(node);
        return;
    }

    //con dos hijos

    TreeNode *minimo = minimum(node->right);

    void* auxKey = minimo->pair->key;
    void* auxValue = minimo->pair->value;

    removeNode(tree, minimo);

    node->pair->key = auxKey;
    node->pair->value = auxValue;

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    
    TreeNode *nodo = tree->root;
    TreeNode *ultimo = NULL; //posible padre
    //recorrer el arbol
    while (nodo != NULL){
        ultimo = nodo;
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
    tree->current = ultimo;
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode *nodo = tree->root;
    TreeNode *upBNodo = NULL;

    while (nodo != NULL){
        if (is_equal(tree, key, nodo->pair->key)){
            tree->current = nodo;
            return nodo->pair;
        }
        if (tree->lower_than(key,nodo->pair->key)){ //similar a la busqueda, solo almacena el nodo anterior upBNodo
            upBNodo = nodo;
            nodo = nodo->left;
        }
        else{
            nodo = nodo->right;
        }
    }

    if (upBNodo != NULL){
        tree->current = upBNodo;
        return upBNodo->pair;
    }

    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode *nodo = tree->root;

    while(nodo->left != NULL){
        nodo = nodo->left;
    }

    tree->current = nodo;
    return nodo->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode *nodo = tree->current;

    //si tiene un hijo derecho
    if(nodo->right != NULL){
        nodo = minimum(nodo->right);
        tree->current = nodo;
        return nodo->pair;
    }

    //busca el padre

    TreeNode *padre = nodo->parent;
    while(padre != NULL && nodo == padre->right){
        nodo = padre;
        padre = padre->parent;
    }

    if(padre != NULL){
        tree->current = padre;
        return padre->pair;
    }

    return NULL;
}
