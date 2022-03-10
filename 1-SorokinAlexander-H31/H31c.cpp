#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Treap {
	int data;
	int priority;
	struct Treap* left;
	struct Treap* right;
} Treap;

Treap* CreateTreap(int data, int priority, Treap* left, Treap* right) {
	Treap* treap = (Treap*)malloc(sizeof(Treap));
	treap->data = data;
	treap->priority = priority;
	treap->left = left;
	treap->right = right;
	return treap;
}

Treap* MergeTreaps(Treap* left, Treap* right) {
	if (left == NULL)
		return right;
	if (right == NULL)
		return left;

	if (left->priority > right->priority) {
		Treap* newR = MergeTreaps(left->right, right);
		return CreateTreap(left->data, left->priority, left->left, newR);
	}
	else {
		Treap* newL = MergeTreaps(left, right->left);
		return CreateTreap(right->data, right->priority, newL, right->right);
	}
}

void SplitTreap(Treap* treap, int data, Treap** left, Treap** right) {
	Treap* newTreap = NULL;
	if (treap->data <= data) {
		if (treap->right == NULL)
			*right = NULL;
		else
			SplitTreap(treap->right, data, &newTreap, right);
		*left = CreateTreap(treap->data, treap->priority, treap->left, newTreap);
	}
	else {
		if (treap->left == NULL)
			*left = NULL;
		else
			SplitTreap(treap->left, data, left, &newTreap);
		*right = CreateTreap(treap->data, treap->priority, newTreap, treap->right);
	}
}

Treap* AddData(int data, Treap* treap) {
	Treap* left, * right;
	SplitTreap(treap, data, &left, &right);
	Treap* newTreap = CreateTreap(data, rand(), NULL, NULL);
	return MergeTreaps(MergeTreaps(left, newTreap), right);
}

Treap* RemoveData(int data, Treap* treap) {
	Treap* left, * middle, * right;
	SplitTreap(treap, data - 1, &left, &right);
	SplitTreap(right, data, &middle, &right);
	return MergeTreaps(left, right);
}

int SearchData(Treap* treap, int data) {
	while (treap != NULL) {
		if (treap->data == data)
			return 1;
		else if (treap->data < data)
			treap = treap->right;
		else
			treap = treap->left;
	}
	return 0;
}

int main() {
	char command = 0;
	int data = 0;
	Treap* treap = NULL;

	while (fscanf(stdin, "%c %d\n", &command, &data) != EOF) {

		if (command == 'a') {
			if (treap == NULL)
				treap = CreateTreap(data, rand(), NULL, NULL);
			else
				treap = AddData(data, treap);
		}
		else if (command == 'r')
			treap = RemoveData(data, treap);
		else if (command == 'f') {
			if (SearchData(treap, data))
				fprintf(stdout, "yes\n");
			else
				fprintf(stdout, "no\n");
		}

	}

}