#include "header.h"

int myPlayerIdx;
CBaseEntity* myPlayer;
myStruct uberStruct;

const byte __AIMBOT_KEY = VK_LBUTTON;
const float __AIMBOT_DIVIDE_BY = 2.1;
float const MAX_AIM_DISTANCE = 100;

VPANEL HackMechanics::mstp;

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

	static int w, h;
	core->g_pIPanel->GetSize(vguiPanel, w, h);

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

		myHack->getEyePosition(myPlayer, myEyes);

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

		draw->byType(player, type, distFromMe, isEnemy, a);

		if (RADAR_SWITCH && type != 3)
		{
			draw->onRadar(screenPos);
		}

		// aimbot &...
		if (isEnemy && type != 2)
		{
			if (type == 1)
			{
				static int randomBone;
				randomBone = rand() % 3 + 9;
				myHack->getBonePos(player, randomBone, enemyAimPosition);
			}
			else
			{
				myHack->getHead(player, enemyAimPosition);
			}

			if (myHack->w2s(Vector(enemyAimPosition[0], enemyAimPosition[1], enemyAimPosition[2]), hisHeadIn2D))
			{
				// vector subtraction
				deltaVector[0] = enemyAimPosition[0] - myEyes[0];
				deltaVector[1] = enemyAimPosition[1] - myEyes[1];
				deltaVector[2] = enemyAimPosition[2] - myEyes[2];
				core->VectorAngles(deltaVector, vectorAngle);
				//
				myEnemiesList[targetCursor] = TargetList_t(vectorAngle, myEyes, enemyAimPosition);
				//
				myEnemiesList[targetCursor].distance2D = (float)sqrt(
					pow(double(w / 2 - hisHeadIn2D.x), 2.0) +
					pow(double(h / 2 - hisHeadIn2D.y), 2.0));
				
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
			draw->statLn();
			draw->debug();

			//myHack->getHead(player, enemyAimPosition);
			//if (CoreHaxFunc::visibilityCheck(Vector(myEyes[0], myEyes[1], myEyes[2]), Vector(enemyAimPosition[0], enemyAimPosition[1], enemyAimPosition[2]), screenPos))
			//{
				/*
				swprintf_s(__DEBUG_BUFF_W, L"visible");
				core->g_pSurface->DrawSetTextPos(screenPos.x, screenPos.y);
				core->g_pSurface->DrawPrintText(__DEBUG_BUFF_W, wcslen(__DEBUG_BUFF_W));
				*/
			//}
		}
	}

	// aimbot..!
	if (targetCursor > 0)
	{
		uberStruct.aimAt[2] = 0;
		std::sort(myEnemiesList, myEnemiesList + targetCursor, CompareTargetEnArray2D());
		uberStruct.enemyDistance2D = myEnemiesList[0].distance2D;
		uberStruct.aimAt[0] = myEnemiesList[0].AimbotAngle[0];
		uberStruct.aimAt[1] = myEnemiesList[0].AimbotAngle[1];
		if (myEnemiesList[0].distance2D < MAX_AIM_DISTANCE)
		{
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

				if (AIMBOT_SWITCH && (uberStruct.aimAt[2] && GetAsyncKeyState(__AIMBOT_KEY) & 0x8000) || uberStruct.enemyDistance2D < 12)
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

	core->keyManager();

	if (!MAIN_SWITCH)
	{
		draw->off();
		if (__DEBUG)
		{
			__DEBUG = 0;
		}
		return;
	}

	if (CROSSHAIR_SWITCH)
	{
		draw->inTheMiddle(vguiPanel);
	}

	playersLoop(vguiPanel);
}