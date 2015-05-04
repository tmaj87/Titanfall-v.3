#include "header.h"

CoreHaxFunc::CoreHaxFunc()
{
}


CoreHaxFunc::~CoreHaxFunc()
{
}



void CoreHaxFunc::DoMeAFavour()
{
	VMThook = new cvmth64();
	VMThook2 = new cvmth64();

	core = new base();
	if (VMThook->bInitialize((PDWORD64*)core->g_pIPanel))
	{
		oPaintTraverse = (tPaintTraverse)VMThook->dwHookMethod((DWORD64)pt, 46);
		myHack = new hack();

		if (VMThook2->bInitialize((PDWORD64*)core->g_pClient))
		{
			oCreateMove = (tCreateMove)VMThook2->dwHookMethod((DWORD64)Hooked_CreateMove, 24);
		}
	}
	else
	{
		MessageBox(NULL, "error", "", MB_OK);
	}
}


MODULEINFO GetModuleInfo(char *szModule){
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}


DWORD FindPattern(char *module, char *pattern, char *mask)
{
	MODULEINFO mInfo = GetModuleInfo(module);
	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	DWORD size = (DWORD)mInfo.SizeOfImage;
	DWORD patternLength = (DWORD)strlen(mask);

	for (DWORD i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (DWORD j = 0; j < patternLength; j++)
		{
			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
		}
		if (found)
		{
			return base + i;
		}
	}
	return NULL;
}

void __fastcall traceLine(const Vector& start /*rcx*/, const Vector& end /*rdx*/, unsigned int mask /*r8*/, const void* ignore /*r9*/, int collisionGroup, int unk, Trace* trace)
{
	static decltype(traceLine)* UTIL_TraceLine;

	if (!UTIL_TraceLine)
	{
		UTIL_TraceLine = (decltype(traceLine)*)FindPattern(
			"client.dll",
			"\x48\x8B\xC4\x48\x89\x58\x08\x48\x89\x70\x10\x57\x48\x81\xEC\x00\x00\x00\x00\xF3\x0F", // fuck, wrong..!
			"xxxxxxxxxxxxxxx????xx");
	}

	UTIL_TraceLine(start, end, mask, ignore, collisionGroup, unk, trace);
}



void CoreHaxFunc::keyManager()
{
	const byte MAIN_SWITCH_KEY = VK_F5;
	const byte NORECOIL_SWITCH_KEY = VK_F8;
	const byte RADAR_SWITCH_KEY = VK_F6;
	const byte __DEBUG_KEY = VK_F9;
	const byte CROSSHAIR_SWITCH_KEY = VK_F7;

	static bool keyBlock[5] = { 1, 1, 1, 1, 1 };

	if (GetAsyncKeyState(MAIN_SWITCH_KEY) && keyBlock[0])
	{
		keyBlock[0] = 0;
		MAIN_SWITCH = MAIN_SWITCH > 0 ? 0 : 1;
	}
	else if (!GetAsyncKeyState(MAIN_SWITCH_KEY) && !keyBlock[0]) keyBlock[0] = 1;

	if (GetAsyncKeyState(NORECOIL_SWITCH_KEY) && keyBlock[1])
	{
		keyBlock[1] = 0;
		NORECOIL_SWITCH = NORECOIL_SWITCH > 0 ? 0 : 1;
	}
	else if (!GetAsyncKeyState(NORECOIL_SWITCH_KEY) && !keyBlock[1]) keyBlock[1] = 1;

	if (GetAsyncKeyState(RADAR_SWITCH_KEY) && keyBlock[2])
	{
		keyBlock[2] = 0;
		RADAR_SWITCH = RADAR_SWITCH > 0 ? 0 : 1;
	}
	else if (!GetAsyncKeyState(RADAR_SWITCH_KEY) && !keyBlock[2]) keyBlock[2] = 1;

	if (GetAsyncKeyState(__DEBUG_KEY) && keyBlock[3])
	{
		keyBlock[3] = 0;
		__DEBUG = __DEBUG > 0 ? 0 : 1;
	}
	else if (!GetAsyncKeyState(__DEBUG_KEY) && !keyBlock[3]) keyBlock[3] = 1;

	if (GetAsyncKeyState(CROSSHAIR_SWITCH_KEY) && keyBlock[4])
	{
		keyBlock[4] = 0;
		CROSSHAIR_SWITCH = CROSSHAIR_SWITCH > 0 ? 0 : 1;
	}
	else if (!GetAsyncKeyState(CROSSHAIR_SWITCH_KEY) && !keyBlock[4]) keyBlock[4] = 1;
}


