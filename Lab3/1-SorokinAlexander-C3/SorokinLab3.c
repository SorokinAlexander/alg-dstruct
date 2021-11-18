#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#pragma warning(disable: 4996)
#pragma once

typedef struct node_t {
	struct node_t* next;
	int data;
}node_t;
typedef struct {
	node_t* head, * tail;
	int size;
}queue_t;
typedef struct {
	int size;
	queue_t** links;
}graph_t;

node_t* CreateNode(int n) {
	node_t* node = (node_t*)malloc(sizeof(node_t));
	if (!node) {
		return NULL;
	}
	node->data = n;
	node->next = NULL;
	return node;
}
queue_t* CreateQueue() {
	queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
	if (!queue) {
		return NULL;
	}
	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;
	return queue;
}
graph_t* CreateGraph(int size) {
	graph_t* graph = (graph_t*)malloc(sizeof(graph_t));
	if (!graph) {
		return NULL;
	}
	graph->size = size;
	graph->links = (queue_t**)malloc(size * sizeof(queue_t));
	if (!graph->links) {
		free(graph);
		return NULL;
	}
	for (int i = 0; i < size; i++) {
		graph->links[i] = CreateQueue();
		if (!graph->links[i]) {
			free(graph);
			return NULL;
		}
	}
	return graph;
}
void DeleteQueue(queue_t* queue) {
	node_t* node = queue->head;
	for (int i = 0; i < queue->size; i++) {
		node_t* tmp = node;
		node = node->next;
		free(tmp);
	}
	free(queue);
}
void GraphDelete(graph_t* graph) {
	if (graph) {
		for (int i = 0; i < graph->size; i++) {
			DeleteQueue(graph->links[i]);
		}
		free(graph->links);
		free(graph);
	}
}
void Enqueue(queue_t* queue, int n) {
	node_t* node = CreateNode(n);
	if (!node) {
		return;
	}
	if (queue->size > 0) {
		queue->tail->next = node;
		queue->tail = node;
	}
	else {
		queue->head = node;
		queue->tail = node;
	}
	queue->size++;
}
int Dequeue(queue_t* queue) {
	if (queue->size > 0) {
		node_t* node = queue->head;
		int value = node->data;
		queue->head = node->next;
		free(node);
		queue->size--;
		return value;
	}
	else
		return 0;
}
void GraphRead(FILE* in, graph_t* graph) {
	int first, nextVal = 0;
	char c;
	if (!in) {
		return;
	}
	while (fscanf(in, "%i%c", &first, &c) > 0) {
		if (c != ' ') {
			continue;
		}
		while (fscanf(in, "%i%c", &nextVal, &c) > 0) {
			Enqueue(graph->links[first], nextVal);
			Enqueue(graph->links[nextVal], first);
			if (c != ' ') {
				break;
			}
		}
	}
}
int BFS(graph_t* graph, FILE* out) {
	bool* flags = (bool*)calloc(graph->size, sizeof(bool));
	if (!flags) {
		return 0;
	}
	queue_t* queue = CreateQueue();
	if (!queue) {
		free(flags);
		return 0;
	}
	int cur = 0;
	flags[0] = 1;
	do {
		fprintf(out, "%d", cur);
		int size = graph->links[cur]->size;
		for (int i = 0; i < size; i++) {
			int n = Dequeue(graph->links[cur]);
			if (!flags[n]) {
				flags[n] = 1;
				Enqueue(queue, n);
			}
		}
		if (queue->size > 0) {
			fprintf(out, " ");
		}
	} while ((cur = Dequeue(queue)));
	free(flags);
	DeleteQueue(queue);
	return 1;
}

int main(void) {
	int size = 0;
	FILE* file = stdin;
	fscanf(file, "%d\n", &size);
	if (!size) {
		return 0;
	}
	graph_t* graph = CreateGraph(size);
	if (!graph) {
		perror("");
		return -1;
	}
	GraphRead(file, graph);
	BFS(graph, stdout);
	GraphDelete(graph);
	return 0;
}
