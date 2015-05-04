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
wchar_t __DEBUG_BUFF_W[512];
bool MAIN_SWITCH = 0;
bool CROSSHAIR_SWITCH = 1;
bool RADAR_SWITCH = 1;
bool NORECOIL_SWITCH = 1;

int myPlayerIdx;
CBaseEntity* myPlayer;
myStruct uberStruct;
float aimAt[3] = {0, 0, 0};
float bufferedAim[3] = { 0, 0, 0 };

float const MAX_AIM_DISTANCE = 8;
float const MIN_AIM_DISTANCE = 30;
float const CRITICAL_3D_DISTANCE = 150;
float const MAX_3D_DISTANCE = 500;



struct TargetList_t
{
	float distance3D;
	float distance2D;
	float AimbotAngle[3];
	//float oEnemyCoords[3];
	//float oMyCoords[3];

	TargetList_t()
	{
	}

	TargetList_t(float aimbotAngle[], float myCoords[], float enemyCoords[])
	{
		//oEnemyCoords = enemyCoords;
		//oMyCoords = myCoords;

		AimbotAngle[0] = aimbotAngle[0];
		AimbotAngle[1] = aimbotAngle[1];
		AimbotAngle[2] = aimbotAngle[2];

		distance3D = Get3dDistance(myCoords[0], myCoords[1], myCoords[2], enemyCoords[0], enemyCoords[1], enemyCoords[2]);
		distance2D = Get2dDistance();
	}

	float Get3dDistance(float myCoordsX, float myCoordsZ, float myCoordsY,
		float eNx, float eNz, float eNy)
	{
		return sqrt(
			pow(double(eNx - myCoordsX), 2.0) +
			pow(double(eNy - myCoordsY), 2.0) +
			pow(double(eNz - myCoordsZ), 2.0));
	}

	float Get2dDistance()
	{
		return sqrt(
			pow(double(AimbotAngle[0] - uberStruct.viewAngles.x), 2.0) +
			pow(double(AimbotAngle[1] - uberStruct.viewAngles.y), 2.0));	
	}
};




struct CompareTargetEnArray2D
{
	bool operator() (TargetList_t & lhs, TargetList_t & rhs)
	{
		return lhs.distance2D < rhs.distance2D;
	}
};

struct CompareTargetEnArray3D
{
	bool operator() (TargetList_t & lhs, TargetList_t & rhs)
	{
		return lhs.distance3D < rhs.distance3D;
	}
};









void CalcAngle(float* src, float* dst, float* angles)
{
	double delta[3] = { (src[0] - dst[0]), (src[1] - dst[1]), (src[2] - dst[2]) };
	double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);


	//double distance, yaw, pitch;
	//distance = sqrt(pow(delta[0], 2) + pow(delta[1], 2) + pow(delta[2], 2));
	//yaw = atan(delta[1] / delta[0]);
	//pitch = acos(delta[2] / distance);


	angles[0] = (float)(asin(delta[2] / hyp) * M_RADPI);  //(float)atan2(delta[2], delta[0]); // yaw; //  // yaw
	angles[1] = (float)(atan(delta[1] / delta[0]) * M_RADPI); // (float)atan2(hyp, delta[1]) + M_RADPI; //  pitch; // (float)(atan(delta[1] / delta[0]) * M_RADPI); // pitch
	angles[2] = 0.0f;

	if (delta[0] >= 0.0) { angles[1] += 180.0f; }
}


void VectorAngles(const float *forward, float *angles)
{
	//Assert(s_bMathlibInitialized);
	float	tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_RADPI); // M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_RADPI); // M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}




void __fastcall Hooked_CreateMove(void* ptr, int sequence_number, float input_sample_frametime, bool active)
{
	oCreateMove(ptr, sequence_number, input_sample_frametime, active);
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
				//QAngle Punch = *(QAngle*)(myPlayer + m_local + m_vecPunchBase_AngleVel);
				uberStruct.viewAngles = pCmd->viewangles;
				if (NORECOIL_SWITCH && aimAt[2] && GetAsyncKeyState(VK_MENU) & 0x8000) // ALT KEY // && sequence_number%2
				{
					// buffered aim;
					pCmd->viewangles.x = aimAt[0];
					pCmd->viewangles.y = aimAt[1];
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

	CoreHaxFunc::keyManager();

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

		// 
		// aim part
		//
		int targetLoop = 0;
		TargetList_t* TargetList = new TargetList_t[64];

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
			if (__DEBUG)
			{
			}
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

				if (isEnemy)
				{
					static float aimAngle[3], enemyHeadPosition[3];
					static float* myHeadPosition;
					//myHack->getBonePos(myPlayer, 7, myHeadPosition);
					myHeadPosition = myHack->getEyePosition(myPlayer);
					//myHack->getHead(myPlayer, myHeadPosition);
					//myHack->getHead(player, enemyHeadPosition);
					myHack->getBonePos(player, 10, enemyHeadPosition);

					Vector movePunch = *(Vector*)(myPlayer + m_local + m_vecPunchBase_Angle);
					Vector punchVec = *(Vector*)(myPlayer + m_local + m_vecPunchWeapon_Angle);

					enemyHeadPosition[0] -= punchVec[0]; // -movePunch[0];
					enemyHeadPosition[1] -= punchVec[1]; // -movePunch[1];
					enemyHeadPosition[2] -= punchVec[2]; // -movePunch[2];

					//myHack->getBonePos(player, 7, enemyHeadPosition);

					CalcAngle(myHeadPosition, enemyHeadPosition, aimAngle); // 5, 6, 7
					VectorAngles(myHeadPosition, enemyHeadPosition);
					TargetList[targetLoop] = TargetList_t(aimAngle, myHeadPosition, enemyHeadPosition);


					if (__DEBUG)
					{
						//myHack->drawAllBones(player, 0, 14);
						swprintf_s(__DEBUG_BUFF_W, L"%.2f", TargetList[targetLoop].distance2D);
						core->g_pSurface->DrawSetTextPos(screenPos.x, screenPos.y);
						core->g_pSurface->DrawPrintText(__DEBUG_BUFF_W, wcslen(__DEBUG_BUFF_W));
					}

					targetLoop++;
				}
				
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

				static float aimAngle[3], enemyHeadPosition[3];
				static float* myHeadPosition;

				//myHack->getBonePos(myPlayer, 7, myHeadPosition);
				myHeadPosition = myHack->getEyePosition(myPlayer);
				//myHack->getHead(myPlayer, myHeadPosition);
				//myHack->getHead(player, enemyHeadPosition);
				myHack->getBonePos(player, 10, enemyHeadPosition);
				CalcAngle(myHeadPosition, enemyHeadPosition, aimAngle);
				TargetList[targetLoop] = TargetList_t(aimAngle, myHeadPosition, enemyHeadPosition);
				targetLoop++;

				core->g_pSurface->DrawSetColor(255, 255, 0, a);
				myHack->drawMinion(player, distFromMe, isEnemy);
				break;
			}

			if (RADAR_SWITCH && type != 3) //  && isEnemy)
			{
				myHack->drawOnRadar(screenPos);
			}

			if (__DEBUG)
			{
				myHack->drawStatLn();
				myHack->drawDebug();
			}
		}



		// aimbot..!
		TargetList_t *cpyOfTrgtLst3D, *cpyOfTrgtLst2D;
		if (targetLoop > 0)
		{
			cpyOfTrgtLst3D = TargetList;
			cpyOfTrgtLst2D = TargetList;
			std::sort(cpyOfTrgtLst2D, TargetList + targetLoop, CompareTargetEnArray2D());
			std::sort(cpyOfTrgtLst3D, TargetList + targetLoop, CompareTargetEnArray3D());

			if (cpyOfTrgtLst2D[0].distance2D < MAX_AIM_DISTANCE)
			{
				aimAt[0] = cpyOfTrgtLst2D[0].AimbotAngle[0];
				aimAt[1] = cpyOfTrgtLst2D[0].AimbotAngle[1];
				aimAt[2] = 1;
			}
			else
			{
				aimAt[2] = 0;
			}
		}

		if (NORECOIL_SWITCH) //  && GetAsyncKeyState(VK_LBUTTON)) // && Plat_FloatTime() % 2
		{
			//cpyOfTrgtLst2D[0].
			//myHack->drawCrossAt(x, y);
		}

		targetLoop = 0;
		delete[] TargetList;
	}
}






BOOL APIENTRY DllMain(HMODULE handle, DWORD  reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(handle);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CoreHaxFunc::DoMeAFavour(), NULL, 0, NULL);
		break;
	}
	return 1;
}