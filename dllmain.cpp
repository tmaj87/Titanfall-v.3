#include "header.h"
#define M_RADPI 57.295779513082

tPaintTraverse oPaintTraverse;
tCreateMove oCreateMove;
cvmth64* VMThook;
cvmth64* VMThook2;
base* core;
hack* myHack;

bool __DEBUG = 0;
char __DEBUG_BUFF[512];
bool MAIN_SWITCH = 0;
bool CROSSHAIR_SWITCH = 1;
bool RADAR_SWITCH = 1;
bool NORECOIL_SWITCH = 0;

struct hckStruct_t {
	float aimAt[3];

	void fillData()
	{
		//
	}
} hckStruct;

int myPlayerIdx;
CBaseEntity* myPlayer;
myStruct uberStruct;
float aimAt[3] = {1., 2., 4.};





struct TargetList_t
{
	float Distance;
	float AimbotAngle[3];

	TargetList_t()
	{
	}

	TargetList_t(float aimbotAngle[], float myCoords[], float enemyCoords[])
	{
		Distance = Get3dDistance(myCoords[0], myCoords[1], myCoords[2],
			enemyCoords[0], enemyCoords[1], enemyCoords[2]);

		AimbotAngle[0] = aimbotAngle[0];
		AimbotAngle[1] = aimbotAngle[1];
		AimbotAngle[2] = aimbotAngle[2];
	}

	float Get3dDistance(float myCoordsX, float myCoordsZ, float myCoordsY,
		float eNx, float eNz, float eNy)
	{
		return sqrt(
			pow(double(eNx - myCoordsX), 2.0) +
			pow(double(eNy - myCoordsY), 2.0) +
			pow(double(eNz - myCoordsZ), 2.0));
	}
};




struct CompareTargetEnArray
{
	bool operator() (TargetList_t & lhs, TargetList_t & rhs)
	{
		return lhs.Distance < rhs.Distance;
	}
};










void CalcAngle(float* src, float* dst, float* angles)
{
	double delta[3] = { (src[0] - dst[0]), (src[1] - dst[1]), (src[2] - dst[2]) };
	double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);

	angles[0] = (float)(asin(delta[2] / hyp) * M_RADPI); // atan?
	angles[1] = (float)(atan(delta[1] / delta[0]) * M_RADPI);
	angles[2] = 0.0f;

	if (delta[0] >= 0.0) { angles[1] += 180.0f; }
}

MODULEINFO GetModuleInfo(char *szModule){
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

void WriteToMemory(uintptr_t addressToWrite, char* valueToWrite, int byteNum)
{
	//used to change our file access type, stores the old
	//access type and restores it after memory is written
	unsigned long OldProtection;
	//give that address read and write permissions and store the old permissions at oldProtection
	VirtualProtect((LPVOID)(addressToWrite), byteNum, PAGE_EXECUTE_READWRITE, &OldProtection);

	//write the memory into the program and overwrite previous value
	memcpy((LPVOID)addressToWrite, valueToWrite, byteNum);

	//reset the permissions of the address back to oldProtection after writting memory
	VirtualProtect((LPVOID)(addressToWrite), byteNum, OldProtection, NULL);
}


DWORD FindPattern(char *module, char *pattern, char *mask)
{
	//Get all module related information
	MODULEINFO mInfo = GetModuleInfo(module);

	//Assign our base and module size
	//Having the values right is ESSENTIAL, this makes sure
	//that we don't scan unwanted memory and leading our game to crash
	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	DWORD size = (DWORD)mInfo.SizeOfImage;

	//Get length for our mask, this will allow us to loop through our array
	DWORD patternLength = (DWORD)strlen(mask);

	for (DWORD i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (DWORD j = 0; j < patternLength; j++)
		{
			//if we have a ? in our mask then we have true by default, 
			//or if the bytes match then we keep searching until finding it or not
			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
		}

		//found = true, our entire pattern was found
		//return the memory addy so we can write to it
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
			"\x48\x8B\xC4\x48\x89\x58\x08\x48\x89\x70\x10\x57\x48\x81\xEC\x00\x00\x00\x00\xF3\x0F",
			"xxxxxxxxxxxxxxx????xx");
	}

	UTIL_TraceLine(start, end, mask, ignore, collisionGroup, unk, trace);
}



void keyManager()
{
	const byte MAIN_SWITCH_KEY = VK_F5;
	const byte NORECOIL_SWITCH_KEY = VK_F8;
	const byte RADAR_SWITCH_KEY = VK_F6;
	const byte __DEBUG_KEY = VK_F9;
	const byte CROSSHAIR_SWITCH_KEY = VK_F7;

	static bool keyBlock[5] = {1, 1, 1, 1, 1};

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




void __fastcall Hooked_CreateMove(void* ptr, int sequence_number, float input_sample_frametime, bool active)
{
	oCreateMove(ptr, sequence_number, input_sample_frametime, active);

	//sprintf_s(__DEBUG_BUFF, "sequence_number: %d, input_sample_frametime: %d, active: %d", sequence_number, input_sample_frametime, active);
	//core->debug->toFile(__DEBUG_BUFF);

	if (active == 1 && MAIN_SWITCH)
	{
		static CInput* pInput;
		static QAngle;

		int static once = 0;
		if (!once)
		{
			DWORD_PTR dwFunc1 = (DWORD_PTR)oCreateMove + 0x27;
			DWORD_PTR dwRenderHolder = (DWORD_PTR)((*(DWORD*)(dwFunc1 + 0x3)) + (dwFunc1 + 0x7)); // cut 4 bytes
			//DWORD_PTR dwRenderHolder = *((DWORD64*)(((DWORD64*)oCreateMove + 0x27) + 0x3));

			do {
				Sleep(50);
				pInput = *(CInput**)dwRenderHolder;
			} while (!pInput);

			once = 1;
		}

		if (pInput)
		{
			CUserCmd* pCmd = pInput->GetUserCmd(0, sequence_number);
			if (pCmd)
			{
				uberStruct.viewAngles = pCmd->viewangles;

				if (NORECOIL_SWITCH && aimAt[0] != 1. && aimAt[1] != 2. && aimAt[2] != 4.)
				{
					pCmd->viewangles.x = aimAt[0];
					pCmd->viewangles.y = aimAt[1];
					pCmd->viewangles.z = aimAt[2];
				}
				
			}
		}
	}
	
}

void __fastcall pt(IPanel* pThis, VPANEL vguiPanel, bool bForceRepaint, bool bAllowForce)
{
	oPaintTraverse(pThis, vguiPanel, bForceRepaint, bAllowForce);

	static CBaseEntity* player;
	static Vector playerPos, screenPos;
	static player_info_s pInfo;
	static float distFromMe;
	static byte type, isEnemy, a;
	static VPANEL mstp = NULL;

	if (mstp == NULL)
	{
		if (!strcmp(pThis->GetName(vguiPanel), "MatSystemTopPanel") && pThis->GetParent(vguiPanel) == 0)
		{
			mstp = vguiPanel;
		}
	}

	keyManager();

	if (vguiPanel == mstp)
	{
		if (!MAIN_SWITCH)
		{
			myHack->drawOff();
			if (__DEBUG)
			{
				__DEBUG = 0;
			}
			return;
		}

		// about me
		myPlayerIdx = core->g_pEngine->GetLocalPlayer();
		myPlayer = core->g_pEntList->GetClientEntity(myPlayerIdx);
		if (
			myPlayer == NULL
			|| *(int*)(DWORD64(myPlayer) + m_lifeState) != 0
			)
		{
			return;
		}

		// TO BE ADDED
		if (*(int*)(DWORD64(myPlayer) + m_iMaxHealth) == 5000)
		{
			// EJECT, EJECT, EJECT
		}

		if (CROSSHAIR_SWITCH)
		{
			myHack->inTheMiddle(vguiPanel);
		}

		int targetLoop = 0;
		TargetList_t* TargetList = new TargetList_t[core->g_pEntList->GetHighestEntityIndex()];

		for (int i = 0; i < core->g_pEntList->GetHighestEntityIndex(); i++)
		{
			player = core->g_pEntList->GetClientEntity(i);

			if (
				player == NULL
				|| i == myPlayerIdx
				|| *(int*)(DWORD64(player) + m_lifeState) != 0
				|| *(int*)(DWORD64(player) + m_iTeamNum) == 0
				|| *(int*)(DWORD64(player) + m_inventory) == 0
				)
			{
				continue;
			}

			static bool matrixCached = 0;
			if (!matrixCached)
			{
				myHack->getMatrix(); // magic!!
				matrixCached = 1;
			}

			playerPos = player->GetAbsOrigin();
			if (!myHack->w2s(playerPos, screenPos))
			{
				continue;
			}

			distFromMe = myHack->getDist(myPlayer->GetAbsOrigin(), playerPos);
			if (distFromMe < 5000)
			{
				// set alpha
				a = 255 - distFromMe / 20;
				if (a < 60)
				{
					a = 60;
				}
			}
			else
			{
				a = 0;
			}

			type = 1;
			// GET CLASS / MODEL NAME??????????????????????
			// GET CLASS / MODEL NAME??????????????????????
			// GET CLASS / MODEL NAME??????????????????????
			core->g_pEngine->GetPlayerInfo(i, &pInfo);
			if (strlen(pInfo.szName) < 6)
			{
				type = 3;
			}

			// get player type
			if (*(int*)(DWORD64(player) + m_iHealth) > 700)
			{
				type = 2;
			}

			// is enemy?
			isEnemy = *(int*)(DWORD64(player) + m_iTeamNum) != *(int*)(DWORD64(myPlayer) + m_iTeamNum);

			// set color
			if (isEnemy)
			{
				core->g_pSurface->DrawSetColor(255, 0, 0, a);
			}
			else
			{
				core->g_pSurface->DrawSetColor(0, 255, 0, 40);
			}

			switch (type)
			{
			case 1: // player
				myHack->drawPlayer(player, distFromMe, isEnemy);

				float aimAngle[3];

				// aimbot
				CalcAngle(myHack->getHead(myPlayer), myHack->getHead(player), aimAngle);

				TargetList[targetLoop] = TargetList_t(aimAngle, myHack->getHead(myPlayer), myHack->getHead(player));
				targetLoop++;


				break;
			case 2: // titan
				if (isEnemy)
				{
					core->g_pSurface->DrawSetColor(255, 153, 0, a);
				}
				myHack->drawTitan(player, distFromMe, isEnemy);
				break;
			case 3: // minon
				if (!isEnemy || *(int*)(DWORD64(player) + m_hGroundEntity) == -1) // remove ally and weapons on ground
				{
					continue;
				}
				core->g_pSurface->DrawSetColor(255, 255, 0, a);
				myHack->drawMinion(player, distFromMe, isEnemy);
				break;
			}

			if (RADAR_SWITCH && type != 3 && isEnemy)
			{
				myHack->drawOnRadar(screenPos);
			}

			if (__DEBUG)
			{
				myHack->drawStatLn();
				myHack->drawDebug();

				/*
				Trace trc;

				traceLine(myPlayer->GetAbsOrigin(), player->GetAbsOrigin(), 0x4600400B, myPlayer, 0, 0, &trc); // ..3?

				if (trc.fraction == 1.0f)
				{
					static wchar_t buff[512];

					swprintf_s(buff, L"visible");
					core->g_pSurface->DrawSetTextPos(screenPos.x, screenPos.y);
					core->g_pSurface->DrawPrintText(buff, wcslen(buff));
				}
				*/
			}
		}

		if (NORECOIL_SWITCH)
		{
			//QAngle Punch = *(QAngle*)(myPlayer + m_local + m_vecPunchBase_AngleVel);
			if (targetLoop > 0)
			{
				std::sort(TargetList, TargetList + targetLoop, CompareTargetEnArray());

				if (TargetList[0].Distance < 4)
				{
					aimAt[0] = TargetList[0].AimbotAngle[0];
					aimAt[1] = TargetList[0].AimbotAngle[1];
					aimAt[2] = TargetList[0].AimbotAngle[2];
				}
				else
				{
					aimAt[0] = 1.;
					aimAt[1] = 2.;
					aimAt[2] = 4.;
				}
			}
			targetLoop = 0;
			delete[] TargetList;
		}
	}
}

void DoMeAFavour()
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