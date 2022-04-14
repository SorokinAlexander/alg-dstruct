#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

int* keys;


Node* create_node(int key, Node* first, Node* second, Node* parent) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->size = 1;
    node->key[0] = key;
    node->first = first;
    node->second = second;
    node->third = NULL;
    node->parent = parent;
    node->fourth = NULL;
    node->brother = NULL;
    return node;
}

int find(Node* node, int k) {
    if (node == NULL)
        return 0;
    for (int i = 0; i < node->size; i++)
        if (node->key[i] == k) return 1;
    return 0;
}

void sort(Node* node) {
    if (node->size == 1) return;
    else if (node->size == 2) {
        if (node->key[0] > node->key[1]) {
            int temp = node->key[0];
            node->key[0] = node->key[1];
            node->key[1] = temp;
        }
    }
    else if (node->size == 3) {
        if (node->key[0] > node->key[1]) {
            int temp = node->key[0];
            node->key[0] = node->key[1];
            node->key[1] = temp;
        }
        if (node->key[0] > node->key[2]) {
            int temp = node->key[0];
            node->key[0] = node->key[2];
            node->key[2] = temp;
        }
        if (node->key[1] > node->key[2]) {
            int temp = node->key[1];
            node->key[1] = node->key[2];
            node->key[2] = temp;
        }
    }
}

void insert_to_node(Node* node, int k) {
    node->key[node->size] = k;
    node->size += 1;
    sort(node);
}

void remove_from_node(Node* node, int k) {
    if (node->size >= 1 && node->key[0] == k) {
        node->key[0] = node->key[1];
        node->key[1] = node->key[2];
        node->size -= 1;
    }
    else if (node->size == 2 && node->key[1] == k) {
        node->key[1] = node->key[2];
        node->size -= 1;
    }
}

void become_node2(Node* node, int k, Node* first, Node* second) {
    node->key[0] = k;
    node->first = first;
    node->second = second;
    node->third = NULL;
    node->parent = NULL;
    node->brother = NULL;
    node->fourth = NULL;
    node->size = 1;
}

int is_leaf(Node* node) {
    return (node->first == NULL && node->second == NULL && node->third == NULL);
}

Node* insert(Node* node, int k) {
    if (node == NULL) return create_node(k, NULL, NULL, NULL);
    if (is_leaf(node)) insert_to_node(node, k);
    else if (k <= node->key[0]) insert(node->first, k);
    else if (node->size == 1 || node->size == 2 && k <= node->key[1]) insert(node->second, k);
    else insert(node->third, k);

    return (Node*)split(node);
}

Node* split(Node* node) {
    if (node->size < 3) return (Node*)node;

    Node* x = create_node(node->key[0], node->first, node->second, node->parent);
    Node* y = create_node(node->key[2], node->third, node->fourth, node->parent);

    x->brother = y;

    if (x->first) x->first->parent = x;
    if (x->second) x->second->parent = x;
    if (y->first) y->first->parent = y;
    if (y->second) y->second->parent = y;

    if (node->parent) {
        insert_to_node(node->parent, node->key[1]);

        if (node->parent->first == node) node->parent->first = NULL;
        else if (node->parent->second == node) node->parent->second = NULL;
        else if (node->parent->third == node) node->parent->third = NULL;

        if (node->parent->first == NULL) {
            node->parent->fourth = node->parent->third;
            node->parent->third = node->parent->second;
            node->parent->second = y;
            node->parent->first = x;
        }
        else if (node->parent->second == NULL) {
            node->parent->fourth = node->parent->third;
            node->parent->third = y;
            node->parent->second = x;
        }
        else {
            node->parent->fourth = y;
            node->parent->third = x;
        }

        return (Node*)(node->parent);
    }
    else {
        x->parent = node;
        y->parent = node;
        become_node2(node, node->key[1], x, y);
        return (Node*)node;
    }
}

Node* search(Node* node, int k) {
    if (!node) return NULL;
    if (find(node, k)) return node;
    else if (k < node->key[0]) return search(node->first, k);
    else if ((node->size == 2) && (k < node->key[1]) || (node->size == 1)) return search(node->second, k);
    else if (node->size == 2) return search(node->third, k);
}

Node* search_min(Node* node) {
    if (!node) return NULL;
    if (!(node->first)) return node;
    else return search_min(node->first);
}

Node* remove_from_tree(Node* node, int k) {
    Node* knode = search(node, k);
    if (knode == NULL) return node;

    Node* min = NULL;
    if (knode->key[0] == k)
        min = search_min(knode->second);
    else
        min = search_min(knode->third);

    if (min != NULL) {
        if (k == knode->key[0]) {
            knode->key[0] = min->key[0];
            min->key[0] = k;
        }
        else {
            knode->key[1] = min->key[0];
            min->key[0] = k;
        }
        knode = min;
    }

    remove_from_node(knode, k);
    return fix(knode);
}

Node* fix(Node* node) {
    if (node->size == 0 && node->parent == NULL)
        return NULL;

    if (node->size != 0) {
        if (node->parent) return fix(node->parent);
        else return node;
    }

    Node* parent = node->parent;
    if (parent->first->size == 2 || parent->second->size == 2 || parent->size == 2)
        node = redistribute(node);
    else
        node = merge(node);

    return fix(node);
}

Node* redistribute(Node* leaf) {
    Node* parent = leaf->parent;
    Node* first = parent->first;
    Node* second = parent->second;
    Node* third = parent->third;

    if ((parent->size == 2) && (first->size < 2) && (second->size < 2) && (third->size < 2)) {
        if (first == leaf) {
            parent->first = parent->second;
            parent->second = parent->third;
            parent->third = NULL;
            insert_to_node(parent->first, parent->key[0]);
            parent->first->third = parent->first->second;
            parent->first->second = parent->first->first;

            if (leaf->first != NULL) parent->first->first = leaf->first;
            else if (leaf->second != NULL) parent->first->first = leaf->second;

            if (parent->first->first != NULL) parent->first->first->parent = parent->first;

            remove_from_node(parent, parent->key[0]);
            free(first);
        }
        else if (second == leaf) {
            insert_to_node(first, parent->key[0]);
            remove_from_node(parent, parent->key[0]);
            if (leaf->first != NULL) first->third = leaf->first;
            else if (leaf->second != NULL) first->third = leaf->second;

            if (first->third != NULL) first->third->parent = first;

            parent->second = parent->third;
            parent->third = NULL;

            free(second);
        }
        else if (third == leaf) {
            insert_to_node(second, parent->key[1]);
            parent->third = NULL;
            remove_from_node(parent, parent->key[1]);
            if (leaf->first != NULL) second->third = leaf->first;
            else if (leaf->second != NULL) second->third = leaf->second;

            if (second->third != NULL)  second->third->parent = second;

            free(third);
        }
    }
    else if ((parent->size == 2) && ((first->size == 2) || (second->size == 2) || (third->size == 2))) {
        if (third == leaf) {
            if (leaf->first != NULL) {
                leaf->second = leaf->first;
                leaf->first = NULL;
            }

            insert_to_node(leaf, parent->key[1]);
            if (second->size == 2) {
                parent->key[1] = second->key[1];
                remove_from_node(second, second->key[1]);
                leaf->first = second->third;
                second->third = NULL;
                if (leaf->first != NULL) leaf->first->parent = leaf;
            }
            else if (first->size == 2) {
                parent->key[1] = second->key[0];
                leaf->first = second->second;
                second->second = second->first;
                if (leaf->first != NULL) leaf->first->parent = leaf;

                second->key[0] = parent->key[0];
                parent->key[0] = first->key[1];
                remove_from_node(first, first->key[1]);
                second->first = first->third;
                if (second->first != NULL) second->first->parent = second;
                first->third = NULL;
            }
        }
        else if (second == leaf) {
            if (third->size == 2) {
                if (leaf->first == NULL) {
                    leaf->first = leaf->second;
                    leaf->second = NULL;
                }
                insert_to_node(second, parent->key[1]);
                parent->key[1] = third->key[0];
                remove_from_node(third, third->key[0]);
                second->second = third->first;
                if (second->second != NULL) second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = NULL;
            }
            else if (first->size == 2) {
                if (leaf->second == NULL) {
                    leaf->second = leaf->first;
                    leaf->first = NULL;
                }
                insert_to_node(second, parent->key[0]);
                parent->key[0] = first->key[1];
                remove_from_node(first, first->key[1]);
                second->first = first->third;
                if (second->first != NULL) second->first->parent = second;
                first->third = NULL;
            }
        }
        else if (first == leaf) {
            if (leaf->first == NULL) {
                leaf->first = leaf->second;
                leaf->second = NULL;
            }
            insert_to_node(first, parent->key[0]);
            if (second->size == 2) {
                parent->key[0] = second->key[0];
                remove_from_node(second, second->key[0]);
                first->second = second->first;
                if (first->second != NULL) first->second->parent = first;
                second->first = second->second;
                second->second = second->third;
                second->third = NULL;
            }
            else if (third->size == 2) {
                parent->key[0] = second->key[0];
                second->key[0] = parent->key[1];
                parent->key[1] = third->key[0];
                remove_from_node(third, third->key[0]);
                first->second = second->first;
                if (first->second != NULL) first->second->parent = first;
                second->first = second->second;
                second->second = third->first;
                if (second->second != NULL) second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = NULL;
            }
        }
    }
    else if (parent->size == 1) {
        insert_to_node(leaf, parent->key[0]);

        if (first == leaf && second->size == 2) {
            parent->key[0] = second->key[0];
            remove_from_node(second, second->key[0]);

            if (leaf->first == NULL) leaf->first = leaf->second;

            leaf->second = second->first;
            second->first = second->second;
            second->second = second->third;
            second->third = NULL;
            if (leaf->second != NULL) leaf->second->parent = leaf;
        }
        else if (second == leaf && first->size == 2) {
            parent->key[0] = first->key[1];
            remove_from_node(first, first->key[1]);

            if (leaf->second == NULL) leaf->second = leaf->first;

            leaf->first = first->third;
            first->third = NULL;
            if (leaf->first != NULL) leaf->first->parent = leaf;
        }
    }
    return parent;
}

Node* merge(Node* leaf) {
    Node* parent = leaf->parent;

    if (parent->first == leaf) {
        insert_to_node(parent->second, parent->key[0]);
        parent->second->third = parent->second->second;
        parent->second->second = parent->second->first;

        if (leaf->first != NULL) parent->second->first = leaf->first;
        else if (leaf->second != NULL) parent->second->first = leaf->second;

        if (parent->second->first != NULL) parent->second->first->parent = parent->second;

        remove_from_node(parent, parent->key[0]);
        free(parent->first);
        parent->first = NULL;
    }
    else if (parent->second == leaf) {
        insert_to_node(parent->first, parent->key[0]);

        if (leaf->first != NULL) parent->first->third = leaf->first;
        else if (leaf->second != NULL) parent->first->third = leaf->second;

        if (parent->first->third != NULL) parent->first->third->parent = parent->first;

        remove_from_node(parent, parent->key[0]);
        free(parent->second);
        parent->second = NULL;
    }

    if (parent->parent == NULL) {
        Node* tmp = NULL;
        if (parent->first != NULL) tmp = parent->first;
        else tmp = parent->second;
        tmp->parent = NULL;
        free(parent);
        return tmp;
    }
    return parent;
}

int tree_height(Node* node, int h) {
    int max = h;
    if (node->first) {
        max = tree_height(node->first, h + 1);
    }
    if (node->second) {
        int k = tree_height(node->second, h + 1);
        if (k > max) max = k;
    }
    if (node->third) {
        int k = tree_height(node->third, h + 1);
        if (k > max) max = k;
    }
    return max;
}

void print_tree(Node* node) {
    int height = tree_height(node, 1);

    keys = (int*)malloc(pow(3, height) * sizeof(int));

    for (int i = 0; i < pow(3, height); i++)
        keys[i] = -32767;

    through_the_tree(node, 1, 1);

    int width = 7 * (pow(3, height - 1));
    int num = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < pow(3, i); j++) {
            for (int k = 0; k < (width - pow(3, i) * 6) / (pow(3, i) + 1); k++) printf(" ");
            if (keys[num] == -32767) {
                printf("[-;");
                num++;
            }
            else
                printf("[%d;", keys[num++]);
            if (keys[num] == -32767) {
                printf("-]");
                num++;
            }
            else
                printf("%d]", keys[num++]);
        }
        printf("\n");
    }
    exit(0);
}

void through_the_tree(Node* node, int height, int num) {
    if (node != NULL) {
        keys[2 * (num - 1)] = node->key[0];
        if (node->size == 2) keys[2 * (num - 1) + 1] = node->key[1];
        through_the_tree(node->first, height + 1, (pow(3, height) - 1) / 2 + 3 * (num - (pow(3, height - 1) - 1) / 2 - 1) + 1);
        through_the_tree(node->second, height + 1, (pow(3, height) - 1) / 2 + 3 * (num - (pow(3, height - 1) - 1) / 2 - 1) + 2);
        through_the_tree(node->third, height + 1, (pow(3, height) - 1) / 2 + 3 * (num - (pow(3, height - 1) - 1) / 2 - 1) + 3);
    }
}

int main() {
    char command = 0;
    int data = 0;
    Node* node = NULL;

    while (fscanf(stdin, "%c %d\n", &command, &data) != EOF) {

        if (command == 'a') {
            node = insert(node, data);
        }
        else if (command == 'r')
            node = remove_from_tree(node, data);
        else if (command == 'f') {
            if (search(node, data))
                fprintf(stdout, "yes\n");
            else
                fprintf(stdout, "no\n");
        }
    }
}