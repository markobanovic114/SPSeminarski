#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <wchar.h>
#include "headeri\HashManager.h"
#include "headeri\ProcessHandler.h"
#include "headeri\TreeHandler.h"
#include "headeri\MemoryManager.h"
#include "headeri\PrintHandler.h"

int main() {
	int option;
	DWORD pid_to_find;
	DWORD pid_to_close;
jump:
	if (getprocesses() || isolateparent()) {
		return 1;
	}

	HashTable* table = CreateTable();
	if (!table) {
		return 1;
	}
	Parent* root = NULL;

	if (read_files(&root, &table)) {
		return 1;
	}

	do {
		puts("----------------------------------");
		puts("Sto zelite uciniti?\n1 - Ispisi samo roditeljske procese i PID-ove\n2 - Ispisi cijelu listu procesa i PID-ove\n3 - Pronadi proces po PID-u\n4 - Zatvori proces po PID-u\n5 - Osvjezi procese\n0 - Izlaz");
		puts("----------------------------------");
		option = read_std();

		switch (option) {
		case 0:
			break;
		case 1:
			print_parents(root);
			break;
		case 2:
			print_tree(root);
			break;
		case 3:
			puts("Unesite PID koji zelite pronaci: "); pid_to_find = read_pid();
			find_hash(table, pid_to_find);
			break;
		case 4:
			puts("Unesite PID procesa za zatvoriti: "); pid_to_close = read_pid();
			if (kill_process(root, table, pid_to_close)) {
				goto jump;
				break;
			}
			goto jump;
			break;
		case 5:
			free_tree(root);
			free_hash(table);
			goto jump;
			break;
		default:
			printf("Nepoznat unos.\n");
			break;
		}
	} while (option != 0);

	free_tree(root);
	free_hash(table);
}