#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include "vmth64.h"
#include "hack.h"

extern bool __DEBUG;
extern char __DEBUG_BUFF[512];
extern bool MAIN_SWITCH;
extern bool CROSSHAIR_SWITCH;
extern bool RADAR_SWITCH;
extern bool NORECOIL_SWITCH;

typedef struct _PEB_LDR_DATA {
	BYTE       Reserved1[8];
	PVOID      Reserved2[3];
	LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

/*
for linker errors:
    add /FORCE:MULTIPLE to the linker command line options
*/
