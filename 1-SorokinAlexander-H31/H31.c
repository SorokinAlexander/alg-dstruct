#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Treap {
    int data;
    int priority;
    struct Treap* Left;
    struct Treap* Right;
} Treap;

typedef Treap treap_rand;


treap_rand* MergeTreaps(treap_rand* left, treap_rand* right) {
    if (!left)
        return right;
    if (!right)
        return left;
    if (left->priority > right->priority) {
        left->Right = MergeTreaps(left->Right, right);
        return left;
    }
    else {
        right->Left = MergeTreaps(left, right->Left);
        return right;
    }
}

void SplitTreap(treap_rand* treap, int data, treap_rand** left, treap_rand** right) {
    if (!treap) {
        *left = NULL;
        *right = NULL;
        return;
    }
    treap_rand* tmp = NULL;
    if (treap->data <= data) {
        SplitTreap(treap->Right, data, &tmp, right);
        treap->Right = tmp;
        *left = treap;
    }
    else {
        SplitTreap(treap->Left, data, left, &tmp);
        treap->Left = tmp;
        *right = treap;
    }
}

treap_rand* Insert(treap_rand* treap, int data) {
    treap_rand* left = NULL, * right = NULL;
    if (treap) {
        SplitTreap(treap, data, &left, &right);
    }
    Treap* node = malloc(sizeof(Treap));
    if (node) {
        node->data = data;
        node->priority = rand();
        node->Left = NULL;
        node->Right = NULL;
    };
    return MergeTreaps(MergeTreaps(left, node), right);
}
void DeleteTreap(treap_rand* treap) {
    if (treap) {
        DeleteTreap(treap->Left);
        DeleteTreap(treap->Right);
        free(treap);
    }
}
treap_rand* RemoveData(treap_rand* treap, int data) {
    treap_rand* left, * middle, * right;
    SplitTreap(treap, data - 1, &left, &right);
    SplitTreap(right, data, &middle, &treap);
    DeleteTreap(middle);
    return MergeTreaps(left, treap);
}



Treap* SearchData(treap_rand* treap, int data) {
    treap_rand* res = NULL;
    while (treap) {
        if (treap->data == data) {
            res = treap;
            break;
        }
        if (treap->data > data) {
            treap = treap->Left;
        }
        else {
            treap = treap->Right;
        }
    }
    return res;
}

int main() {
    treap_rand* treap = NULL;
    char command;
    int data;
    while (scanf("%c %d\n", &command, &data) > 0) {
        switch (command) {
        case 'a': {
            treap = Insert(treap, data);
            break;
        }
        case 'r': {
            treap = RemoveData(treap, data);
            break;
        }
        case 'f': {
            if (SearchData(treap, data)) {
                printf("%s\n", "yes"); break;
            }
            else { printf("%s\n", "no"); break; }
        }
        }
    }
}