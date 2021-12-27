#include <stdio.h>
#include <stdlib.h>
#include "TreeHeader.h"

int main() {
    tree_t* tree = TreeCreate(10);
    TreeInsert(tree, 15);
    TreeInsert(tree, 1);
    TreeInsert(tree, 4);
    TreeInsert(tree, -1);
    TreeInsert(tree, 12);
    PrintTree(tree, 0);
    PrintTreeNodes(tree);
}