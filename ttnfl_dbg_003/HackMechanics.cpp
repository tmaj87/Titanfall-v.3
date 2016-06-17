#include "header.h"

int myPlayerIndex;
Player player;
Player myPlayer;
myStruct uberStruct;

Vector screenPos, hisHeadIn2D;

byte a;

// aimbot..!
const byte AIMBOT_PRESS_KEY = VK_LBUTTON;
float const AIMBOT_MAX_DISTANCE = 80;
float const AIMBOT_AUTO_DISTANCE = 12;

VPANEL HackMechanics::matSystemTopPanel;
int HackMechanics::matSystemTopPanelWidth;
int HackMechanics::matSystemTopPanelHeight;

void HackMechanics::playersLoop(VPANEL vguiPanel)
{
	// aim part
	static float aimAngle[3], vectorAngle[3]; // ffs, do it with Vector
	static Vector enemyAimPosition, deltaVec;
	int targetCursor = 0;
	TargetList* myEnemiesList = new TargetList[32];


	for (int i = 0; i < core->g_pEntList->GetHighestEntityIndex(); i++)
	{
		player = Player(i);

		if (i == myPlayerIndex || !player.check())
		{
			continue;
		}


		static bool matrixCached = 0;
		if (!matrixCached)
		{
			myHack->getMatrix(); // magic!!
			matrixCached = 1;
		}

		if (!myHack->worldToScreen(player.position, screenPos))
		{
			continue;
		}

		if (player.distanceFromMe < 5000) // setDrawAlpha();
		{
			a = 255 - player.distanceFromMe / 20;
			if (a < 60)
			{
				a = 60;
			}
		}
		else
		{
			a = 0;
		}

		if (player.enemy) // setPlayerColor();
		{
			core->g_pSurface->DrawSetColor(255, 0, 0, a);
		}
		else
		{
			core->g_pSurface->DrawSetColor(0, 255, 0, 40);
		}

		draw->drawMarkBasedOnType(player.player, player.type, player.distanceFromMe, player.enemy, a);

		if (RADAR_SWITCH && player.type != MINION)
		{
			draw->onRadar(screenPos);
		}

		// aimbot &...
		if (player.enemy && player.type != TITAN)
		{
			if (player.type == PLAYER)
			{
				static int randomBone;
				randomBone = rand() % 3 + 9;
				myHack->getBonePos(player.player, randomBone, &enemyAimPosition); // arg0: plyr.position?
			}
			else
			{
				myHack->getHead(player.player, &enemyAimPosition); // arg0: plyr.position?
			}

			if (myHack->worldToScreen(enemyAimPosition, hisHeadIn2D))
			{
				deltaVec = enemyAimPosition - myPlayer.eyesPositon;
				float deltaFlo[3] = {deltaVec[0], deltaVec[1], deltaVec[2]};
				core->VectorAngles(deltaFlo, vectorAngle);

				myEnemiesList[targetCursor] = TargetList(vectorAngle);
				myEnemiesList[targetCursor].distance3D = player.distanceFromMe
				myEnemiesList[targetCursor].distance2D = (float)sqrt( // refactor
					pow(double(matSystemTopPanelWidth / 2 - hisHeadIn2D.x), 2.0) +
					pow(double(matSystemTopPanelHeight / 2 - hisHeadIn2D.y), 2.0)
				);

				targetCursor++;
			}
		}

	}

	// aimbot..!
	if (targetCursor > 0)
	{
		// pickNearestTargetIn2D();
		std::sort(myEnemiesList, myEnemiesList + targetCursor, CompareTargetsIn2D());
		uberStruct.enemyDistance2D = myEnemiesList[0].distance2D;
		uberStruct.aimAt[0] = myEnemiesList[0].aimAngle[0];
		uberStruct.aimAt[1] = myEnemiesList[0].aimAngle[1];
	}

	targetCursor = 0;
	delete[] myEnemiesList;
}


void __fastcall HackMechanics::cm(void* ptr, int sequence_number, float input_sample_frametime, bool active)
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
	myPlayerIndex = core->g_pEngine->GetLocalPlayer();
	myPlayer = Player(myPlayerIndex);

	return  myPlayer.check();
}

void HackMechanics::initPanel(IPanel* pThis, VPANEL vguiPanel)
{
	if (!strcmp(pThis->GetName(vguiPanel), "MatSystemTopPanel") && pThis->GetParent(vguiPanel) == 0)
	{
		matSystemTopPanel = vguiPanel;
		core->g_pIPanel->GetSize(vguiPanel, matSystemTopPanelWidth, matSystemTopPanelHeight);
	}
}


bool HackMechanics::isCorrectPanel(IPanel* pThis, VPANEL vguiPanel)
{
	if (matSystemTopPanel == NULL)
	{
		initPanel(pThis, vguiPanel);
	}

	if (vguiPanel == matSystemTopPanel)
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