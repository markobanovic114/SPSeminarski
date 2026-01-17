#pragma once
#ifndef HASHMANAGER_H
#define HASHMANAGER_H
#define MAX_HASH 1024
#define MAX_NAME_HASH 100

#include <windows.h>

typedef struct HashNode {
	DWORD pid, ppid;
	WCHAR ExeName[MAX_NAME_HASH];
	WCHAR ExePath[MAX_PATH];
	struct HashNode* next;
}HashNode;

typedef struct HashTable {
	struct HashNode* next;
}HashTable;

DWORD hash_function(DWORD);
HashTable* CreateTable();
HashNode* CreateNode(WCHAR*, WCHAR*, DWORD, DWORD);
HashTable* add_to_table(HashTable*, WCHAR*, WCHAR*, DWORD, DWORD);
int find_hash(HashTable*, DWORD);

#endif