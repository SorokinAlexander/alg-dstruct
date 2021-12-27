#pragma once
#include "stdio.h"
#include "stdlib.h"

typedef struct tree {
    int data;
    struct tree* left;
    struct tree* right;
} tree_t;

tree_t* TreeCreate(int data);
void FreeTree(tree_t* tree);
tree_t* TreeInsert(tree_t* tree, int data);
void PrintTree(tree_t* tree, int n);
void PrintTreeNodes(tree_t* tree);

