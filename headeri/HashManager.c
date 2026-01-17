#define _CRT_SECURE_NO_WARNINGS
#include "HashManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

DWORD hash_function(DWORD pid) {
	return pid % MAX_HASH;
}

HashTable* CreateTable() {
	HashTable* table = (HashTable*)malloc(MAX_HASH * sizeof(HashTable));
	if (!table) {
		printf("Greska u alociranju hash tablice.\n");
		return NULL;
	}
	for (int i = 0; i < MAX_HASH; i++) {
		(table + i)->next = NULL;
	}
	return table;
}

HashNode* CreateNode(WCHAR* name, WCHAR* path, DWORD pid, DWORD ppid) {
	HashNode* node = (HashNode*)calloc(1, sizeof(HashNode));
	if (!node) {
		printf("Greska u alokaciji nodea.\n");
		return NULL;
	}
	node->pid = pid;
	node->ppid = ppid;
	wcscpy(node->ExeName, name);
	wcscpy(node->ExePath, path);
	return node;
}

HashTable* add_to_table(HashTable* table, WCHAR* name, WCHAR* path, DWORD pid, DWORD ppid) {
	HashNode* node = CreateNode(name, path, pid, ppid);
	if (!node) {
		return NULL;
	}
	DWORD pidx = hash_function(pid);

	if ((table + pidx)->next == NULL) {
		(table + pidx)->next = node;
		return table;
	}

	node->next = (table + pidx)->next;
	(table + pidx)->next = node;

	return table;
}

int find_hash(HashTable* table, DWORD pid) {
	DWORD pidx = hash_function(pid);
	if ((table + pidx)->next == NULL) {
		printf("Ne postoji.\n");
		return 1;
	}
	HashNode* temp = (table + pidx)->next;

	while (temp) {
		if (temp->pid == pid) {
			wprintf(L"Name: %ls | Path: %ls | PID: %lu | Parent: %lu\n\n", temp->ExeName, temp->ExePath, temp->pid, temp->ppid);
			return 0;
		}
		temp = temp->next;
	}
	printf("Ne postoji.\n");
	return 1;
}

