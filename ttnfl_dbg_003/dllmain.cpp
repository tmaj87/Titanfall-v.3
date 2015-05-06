#include "header.h"

tPaintTraverse oPaintTraverse;
tCreateMove oCreateMove;
cvmth64* VMThook;
cvmth64* VMThook2;
base* core;
hack* myHack;
HackMechanics* hackEngine;

bool __DEBUG = 0;
char __DEBUG_BUFF[512];
wchar_t __DEBUG_BUFF_W[512];
bool MAIN_SWITCH = 0;
bool CROSSHAIR_SWITCH = 1;
bool RADAR_SWITCH = 0;
bool AIMBOT_SWITCH = 1;

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
		else
		{
			MessageBox(NULL, "error2", "", MB_OK);
		}

		// !!
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

			/*
			int funcIdxs2[] = { 42, 43, 44 }; // 30,..38,39,40
			for (int i = 0; i < sizeof(funcIdxs2) / sizeof(*funcIdxs2); i++)
			{
				func2check2 = (testFunc2)VMThook->dwGetMethodAddress(funcIdxs2[i]);
				sprintf_s(buff2, "%d --> %s", funcIdxs2[i], func2check2(pThis, vguiPanel));
				d->toFile(buff2);
			}
			d->toFile("");
			*/
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