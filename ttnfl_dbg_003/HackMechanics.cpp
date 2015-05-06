#include "header.h"

int myPlayerIdx;
CBaseEntity* myPlayer;
myStruct uberStruct;

const byte __AIMBOT_KEY = VK_LBUTTON;
const float __AIMBOT_DIVIDE_BY = 3.1;
float const MAX_AIM_DISTANCE = 120;

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

void HackMechanics::playersLoop(VPANEL vguiPanel)
{
	static CBaseEntity* player;
	static Vector playerPos, screenPos, hisHeadIn2D;
	static player_info_s pInfo;
	static float distFromMe;
	static byte type, isEnemy, a;
 
	// aim part
	static float aimAngle[3], vectorAngle[3], deltaVector[3], myEyes[3], enemyAimPosition[3];
	static Vector vecEnemyAimPosition;
	int targetCursor = 0;
	static int TARGET_ARRAY_SIZE = 32;
	TargetList_t* myEnemiesList = new TargetList_t[32];
	//

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

		// aimbot &...
		if (isEnemy)
		{
			myHack->getEyePosition(myPlayer, myEyes);

			static int randomBone;
			randomBone = rand() % 3 + 9;

			if (type == 2)
			{
				myHack->getHead(player, enemyAimPosition);
			}
			else
			{
				myHack->getBonePos(player, randomBone, enemyAimPosition);
			}

			if (myHack->w2s(Vector(enemyAimPosition[0], enemyAimPosition[1], enemyAimPosition[2]), hisHeadIn2D))
			{
				// vector substraction
				deltaVector[0] = enemyAimPosition[0] - myEyes[0];
				deltaVector[1] = enemyAimPosition[1] - myEyes[1];
				deltaVector[2] = enemyAimPosition[2] - myEyes[2];
				CoreHaxFunc::VectorAngles(deltaVector, vectorAngle);
				//
				myEnemiesList[targetCursor] = TargetList_t(vectorAngle, myEyes, enemyAimPosition);
				//
				static int w, h;
				core->g_pIPanel->GetSize(vguiPanel, w, h);
				myEnemiesList[targetCursor].distance2D = (float)sqrt(
					pow(double(w / 2 - hisHeadIn2D.x), 2.0) +
					pow(double(h / 2 - hisHeadIn2D.y), 2.0));
				//
				////////////////////////////////////////////////////

				if (__DEBUG)
				{
					swprintf_s(__DEBUG_BUFF_W, L"%.0f", myEnemiesList[targetCursor].distance2D);
					core->g_pSurface->DrawSetTextPos(hisHeadIn2D.x, hisHeadIn2D.y);
					core->g_pSurface->DrawPrintText(__DEBUG_BUFF_W, wcslen(__DEBUG_BUFF_W));
				}

				targetCursor++;
			}
		}

		if (__DEBUG)
		{
			myHack->drawStatLn();
			myHack->drawDebug();
		}
	}

	// aimbot..!
	if (targetCursor > 0)
	{
		uberStruct.aimAt[2] = 0;
		std::sort(myEnemiesList, myEnemiesList + targetCursor, CompareTargetEnArray2D());
		if (myEnemiesList[0].distance2D < MAX_AIM_DISTANCE)
		{
			uberStruct.aimAt[0] = myEnemiesList[0].AimbotAngle[0];
			uberStruct.aimAt[1] = myEnemiesList[0].AimbotAngle[1];
			uberStruct.aimAt[2] = 1;
		}
	}

	targetCursor = 0;
	delete[] myEnemiesList;
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
				uberStruct.bufferedAngles.x = (uberStruct.aimAt[0] - pCmd->viewangles.x) / __AIMBOT_DIVIDE_BY;
				uberStruct.bufferedAngles.y = (uberStruct.aimAt[1] - pCmd->viewangles.y) / __AIMBOT_DIVIDE_BY;

				if (NORECOIL_SWITCH && uberStruct.aimAt[2] && GetAsyncKeyState(__AIMBOT_KEY) & 0x8000)
				{
					pCmd->viewangles.x += uberStruct.bufferedAngles.x;
					pCmd->viewangles.y += uberStruct.bufferedAngles.y;
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

	playersLoop(vguiPanel);
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