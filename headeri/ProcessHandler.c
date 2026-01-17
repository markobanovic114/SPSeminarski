#define _CRT_SECURE_NO_WARNINGS
#include "ProcessHandler.h"
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

int getprocesses() {
	FILE* f1 = fopen("output/output.txt", "w");
	if (!f1) {
		printf("Greška u otvaranju datoteke kod dobavljanja procesa.\n");
		return 1;
	}

	HANDLE snapshot;
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32); //postavi na sizeof(PROCESSENTRY32) nez zasto pise u dokumentaciji

	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //moze i SNAPALL ali onda PID mora bit specificiran
	if (snapshot == INVALID_HANDLE_VALUE) {
		printf("Greška u stvaranju snapshota.\n");
		return 1;
	}

	if (!Process32First(snapshot, &pe)) {
		printf("Greška u proraèunu procesa.\n");
		CloseHandle(snapshot);
		return 1;
	}

	do {
		DWORD pid = pe.th32ProcessID; // postavi na PID koji handle ima nakon Process32First funkcije
		DWORD parentid = pe.th32ParentProcessID;
		char imagePath[MAX_PATH] = "<access denied>";
		HANDLE ProcessH; //novi handle

		ProcessH = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);

		if (ProcessH) {
			DWORD exitCode;
			if (!GetExitCodeProcess(ProcessH, &exitCode) || exitCode != STILL_ACTIVE) {
				CloseHandle(ProcessH);
				continue;
			}

			DWORD size = MAX_PATH;
			if (!QueryFullProcessImageNameA(ProcessH, 0, imagePath, &size)) {
				snprintf(imagePath, MAX_PATH, "<unknown>");
			}

			CloseHandle(ProcessH);
		}
		fwprintf(f1, L"Name: %-30ls | Path: %S | PID: %-6lu | Parent: %-6lu\n", pe.szExeFile, imagePath, pid, parentid);

	} while (Process32Next(snapshot, &pe));
	CloseHandle(snapshot);
	fclose(f1);
	return 0;
}

int isolateparent() {
	FILE* f1 = _wfopen(L"output/output.txt", L"r, ccs=UTF-8");
	FILE* f2 = _wfopen(L"output/parents.txt", L"w, ccs=UTF-8");
	if (!f1 || !f2) {
		printf("Greska u otvaranju datoteke kod odvajanja procesa.\n");
		return 1;
	}

	ProcessCheck* processes_list = calloc(MAX_PROCESSES, sizeof(ProcessCheck));
	if (!processes_list) {
		printf("Greska u alociranju.\n");
		free(processes_list);
		return 1;
	}
	int counter = 0;
	WCHAR buffer[1024];

	while (counter < MAX_PROCESSES && fgetws(buffer, 1024, f1)) {
		if (swscanf(buffer, L"Name: %99[^|] | Path: %259[^|] | PID: %lu | Parent: %lu\n", processes_list[counter].exename, processes_list[counter].exepath, &processes_list[counter].pid, &processes_list[counter].parentid) == 4) {
			counter++;
		}
	}

	fclose(f1);

	for (int i = 0; i < counter; i++) {
		for (int j = 0; j < counter; j++) {
			if ((processes_list[j].parentid == processes_list[i].pid) && (processes_list[j].pid != processes_list[i].pid)) {
				fwprintf(f2, L"Name: %ls | Path: %ls | PID: %lu\n", processes_list[i].exename, processes_list[i].exepath, processes_list[i].pid);
				break;
			}
		}
	}
	
	free(processes_list);
	fclose(f2);
	return 0;
}