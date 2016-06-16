#include "header.h"

int myPlayerIdx;
CBaseEntity* myPlayer;
myStruct uberStruct;

// aimbot..!
const byte AIMBOT_PRESS_KEY = VK_LBUTTON;
float const AIMBOT_MAX_DISTANCE = 80;
float const AIMBOT_AUTO_DISTANCE = 12;

VPANEL HackMechanics::mstp;
int HackMechanics::mstpWidth;
int HackMechanics::mstpHeight;

void HackMechanics::playersLoop(VPANEL vguiPanel)
{
	static CBaseEntity* player;
	static Vector playerPos, screenPos, hisHeadIn2D;
	static playerInfo pInfo;
	static float distFromMe;
	static byte type, isEnemy, a;
 
	// aim part
	static float aimAngle[3], vectorAngle[3], deltaVector[3], myEyes[3], enemyAimPosition[3]; // ffs, do it with Vector
	static Vector vecEnemyAimPosition;
	int targetCursor = 0;
	TargetList* myEnemiesList = new TargetList[32];
	//
	//
	ExtendedPlayerClass* playerList[32];
	//

	for (int i = 0; i < core->g_pEntList->GetHighestEntityIndex(); i++)
	{
		player = core->g_pEntList->GetClientEntity(i);

		if (
			player == NULL // struct player {int lifeState, int team, int inventory}
			|| i == myPlayerIdx // take mem pointer in constructor
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

		// get player type
		type = 1;
		core->g_pEngine->GetPlayerInfo(i, &pInfo);
		if (strlen(pInfo.name) < 6)
		{
			type = 3;
		}
		if (*(int*)(DWORD64(player) + m_iHealth) > 700)
		{
			type = 2;
		}

		// is enemy?
		isEnemy = *(int*)(DWORD64(player) + m_iTeamNum) != *(int*)(DWORD64(myPlayer) + m_iTeamNum);

		distFromMe = myHack->getDist(myPlayer->GetAbsOrigin(), playerPos);
		if (distFromMe < 5000) // setDrawAlpha();
		{
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

		if (isEnemy) // setPlayerColor();
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
				myEnemiesList[targetCursor] = TargetList(vectorAngle, myEyes, enemyAimPosition);
				//
				myEnemiesList[targetCursor].distance2D = (float)sqrt(
					pow(double(mstpWidth / 2 - hisHeadIn2D.x), 2.0) +
					pow(double(mstpHeight / 2 - hisHeadIn2D.y), 2.0)
				);
				
				if (__DEBUG)
				{
					//swprintf_s(__DEBUG_BUFF_W, L"%.0f", myEnemiesList[targetCursor].distance2D);
					//core->g_pSurface->DrawSetTextPos(hisHeadIn2D.x, hisHeadIn2D.y);
					//core->g_pSurface->DrawPrintText(__DEBUG_BUFF_W, wcslen(__DEBUG_BUFF_W));
				}

				targetCursor++;
			}
		}
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

	// aimbot..!
	if (targetCursor > 0)
	{
		// pickNearestTargetIn2D();
		std::sort(myEnemiesList, myEnemiesList + targetCursor, CompareTargetsIn2D());
		uberStruct.enemyDistance2D = myEnemiesList[0].distance2D;
		uberStruct.aimAt[0] = myEnemiesList[0].AimbotAngle[0];
		uberStruct.aimAt[1] = myEnemiesList[0].AimbotAngle[1];
	}

	targetCursor = 0;
	delete[] myEnemiesList;
}


void __fastcall HackMechanics::Hooked_CreateMove(void* ptr, int sequence_number, float input_sample_frametime, bool active)
{
	static CInput* pInput;

	oCreateMove(ptr, sequence_number, input_sample_frametime, active);

	if (active == 1 && MAIN_SWITCH)
	{
		bool static once = 0; // findPointerToCInput();
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

				if (!AIMBOT_SWITCH) {
					return;
				}

				pCmd->viewangles.x += uberStruct.aimAt[0] - pCmd->viewangles.x;
				pCmd->viewangles.y += uberStruct.aimAt[1] - pCmd->viewangles.y;
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
		core->g_pIPanel->GetSize(vguiPanel, mstpWidth, mstpHeight);
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

	if (__DEBUG)
	{
		draw->statLn();
		draw->debug();
	}
}