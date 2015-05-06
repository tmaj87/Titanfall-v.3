#include "header.h"

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

	yaw = (float)(atan2(forward[1], forward[0]) * 180 / M_PI);
	tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
	pitch = (float)(atan2(-forward[2], tmp) * 180 / M_PI);

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

void __fastcall CoreHaxFunc::traceLine(const Vector& start /*rcx*/, const Vector& end /*rdx*/, unsigned int mask /*r8*/, const void* ignore /*r9*/, int collisionGroup, int unk, Trace* trace)
{
	static decltype(traceLine)* UTIL_TraceLine;

	if (!UTIL_TraceLine)
	{
		UTIL_TraceLine = (decltype(traceLine)*)findPatternInModule(
			GetModuleHandle("client.dll"),
			(BYTE*)"\x48\x8B\xC4\x48\x89\x58\x08\x48\x89\x70\x10\x57\x48\x81\xEC\x00\x00\x00\x00\xF3\x0F",
			"xxxxxxxxxxxxxxx????xx");
	}
	else
	{
		UTIL_TraceLine(start, end, mask, ignore, collisionGroup, unk, trace);
	}
}

DWORD64* CoreHaxFunc::findPatternInModule(void* var1, byte* var2, const char* var3)
{
	return 0;
}

bool CoreHaxFunc::visibilityCheck(Vector &vecAbsStart, Vector &vecAbsEnd, CBaseEntity *baseEnt)
{
	//player_info_t pinfo;
	Trace tr;
	Ray_t ray;
	ray.Init(vecAbsStart, vecAbsEnd);
	core->g_pTrace->TraceRay(ray, 0x4600400B, 0, &tr);

	/*if (tr.allsolid || tr.startsolid)
		return false;*/
	/*if (tr.m_pEnt && tr.m_pEnt == pTargetPlayer)
		return true;*/
	if (tr.fraction >= 1.0f)
		return true;

	return false;

	/*
	if (tr.fraction > 0.97f)
	{
		return true;
	}
	return false;

	if (tr.m_pEnt && baseEnt)
	{
		if (tr.m_pEnt->index == 0 || tr.allsolid)
			return false;
		if ((GEngine->GetPlayerInfo(tr.m_pEnt->index, &pinfo)
			|| baseEnt->index == tr.m_pEnt->index) && tr.fraction > 0.92)
			return true;
	*/
}
