#include "header.h"

PaintTraverse oPaintTraverse;
CreateMove oCreateMove;
cvmth64* VMThook;
cvmth64* VMThook2;
CoreHaxFunc* core;
hack* myHack;
DrawStuff* draw;

bool __DEBUG = 0;
char __DEBUG_BUFF[512];
wchar_t __DEBUG_BUFF_W[512];
bool MAIN_SWITCH = 0;
bool CROSSHAIR_SWITCH = 1;
bool RADAR_SWITCH = 1;
bool AIMBOT_SWITCH = 1;

void DoMeAFavour()
{
	VMThook = new cvmth64();
	VMThook2 = new cvmth64();

	core = new CoreHaxFunc();
	if (VMThook->bInitialize((PDWORD64*)core->g_pIPanel))
	{
		oPaintTraverse = (PaintTraverse)VMThook->dwHookMethod((DWORD64)HackMechanics::pt, 46);
		myHack = new hack();
		draw = new DrawStuff();

		if (VMThook2->bInitialize((PDWORD64*)core->g_pClient))
		{
			oCreateMove = (CreateMove)VMThook2->dwHookMethod((DWORD64)HackMechanics::Hooked_CreateMove, 24);
		}
		else
		{
			MessageBox(NULL, "error2", "", MB_OK);
		}

		if (SHOW_DEBUG)
		{
			static cvmth64* tmpHook = new cvmth64();
			if (tmpHook->bInitialize((PDWORD64*)core->g_pTrace))
			{
				sprintf_s(__DEBUG_BUFF, "func count: %d", tmpHook->iGetFuncCount());
				core->debug->toFile(__DEBUG_BUFF);

				static int MAX_LOOP = tmpHook->iGetFuncCount();
				for (int i = 0; i < MAX_LOOP; i++)
				{
					sprintf_s(__DEBUG_BUFF, "func%d: %d", i, tmpHook->dwGetMethodAddress(i));
					core->debug->toFile(__DEBUG_BUFF);
				}
			}
		}
	}
	else
	{
		MessageBox(NULL, "error1", "", MB_OK);
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