#include "TreeHeader.h"
#include "stdio.h"
#include "stdlib.h"

tree_t* TreeCreate(int data) {
    tree_t* tree = (tree_t*)malloc(sizeof(tree_t));
    tree->data = data;
    tree->left = NULL;
    tree->right = NULL;
    return tree;
}

void FreeTree(tree_t* tree)
{
    if (tree != NULL)
    {
        FreeTree(tree->left);
        FreeTree(tree->right);
    }
    free(tree);
}

tree_t* TreeInsert(tree_t* tree, int data) {
    if (!tree) {
        tree = (tree_t*)malloc(sizeof(tree_t));
        tree->data = data;
        tree->left = NULL;
        tree->right = NULL;
    }

    else {
        if (data < tree->data) {
            tree->left = TreeInsert(tree->left, data);
        }
        if (data >= tree->data) {
            tree->right = TreeInsert(tree->right, data);
        }
    }

    return tree;
}

void PrintTree(tree_t* tree, int n) {
    if (tree != NULL) {
        PrintTree(tree->right, n + 1);
        for (int i = 0; i < n; i++) printf("   ");
        printf("%d\n", tree->data);
        PrintTree(tree->left, n + 1);
    }
}

void PrintTreeNodes(tree_t* tree) {
    if (tree->left != NULL || tree->right != NULL) {
        printf("%d (", tree->data);
        if (tree->left != NULL) PrintTreeNodes(tree->left);
        printf(") (");
        if (tree->right != NULL) PrintTreeNodes(tree->right);
        printf(") ");
    }
    else printf("%d", tree->data);
}