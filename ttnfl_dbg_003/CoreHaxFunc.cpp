#include "header.h"

tPlat_FloatTime Plat_FloatTime;

CoreHaxFunc::CoreHaxFunc()
{
	debug = new dbg();
	debug->cleanFile();
	initFaces();
}

void CoreHaxFunc::initFaces()
{
	// wait till modules load
	while (hEngine == NULL)
	{
		hEngine = GetModuleHandle("engine.dll");
		Sleep(100);
	}
	while (hClient == NULL)
	{
		hClient = GetModuleHandle("client.dll");
		Sleep(100);
	}
	while (hVGui == NULL)
	{
		hVGui = GetModuleHandle("vguimatsurface.dll");
		Sleep(100);
	}
	while (hVGui2 == NULL)
	{
		hVGui2 = GetModuleHandle("vgui2.dll");
		Sleep(100);
	}
	while (hTier0 == NULL)
	{
		hTier0 = GetModuleHandle("tier0.dll");
		Sleep(100);
	}

	// funny way of doing this
	g_pEngineFactory = (tCreateInterface)GetProcAddress(hEngine, "CreateInterface");
	g_ClientFactory = (tCreateInterface)GetProcAddress(hClient, "CreateInterface");
	g_VGUIFactory = (tCreateInterface)GetProcAddress(hVGui, "CreateInterface");
	g_VGUI2Factory = (tCreateInterface)GetProcAddress(hVGui2, "CreateInterface");

	// ...
	g_pEngine = (EngineClient*)g_pEngineFactory("VEngineClient013", NULL);
	g_pTrace = (IEngineTrace*)g_pEngineFactory("EngineTraceClient004", NULL);
	g_pEntList = (CEntList*)g_ClientFactory("VClientEntityList003", NULL);
	g_pSurface = (ISurface*)g_VGUIFactory("VGUI_Surface031", NULL);
	g_pIPanel = (IPanel*)g_VGUI2Factory("VGUI_Panel009", NULL);
	//g_pDebug = (IVDebugOverlay*)g_pEngineFactory("VDebugOverlay004", NULL);
	g_pClient = (IBaseClientDLL*)g_ClientFactory("VClient018", NULL);
	Plat_FloatTime = (tPlat_FloatTime)GetProcAddress(hTier0, "Plat_FloatTime");

	if (SHOW_DEBUG)
	{
		sprintf_s(__DEBUG_BUFF, "g_pTrace: %p", g_pTrace);
		debug->toFile(__DEBUG_BUFF);
	}
}

void CoreHaxFunc::keyManager()
{
	const byte MAIN_SWITCH_KEY = VK_F5;
	const byte CROSSHAIR_SWITCH_KEY = VK_F6;
	const byte RADAR_SWITCH_KEY = VK_F7;
	const byte AIMBOT_SWITCH_KEY = VK_F8;
	const byte __DEBUG_KEY = VK_F9;

	static bool keyBlock[5] = { 1, 1, 1, 1, 1 };

	if (GetAsyncKeyState(MAIN_SWITCH_KEY) && keyBlock[0])
	{
		keyBlock[0] = 0;
		MAIN_SWITCH = MAIN_SWITCH > 0 ? 0 : 1;
	}
	else if (!GetAsyncKeyState(MAIN_SWITCH_KEY) && !keyBlock[0]) keyBlock[0] = 1;

	if (GetAsyncKeyState(AIMBOT_SWITCH_KEY) && keyBlock[1])
	{
		keyBlock[1] = 0;
		AIMBOT_SWITCH = AIMBOT_SWITCH > 0 ? 0 : 1;
	}
	else if (!GetAsyncKeyState(AIMBOT_SWITCH_KEY) && !keyBlock[1]) keyBlock[1] = 1;

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

void CoreHaxFunc::VectorAngles(const float *forward, float *angles)
{
	float tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 180;
		else
			pitch = 0;
	}
	else
	{
		yaw = (float)(atan2(forward[1], forward[0]) * 180 / M_PI);
		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (float)(atan2(-forward[2], tmp) * 180 / M_PI);

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}
}

bool CoreHaxFunc::visibilityCheck(Vector &vecAbsStart, Vector &vecAbsEnd, Vector &posToWrite)
{
	Trace tr;
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	core->g_pTrace->TraceRay(ray, 0x4600400B, 0, &tr); // 0x46004003

	/*if (tr.m_pEnt && tr.m_pEnt == pTargetPlayer)
		return true;*/

	swprintf_s(__DEBUG_BUFF_W, L"%.4f", tr.fraction);
	core->g_pSurface->DrawSetTextPos(posToWrite.x, posToWrite.y);
	core->g_pSurface->DrawPrintText(__DEBUG_BUFF_W, wcslen(__DEBUG_BUFF_W));

	if (tr.fraction >= 1.0f)
		return true;

	return false;
}
