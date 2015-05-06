#include "header.h"

tPlat_FloatTime Plat_FloatTime;

base::base()
{
	debug = new dbg();
	debug->cleanFile();
	initFaces();
}

void base::initFaces()
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
	g_pTrace = (IEngineTrace*)g_pEngineFactory("g_pTraceClient004", NULL);
	g_pEntList = (CEntList*)g_ClientFactory("VClientEntityList003", NULL);
	g_pSurface = (ISurface*)g_VGUIFactory("VGUI_Surface031", NULL);
	g_pIPanel = (IPanel*)g_VGUI2Factory("VGUI_Panel009", NULL);
	//g_pDebug = (IVDebugOverlay*)g_pEngineFactory("VDebugOverlay004", NULL);
	g_pClient = (IBaseClientDLL*)g_ClientFactory("VClient018", NULL);
	Plat_FloatTime = (tPlat_FloatTime)GetProcAddress(hTier0, "Plat_FloatTime");

	if (0) // __DEBUG
	{
		//char buff[64] = { 0 };
		//sprintf_s(buff, "g_pTrace: %p\n", g_pTrace);
		//debug->toFile(buff);
		//sprintf_s(buff, "g_pSurface: %p\n", g_pSurface);
		//debug->toFile(buff);
	}
}


float hack::getDist(Vector& myCoords, Vector& enemyCoords)
{
	return (float)sqrt(
		pow(enemyCoords[0] - myCoords[0], 2.0) +
		pow(enemyCoords[1] - myCoords[1], 2.0) +
		pow(enemyCoords[2] - myCoords[2], 2.0));
}

bool hack::w2s(Vector &vOrigin, Vector &vScreen) {
	const matrix3x4& worldToScreen = *m_vMatrix;

	float w = worldToScreen[3][0] * vOrigin[0] + worldToScreen[3][1] * vOrigin[1] + worldToScreen[3][2] * vOrigin[2] + worldToScreen[3][3]; //Calculate the angle in compareson to the player's camera.
	vScreen.z = 0;

	if (w > .001)
	{
		int wScreen, hScreen;
		core->g_pEngine->GetScreenSize(wScreen, hScreen);

		float fl1DBw = 1 / w;

		vScreen.x = int((wScreen / 2) + (0.5 * ((worldToScreen[0][0] * vOrigin[0] + worldToScreen[0][1] * vOrigin[1] + worldToScreen[0][2] * vOrigin[2] + worldToScreen[0][3]) * fl1DBw) * wScreen + 0.5));
		vScreen.y = int((hScreen / 2) - (0.5 * ((worldToScreen[1][0] * vOrigin[0] + worldToScreen[1][1] * vOrigin[1] + worldToScreen[1][2] * vOrigin[2] + worldToScreen[1][3]) * fl1DBw) * hScreen + 0.5));
		return 1;
	}

	return 0;
}

void hack::inTheMiddle(VPANEL vguiPanel)
{
	static int w, h;
	core->g_pIPanel->GetSize(vguiPanel, w, h);
	core->g_pSurface->DrawSetColor(255, 20, 147, 160);
	core->g_pSurface->DrawLine(w / 2 + 5, h / 2 - 2, w / 2 + 5, h / 2 + 2);
	core->g_pSurface->DrawLine(w / 2 - 5, h / 2 - 2, w / 2 - 5, h / 2 + 2);
	core->g_pSurface->DrawLine(w / 2 - 2, h / 2 - 5, w / 2 + 2, h / 2 - 5);
	core->g_pSurface->DrawLine(w / 2 - 2, h / 2 + 5, w / 2 + 2, h / 2 + 5);
}

void hack::drawOff()
{
	const wchar_t offStr[] = L"off";
	core->g_pSurface->DrawSetTextFont(0);
	core->g_pSurface->DrawSetTextColor(255, 0, 0, 200);
	core->g_pSurface->DrawSetTextPos(10, 10);
	core->g_pSurface->DrawPrintText(offStr, wcslen(offStr));
}

void hack::getMatrix()
{
	cvmth64* clientHook = new cvmth64;

	if (!clientHook->bInitialize((PDWORD64*)core->g_pClient))
	{
		//
	}

	DWORD_PTR dwFunc1 = (DWORD_PTR)clientHook->dwGetMethodAddress(74) + 0x6;
	DWORD_PTR dwRenderHolder = (DWORD_PTR)((*(DWORD*)(dwFunc1 + 0x3)) + (dwFunc1 + 0x7)); // cut 4 bytes

	CViewRender* pRender;
	do {
		Sleep(50);
		pRender = *(CViewRender**)dwRenderHolder;
	} while (!pRender);

	m_vMatrix = pRender->GetWorldToScreenMatrix();
}


void hack::getHead(CBaseEntity* player, float* posToWrite)
{
	static matrix3x4 boneList[128];
	static int boneId = 12;

	if (player->SetupBones(boneList, 128, 256, Plat_FloatTime()))
	{
		if (boneList[11][2][3] > boneList[12][2][3])
		{
			boneId = 11;
		}
		posToWrite[0] = boneList[boneId][0][3];
		posToWrite[1] = boneList[boneId][1][3];
		posToWrite[2] = boneList[boneId][2][3];
	}
}

// gets 128 bones...
void hack::getBonePos(CBaseEntity* player, int boneId, float* posToWrite)
{
	static matrix3x4 boneList[128];

	if (player->SetupBones(boneList, 128, 256, Plat_FloatTime()))
	{
		posToWrite[0] = boneList[boneId][0][3];
		posToWrite[1] = boneList[boneId][1][3];
		posToWrite[2] = boneList[boneId][2][3];
	}
}

void hack::drawPlayer(CBaseEntity* player, float distance, byte isEnemy)
{
	const int VECTOR_SIZE_ADDON = 8;
	const int MIN_CROSSHAIR_SIZE = 6;
	const int MIN_CROSSHAIR_ALPHA = 60;
	static usedToDrawStruct s;

	static int iSize;
	static byte alpha;

	// new distance system
	distance /= 10;
	alpha = 255 - distance / 2;

	if (player->SetupBones(s.boneList, 128, 256, Plat_FloatTime()))
	{
		if (s.boneList[11][2][3] > s.boneList[12][2][3]) // head bones
		{
			s.playerPos = Vector(s.boneList[11][0][3], s.boneList[11][1][3], s.boneList[11][2][3]);
		}
		else
		{
			s.playerPos = Vector(s.boneList[12][0][3], s.boneList[12][1][3], s.boneList[12][2][3]);
		}
		s.boneTop = s.playerPos + Vector(0, 0, VECTOR_SIZE_ADDON);
		s.boneBottom = s.playerPos - Vector(0, 0, VECTOR_SIZE_ADDON);

		if (w2s(s.boneTop, s.screenTop) && w2s(s.boneBottom, s.screenBottom) && w2s(s.playerPos, s.screenPos))
		{
			iSize = (s.screenBottom.y - s.screenTop.y) / 2;

			// ENEMY, SPECIAL CARE
			core->g_pSurface->DrawSetColor(255, 0, 0, alpha < MIN_CROSSHAIR_ALPHA ? MIN_CROSSHAIR_ALPHA : alpha); // 210
			if (isEnemy)
			{
				drawCross(s.screenPos.x, s.screenPos.y, iSize > MIN_CROSSHAIR_SIZE ? MIN_CROSSHAIR_SIZE : iSize);
			}

			// BRACKET PART
			if (distance > 500) return;
			if (!isEnemy)
			{
				core->g_pSurface->DrawSetColor(0, 255, 0, alpha);
			}
			drawBrackets(s.screenPos.x, s.screenPos.y, iSize);
			
			// draw line from screen center
			/*
			if (distance < 1000 && isEnemy)
			{
				core->g_pSurface->DrawSetColor(255, 0, 0, 60);
				drawLineFromScreenCenter(s.screenPos.x, s.screenPos.y);
				
			}
			*/
		}
	}
}

void hack::drawLineFromScreenCenter(int x, int y)
{
	static int width, height;
	core->g_pEngine->GetScreenSize(width, height);
	core->g_pSurface->DrawLine(x, y, width / 2, height / 2);
}

void hack::drawBrackets(int x, int y, int margin)
{
	// left head bracket
	core->g_pSurface->DrawLine(x - margin, y - margin, x - margin, y + margin);
	core->g_pSurface->DrawLine(x - margin, y - margin, x - margin + margin / 3, y - margin);
	core->g_pSurface->DrawLine(x - margin, y + margin, x - margin + margin / 3, y + margin);
	// right head bracket
	core->g_pSurface->DrawLine(x + margin, y - margin, x + margin, y + margin);
	core->g_pSurface->DrawLine(x + margin - margin / 3, y - margin, x + margin, y - margin);
	core->g_pSurface->DrawLine(x + margin - margin / 3, y + margin, x + margin, y + margin);
}

void hack::drawCross(int x, int y, int size)
{
	core->g_pSurface->DrawLine(x - size / 2, y, x + size / 2, y); // width
	core->g_pSurface->DrawLine(x, y - size / 2, x, y + size / 2); // height
}

void hack::drawTitanSign(int x, int y, int size)
{
	core->g_pSurface->DrawLine(x - size, y, x + size, y); // top line
	core->g_pSurface->DrawLine(x, y, x, y - size * 2); // connector
	core->g_pSurface->DrawLine(x - size / 2, y - size * 2, x + size / 2, y - size * 2); // bottom line
}

void hack::drawTitan(CBaseEntity* player, float distance, byte isEnemy)
{
	// distance is unused
	// isEnemy is unused
	const int VECTOR_SIZE_ADDON = 100;
	static usedToDrawStruct s;

	s.playerPos = player->GetAbsOrigin();
	s.boneTop = s.playerPos + Vector(0, 0, VECTOR_SIZE_ADDON);
	s.boneBottom = s.playerPos - Vector(0, 0, VECTOR_SIZE_ADDON);
	if (
		w2s(s.boneTop, s.screenTop)
		&& w2s(s.boneBottom, s.screenBottom)
		&& w2s(s.playerPos, s.screenPos)
		)
	{
		drawTitanSign(s.screenPos.x, s.screenPos.y, (s.screenBottom.y - s.screenTop.y) / 2);
	}
}

void hack::drawMinion(CBaseEntity* player, float distance, byte isEnemy)
{
	const int VECTOR_SIZE_ADDON = 4;
	static usedToDrawStruct s;

	if (player->SetupBones(s.boneList, 128, 256, Plat_FloatTime()))
	{
		s.playerPos = Vector(s.boneList[12][0][3], s.boneList[12][1][3], s.boneList[12][2][3]);
		s.boneTop = s.playerPos + Vector(0, 0, VECTOR_SIZE_ADDON);
		s.boneBottom = s.playerPos - Vector(0, 0, VECTOR_SIZE_ADDON);
		if (
			w2s(s.boneTop, s.screenTop)
			&& w2s(s.boneBottom, s.screenBottom)
			&& w2s(s.playerPos, s.screenPos)
			)
		{
			drawCross(s.screenPos.x, s.screenPos.y, (s.screenBottom.y - s.screenTop.y) / 2);
			/*
			if (isEnemy && distance < 1000)
			{
				drawLineFromScreenCenter(s.screenPos.x, s.screenPos.y);
			}
			*/
		}
	}
}

void hack::drawOnRadar(Vector& screenPos)
{
	static int w, h, x2, y2;
	core->g_pEngine->GetScreenSize(w, h);
	if (screenPos.x < w / 2)
	{
		x2 = w / 2 - (w / 2 - screenPos.x) / 8;
	}
	else
	{
		x2 = w / 2 + (screenPos.x - w / 2) / 8;
	}
	y2 = h - (h - screenPos.y) / 8;
	core->g_pSurface->DrawLine(w / 2, h - 50, x2, y2 - 50);
}

void hack::drawDebug()
{
	static wchar_t buff[512];

	core->g_pSurface->DrawSetTextColor(255, 255, 255, 200);
	
	/*
	swprintf_s(buff, L"uberStruct.bufferedAngles.y: x:%.2f,y:%.2f", uberStruct.bufferedAngles.x, uberStruct.bufferedAngles.y);
	core->g_pSurface->DrawSetTextPos(10, 200);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));

	swprintf_s(buff, L"uberStruct.viewAngles.x: x:%.2f,y:%.2f", uberStruct.viewAngles.x, uberStruct.viewAngles.y);
	core->g_pSurface->DrawSetTextPos(10, 220);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));
	*/
}

void hack::getEyePosition(CBaseEntity* player, float* returnAngles)
{
	Vector vect1 = player->GetAbsOrigin();
	
	returnAngles[0] = vect1.x;
	returnAngles[1] = vect1.y;
	returnAngles[2] = vect1.z + *(float*)(player + m_vecViewOffset_z);
}

void hack::drawAllBones(CBaseEntity* player, int from, int to)
{
	const int VECTOR_SIZE_ADDON = 1;
	static matrix3x4 boneList[128];
	static Vector bone, boneScreen;
	static Vector boneTop, boneBottom, screenTop, screenBottom;
	static wchar_t buff[512];
	static int boneSize;

	if (player->SetupBones(boneList, 128, 256, Plat_FloatTime()))
	{
		for (int i = from; i < to; i++)
		{
			bone = Vector(boneList[i][0][3], boneList[i][1][3], boneList[i][2][3]);
			boneTop = bone + Vector(0, 0, VECTOR_SIZE_ADDON);
			boneBottom = bone - Vector(0, 0, VECTOR_SIZE_ADDON);

			if (w2s(bone, boneScreen) && w2s(boneTop, screenTop) && w2s(boneBottom, screenBottom))
			{
				boneSize = screenBottom.x - screenTop.x;

				swprintf_s(buff, L"%d", i);
				core->g_pSurface->DrawOutlineRectangle(screenTop.x - boneSize, screenTop.y - boneSize, screenBottom.y + boneSize, screenTop.y + boneSize);
				core->g_pSurface->DrawSetTextColor(255, 255, 255, 255);
				core->g_pSurface->DrawSetTextPos(boneScreen.x, boneScreen.y);
				core->g_pSurface->DrawPrintText(buff, wcslen(buff));
			}
		}
	}
}

void hack::drawStat()
{
	const wchar_t offStr[] = L"off";
	core->g_pSurface->DrawSetTextFont(0);
	core->g_pSurface->DrawSetTextColor(255, 0, 0, 200);
	core->g_pSurface->DrawSetTextPos(10, 10);
	core->g_pSurface->DrawPrintText(offStr, wcslen(offStr));
}

void hack::drawStatLn()
{
	static wchar_t buff[512];

	//core->g_pSurface->DrawSetTextFont(0);
	core->g_pSurface->DrawSetTextColor(255, 255, 255, 150);

	swprintf_s(buff, L"MAIN: %S", MAIN_SWITCH ? "true" : "false");
	core->g_pSurface->DrawSetTextPos(10, 10);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));

	swprintf_s(buff, L"CROSSHAIR: %S", CROSSHAIR_SWITCH ? "true" : "false");
	core->g_pSurface->DrawSetTextPos(10, 30);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));

	swprintf_s(buff, L"RADAR: %S", RADAR_SWITCH ? "true" : "false");
	core->g_pSurface->DrawSetTextPos(10, 50);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));

	swprintf_s(buff, L"AIM: %S", AIMBOT_SWITCH ? "true" : "false");
	core->g_pSurface->DrawSetTextPos(10, 70);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));
}