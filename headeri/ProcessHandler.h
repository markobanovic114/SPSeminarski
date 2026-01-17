#pragma once
#ifndef PROCESSHANDLER_H
#define PROCESSHANDLER_H
#define MAX_PROCESSES 500
#define MAX_NAME_PROCESS 100

#include <windows.h>

typedef struct ProcessCheck {
	DWORD pid;
	DWORD parentid;
	WCHAR exepath[MAX_PATH];
	WCHAR exename[MAX_NAME_PROCESS];
}ProcessCheck;

int isolateparent();
int getprocesses();

#endif