#define	_CRT_SECURE_NO_WARNINGS
#include "HashManager.h"
#include "TreeHandler.h"
#include "MemoryManager.h"
#include "ProcessHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <wchar.h>

Parent* CreateParent() {
	Parent* ParentNode = calloc(1, sizeof(Parent));
	if (!ParentNode) {
		printf("Greska u stvaranju parent procesa.\n");
		return NULL;
	}
	return ParentNode;
}

Child* CreateChild() {
	Child* ChildNode = calloc(1, sizeof(Child));
	if (!ChildNode) {
		printf("Greska u stvaranju child procesa.\n");
		free(ChildNode);
		return NULL;
	}
	return ChildNode;
}

int read_files(Parent** root, HashTable** table) {
	FILE* f1 = _wfopen(L"output/parents.txt", L"r, ccs = UTF-8");
	if (!f1) {
		printf("Greska u otvaranju datoteke kod stavljanja u stablo.\n");
		return 1;
	}
	WCHAR buffer[1024];

	while (fgetws(buffer, 1024, f1)) {
		Parent* parentNode = CreateParent();
		if (!parentNode) {
			return 1;
		}
		if (swscanf(buffer, L"Name: %99[^|] | Path: %259[^|] | PID: %lu\n", parentNode->ExecutableName, parentNode->ExecutablePath, &parentNode->pid) == 3) {
			*root = add_to_tree(*root, parentNode);
		}
	}

	fclose(f1);

	wmemset(buffer, 0, 1024);
	FILE* f2 = _wfopen(L"output/output.txt", L"r, ccs = UTF-8");
	if (!f2) {
		printf("Greska u otvaranju datoteke kod stavljanja u stablo.\n");
		return 1;
	}

	while (fgetws(buffer, 1024, f2)) {
		Child* childNode = CreateChild();
		if (!childNode) {
			return 1;
		}
		if (swscanf(buffer, L"Name: %99[^|] | Path: %259[^|] | PID: %lu | Parent: %lu\n", childNode->ExecutableName, childNode->ExecutablePath, &childNode->pid, &childNode->parentid) == 4) {
			HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, childNode->pid);
			if (h) {
				DWORD exitCode;
				if (GetExitCodeProcess(h, &exitCode) && exitCode == STILL_ACTIVE) {
				}
				CloseHandle(h);
			}

			*table = add_to_table(*table, childNode->ExecutableName, childNode->ExecutablePath, childNode->pid, childNode->parentid);
			*root = find_pid(*root, childNode);
		}
	}

	fclose(f2);
	return 0;
}

Parent* find_pid(Parent* root, Child* to_add) {
	if (root == NULL) {
		return root;
	}

	if (root->pid == to_add->parentid) {
		root = add_to_task(root, to_add);
	}
	find_pid(root->nextLeft, to_add);
	find_pid(root->nextRight, to_add);
	return root;
}

Parent* add_to_tree(Parent* root, Parent* to_add) {
	if (root == NULL) {
		return to_add;
	}

	if (to_add->pid < root->pid) {
		root->nextLeft = add_to_tree(root->nextLeft, to_add);
	}

	else if (to_add->pid > root->pid) {
		root->nextRight = add_to_tree(root->nextRight, to_add);
	}

	else {
		free(to_add);
	}

	return root;
}

Parent* add_to_task(Parent* root, Child* to_add) {
	if (root->nextChild == NULL) {
		root->nextChild = to_add;
		return root;
	}

	to_add->next = root->nextChild;
	root->nextChild = to_add;
	return root;
}

int kill_process(Parent* root, HashTable* table, DWORD pid) {
	free_tree(root);
	free_hash(table);

	HANDLE phandle = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	if (!phandle) {
		printf("Greska u stvaranju handlea za zatvaranje procesa.\n");
		return 1;
	}

	if (!TerminateProcess(phandle, 1)) {
		printf("Greska u zatvaranju procesa.\n");
		CloseHandle(phandle);
		return 1;
	}

	WaitForSingleObject(phandle, INFINITE);
	CloseHandle(phandle);
	return 0;
}