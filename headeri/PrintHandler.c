#define _CRT_SECURE_NO_WARNINGS
#include "TreeHandler.h"
#include "PrintHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void print_tree(Parent* root) {
	int suboption;
	do {
		puts("----------------------------------");
		puts("Kako zelite ispisati stablo:\n1 - inorder\n2 - preorder\n3 - postorder\n0 - Natrag");
		puts("----------------------------------");
		suboption = read_std();
		switch (suboption) {
		case 0:
			break;
		case 1:
			print_inorder(root);
			break;
		case 2:
			print_preorder(root);
			break;
		case 3:
			print_postorder(root);
			break;
		default:
			printf("Nepoznat unos.\n");
			break;
		}
	} while (suboption != 0);
}

void print_parents(Parent* root) {
	if (root == NULL) {
		return;
	}

	print_parents(root->nextLeft);
	wprintf(L"Name: %-30ls | Path: %-30ls | PID: %-6lu\n", root->ExecutableName, root->ExecutablePath, root->pid);
	print_parents(root->nextRight);
}

void print_inorder(Parent* root) {
	if (root == NULL) {
		return;
	}

	Child* temp = root->nextChild;

	print_inorder(root->nextLeft);
	wprintf(L"Name: %-30ls | Path: %-30ls | PID: %-6lu\n", root->ExecutableName, root->ExecutablePath, root->pid);
	while (temp != NULL) {
		wprintf(L"\t|Name: %-30ls | Path: %-30ls | PID: %-6lu | PPID: %-6lu\n", temp->ExecutableName, temp->ExecutablePath, temp->pid, temp->parentid);
		temp = temp->next;
	}
	print_inorder(root->nextRight);
}

void print_preorder(Parent* root) {
	if (root == NULL) {
		return;
	}

	Child* temp = root->nextChild;

	wprintf(L"Name: %-30ls | Path: %-30ls | PID: %-6lu\n", root->ExecutableName, root->ExecutablePath, root->pid);
	while (temp != NULL) {
		wprintf(L"\t|Name: %-30ls | Path: %-30ls | PID: %-6lu | PPID: %-6lu\n", temp->ExecutableName, temp->ExecutablePath, temp->pid, temp->parentid);
		temp = temp->next;
	}
	print_preorder(root->nextLeft);
	print_preorder(root->nextRight);
}

void print_postorder(Parent* root) {
	if (root == NULL) {
		return;
	}

	Child* temp = root->nextChild;

	print_postorder(root->nextLeft);
	print_postorder(root->nextRight);
	wprintf(L"Name: %-30ls | Path: %-30ls | PID: %-6lu\n", root->ExecutableName, root->ExecutablePath, root->pid);
	while (temp != NULL) {
		wprintf(L"\t|Name: %-30ls | Path: %-30ls | PID: %-6lu | PPID: %-6lu\n", temp->ExecutableName, temp->ExecutablePath, temp->pid, temp->parentid);
		temp = temp->next;
	}
}

int read_std() {
	char buffer[1024];

	if (!fgets(buffer, sizeof buffer, stdin)) {
		return -1;
	}

	if (buffer[0] == '\n') {
		return -1;
	}

	if (buffer[1] != '\n' && buffer[1] != '\0')
		return -1;

	if (buffer[0] < '0' || buffer[0] > '5')
		return -1;

	return buffer[0] - '0';
}

DWORD read_pid(void) {
	char buffer[32];
	if (!fgets(buffer, sizeof buffer, stdin))
		return 0;

	char* endptr;
	long val = strtol(buffer, &endptr, 10);

	if (val <= 0 || val > UINT32_MAX || *endptr != '\n')
		return 0;

	return (DWORD)val;
}