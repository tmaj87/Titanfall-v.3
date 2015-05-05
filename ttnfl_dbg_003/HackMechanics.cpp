#include "header.h"

int myPlayerIdx;
CBaseEntity* myPlayer;
myStruct uberStruct;
float aimAt[3] = { 0, 0, 0 };
float bufferedAim[3] = { 0, 0, 0 };

float const MAX_AIM_DISTANCE = 8;
// float const MIN_AIM_DISTANCE = 30;
float const CRITICAL_3D_DISTANCE = 200;
// float const MAX_3D_DISTANCE = 500;

VPANEL HackMechanics::mstp;

HackMechanics::HackMechanics()
{
}


HackMechanics::~HackMechanics()
{
}

void HackMechanics::aimAtThisPlayer(CBaseEntity* player)
{
	//
}

void __fastcall HackMechanics::Hooked_CreateMove(void* ptr, int sequence_number, float input_sample_frametime, bool active)
{
	oCreateMove(ptr, sequence_number, input_sample_frametime, active);
	if (active == 1 && MAIN_SWITCH)
	{
		static CInput* pInput;

		int static once = 0;
		if (!once)
		{
			DWORD_PTR dwFunc1 = (DWORD_PTR)oCreateMove + 0x27;
			DWORD_PTR dwRenderHolder = (DWORD_PTR)((*(DWORD*)(dwFunc1 + 0x3)) + (dwFunc1 + 0x7)); // cut 4 bytes

			do {
				Sleep(50);
				pInput = *(CInput**)dwRenderHolder; // wrong name
			} while (!pInput);

			once = 1;
		}

		if (pInput)
		{
			CUserCmd* pCmd = pInput->GetUserCmd(0, sequence_number);
			if (pCmd)
			{
				uberStruct.viewAngles = pCmd->viewangles;
				if (NORECOIL_SWITCH && aimAt[2] && GetAsyncKeyState(VK_MENU) & 0x8000)
				{
					// buffered aim
					// http://www.unknowncheats.me/forum/counterstrike-global-offensive/137492-math-behind-hack-1-coding-better-aimbot-stop-using-calcangle.html
					static float smoothAim[2];
					smoothAim[0] = aimAt[0] - pCmd->viewangles.x;
					smoothAim[1] = aimAt[1] - pCmd->viewangles.y;
					//
					static int divideBy = 4;
					//
					pCmd->viewangles.x += smoothAim[0] / divideBy;
					pCmd->viewangles.y += smoothAim[1] / divideBy;
				}

			}
		}
	}

}

bool HackMechanics::isAboutMeAvailable()
{
	myPlayerIdx = core->g_pEngine->GetLocalPlayer();
	myPlayer = core->g_pEntList->GetClientEntity(myPlayerIdx);
	if (
		myPlayer == NULL
		|| *(int*)(DWORD64(myPlayer) + m_lifeState) != 0
		)
	{
		return false;
	}
	return true;
}

void HackMechanics::initPanel(IPanel* pThis, VPANEL vguiPanel)
{
	if (!strcmp(pThis->GetName(vguiPanel), "MatSystemTopPanel") && pThis->GetParent(vguiPanel) == 0)
	{
		mstp = vguiPanel;
	}
}


bool HackMechanics::isCorrectPanel(IPanel* pThis, VPANEL vguiPanel)
{
	if (mstp == NULL)
	{
		initPanel(pThis, vguiPanel);
	}

	if (vguiPanel == mstp)
	{
		return true;
	}
	return false;
}


void __fastcall HackMechanics::pt(IPanel* pThis, VPANEL vguiPanel, bool bForceRepaint, bool bAllowForce)
{
	oPaintTraverse(pThis, vguiPanel, bForceRepaint, bAllowForce);

	static CBaseEntity* player;
	static Vector playerPos, screenPos;
	static player_info_s pInfo;
	static float distFromMe;
	static byte type, isEnemy, a;

	if (!isCorrectPanel(pThis, vguiPanel) || !isAboutMeAvailable())
	{
		return;
	}

	CoreHaxFunc::keyManager();

	if (!MAIN_SWITCH)
	{
		myHack->drawOff();
		if (__DEBUG)
		{
			__DEBUG = 0;
		}
		return;
	}

	if (CROSSHAIR_SWITCH)
	{
		myHack->inTheMiddle(vguiPanel);
	}

	// 
	// aim part
	//
	int targetLoop = 0;
	TargetList_t* TargetList = new TargetList_t[32];


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

		// get player type
		type = 1;
		core->g_pEngine->GetPlayerInfo(i, &pInfo);
		if (strlen(pInfo.szName) < 6)
		{
			type = 3;
		}
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

		drawByType(player, type, distFromMe, isEnemy, a);

		if (RADAR_SWITCH && type != 3)
		{
			myHack->drawOnRadar(screenPos);
		}

		if (__DEBUG)
		{
			myHack->drawStatLn();
			myHack->drawDebug();
		}

		// aimbot
		if (isEnemy)
		{
			static float aimAngle[3], enemyHeadPosition[3];
			static float* myHeadPosition;

			myHeadPosition = myHack->getEyePosition(myPlayer);
			myHack->getBonePos(player, 10, enemyHeadPosition);

			Vector punchVec = *(Vector*)(myPlayer + m_local + m_vecPunchWeapon_Angle);

			CoreHaxFunc::CalcAngle(myHeadPosition, enemyHeadPosition, aimAngle);
			//VectorAngles(myHeadPosition, enemyHeadPosition);

			TargetList[targetLoop] = TargetList_t(aimAngle, myHeadPosition, enemyHeadPosition);

			targetLoop++;
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

		if (
				cpyOfTrgtLst2D[0].distance2D < MAX_AIM_DISTANCE
				|| cpyOfTrgtLst3D[0].distance3D < CRITICAL_3D_DISTANCE
			)
		{
			aimAt[0] = cpyOfTrgtLst2D[0].AimbotAngle[0];
			aimAt[1] = cpyOfTrgtLst2D[0].AimbotAngle[1];
			if (cpyOfTrgtLst3D[0].distance3D < CRITICAL_3D_DISTANCE)
			{
				aimAt[0] = cpyOfTrgtLst3D[0].AimbotAngle[0];
				aimAt[1] = cpyOfTrgtLst3D[0].AimbotAngle[1];
			}

			aimAt[2] = 1;
		}
		else
		{
			aimAt[2] = 0;
		}
	}

	targetLoop = 0;
	delete[] TargetList;
}

void HackMechanics::drawByType(CBaseEntity* player, byte type, float distFromMe, byte isEnemy, byte alpha)
{
	switch (type)
	{
	case 1: // player
		myHack->drawPlayer(player, distFromMe, isEnemy);
		break;
	case 2: // titan
		if (isEnemy)
		{
			core->g_pSurface->DrawSetColor(255, 153, 0, alpha);
		}
		myHack->drawTitan(player, distFromMe, isEnemy);
		break;
	case 3: // minon
		if (!isEnemy || *(int*)(DWORD64(player) + m_hGroundEntity) == -1) // remove ally and weapons on ground
		{
			return;
		}
		core->g_pSurface->DrawSetColor(255, 255, 0, alpha);
		myHack->drawMinion(player, distFromMe, isEnemy);
		break;
	}
}