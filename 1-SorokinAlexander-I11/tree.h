#pragma once

typedef struct Node {
	int size;
	int key[3];
	struct Node* first;
	struct Node* second;
	struct Node* third;
	struct Node* fourth;
	struct Node* parent;
	struct Node* brother;
}Node;

Node* create_node(int key, Node* first, Node* second, Node* parent);
int find(Node* node, int k);
void sort(Node* node);
void insert_to_node(Node* node, int k);
void remove_from_node(Node* node, int k);
void become_node2(Node* node, int k, Node* first, Node* second);
int is_leaf(Node* node);
Node* insert(Node* node, int k);
Node* split(Node* node);
Node* search(Node* node, int k);
Node* search_min(Node* node);
Node* remove_from_tree(Node* node, int k);
Node* fix(Node* node);
Node* redistribute(Node* leaf);
Node* merge(Node* leaf);
int tree_height(Node* node, int h);
void print_tree(Node* node);
void through_the_tree(Node* node, int height, int num);
