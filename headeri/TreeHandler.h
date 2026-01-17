#pragma once
#ifndef TREEHANDLER_H
#define TREEHANDLER_H
#define MAX_NAME 100

#include "HashManager.h"

typedef struct Child {
	WCHAR ExecutableName[MAX_NAME];
	WCHAR ExecutablePath[MAX_PATH];
	DWORD pid;
	DWORD parentid;
	struct Child* next;
}Child;

typedef struct Parent {
	struct Child* nextChild;
	struct Parent* nextRight;
	struct Parent* nextLeft;
	WCHAR ExecutableName[MAX_NAME];
	WCHAR ExecutablePath[MAX_PATH];
	DWORD pid;
	DWORD ppid;
}Parent;

DWORD hash_function(DWORD);
Parent* CreateParent();
Child* CreateChild();
int read_files(Parent**, HashTable**);
Parent* add_to_tree(Parent*, Parent*);
Parent* add_to_task(Parent*, Child*);
Parent* find_pid(Parent*, Child*);
int kill_process(Parent*, HashTable*, DWORD);

#endif 