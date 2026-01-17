#define _CRT_SECURE_NO_WARNINGS
#include "HashManager.h"
#include "TreeHandler.h"
#include "MemoryManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void free_child(Child* head) {
	Child* temp;
	while (head != NULL) {
		temp = head->next;
		free(head);
		head = temp;
	}
}

void free_tree(Parent* root) {
	if (root == NULL) {
		return;
	}
	free_child(root->nextChild);
	free_tree(root->nextLeft);
	free_tree(root->nextRight);
	free(root);
}

void free_hash(HashTable* table) {
	if (table == NULL) {
		return;
	}

	for (int i = 0; i < MAX_HASH; i++) {
		HashNode* node = (table + i)->next;
		while (node) {
			HashNode* tmp = node->next;
			free(node);
			node = tmp;
		}
	}

	free(table);
}
