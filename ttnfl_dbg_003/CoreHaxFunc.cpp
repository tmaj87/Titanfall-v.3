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


