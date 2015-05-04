#include "header.h"

tPaintTraverse oPaintTraverse;
tCreateMove oCreateMove;
cvmth64* VMThook;
cvmth64* VMThook2;
base* core;
hack* myHack;

bool __DEBUG = 0;
char __DEBUG_BUFF[512];
wchar_t __DEBUG_BUFF_W[512];
bool MAIN_SWITCH = 0;
bool CROSSHAIR_SWITCH = 1;
bool RADAR_SWITCH = 1;
bool NORECOIL_SWITCH = 1;

void DoMeAFavour()
{
	VMThook = new cvmth64();
	VMThook2 = new cvmth64();

	core = new base();
	if (VMThook->bInitialize((PDWORD64*)core->g_pIPanel))
	{
		oPaintTraverse = (tPaintTraverse)VMThook->dwHookMethod((DWORD64)HackMechanics::pt, 46);
		myHack = new hack();

		if (VMThook2->bInitialize((PDWORD64*)core->g_pClient))
		{
			oCreateMove = (tCreateMove)VMThook2->dwHookMethod((DWORD64)HackMechanics::Hooked_CreateMove, 24);
		}
	}
	else
	{
		MessageBox(NULL, "error", "", MB_OK);
	}
}



BOOL APIENTRY DllMain(HMODULE handle, DWORD  reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(handle);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DoMeAFavour, NULL, 0, NULL);
		break;
	}
	return 1;
}