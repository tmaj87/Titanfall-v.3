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
	engineTrace = (EngineTraceClient004*)g_pEngineFactory("EngineTraceClient004", NULL);
	g_pEntList = (CEntList*)g_ClientFactory("VClientEntityList003", NULL);
	g_pSurface = (ISurface*)g_VGUIFactory("VGUI_Surface031", NULL);
	g_pIPanel = (IPanel*)g_VGUI2Factory("VGUI_Panel009", NULL);
	//g_pDebug = (IVDebugOverlay*)g_pEngineFactory("VDebugOverlay004", NULL);
	g_pClient = (IBaseClientDLL*)g_ClientFactory("VClient018", NULL);
	Plat_FloatTime = (tPlat_FloatTime)GetProcAddress(hTier0, "Plat_FloatTime");

	if (SHOW_DEBUG)
	{
		char buff[64] = { 0 };
		sprintf_s(buff, "engineTrace: %p\n", engineTrace);
		debug->toFile(buff);
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
	//core->g_pSurface->DrawLine(w / 2 - 5, h / 2, w / 2 + 5, h / 2);
	//core->g_pSurface->DrawLine(w / 2, h / 2 - 5, w / 2, h / 2 + 5);
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

	//sprintf_s(__DEBUG_BUFF, "dwFunc1:%llX dwRenderHolder:%llX pRender:%llX", dwFunc1, dwRenderHolder, pRender);
	//core->debug->toFile(__DEBUG_BUFF);
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
			// *(int*)(DWORD64(myPlayer) + m_iTeamNum) != *(int*)(DWORD64(player) + m_iTeamNum)
			if (distance < 1000 && isEnemy)
			{
				core->g_pSurface->DrawSetColor(255, 0, 0, 60);
				drawLineFromScreenCenter(s.screenPos.x, s.screenPos.y);
				
			}
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
			if (isEnemy && distance < 1000)
			{
				drawLineFromScreenCenter(s.screenPos.x, s.screenPos.y);
			}
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

void hack::drawPunchedCrosshair(float vecPunchX, float vecPunchY)
{
	static int screenWidth, screenHeight;
	core->g_pEngine->GetScreenSize(screenWidth, screenHeight);

	static float punchX, punchY, screenPosX, screenPosY;
	screenPosX = screenWidth / 2;
	screenPosY = screenHeight / 2;

	punchX = (screenWidth / 358.f) * (vecPunchX * 2);
	punchY = (screenHeight / 178.f) * (vecPunchY * 2);
	screenPosX -= punchY;
	screenPosY += punchX;

	// core->g_pSurface->DrawSetColor(255, 255, 0, 190);
	core->g_pSurface->DrawLine(screenPosX - 4, screenPosY, screenPosX + 4, screenPosY);
	core->g_pSurface->DrawLine(screenPosX, screenPosY + 4, screenPosX, screenPosY - 4);
}

bool hack::traceray(CBaseEntity* me, CBaseEntity* you)
{
	/*
	Ray_t ray;
	trace_t tr;
	
	ray.Init(me->GetAbsOrigin(), you->GetAbsOrigin());

	core->engineTrace->TraceRay(ray, 0x4600400B, NULL, &tr);

	if (tr.m_pEnt == you && tr.fractionleftsolid == 1.0f)
	{
		return 1;
	}
	return 0;
	*/

	/*
	trace_t tr;
	Ray_t ray;

	CTraceFilter traceFilter(me);

	ray.Init(me->GetAbsOrigin(), you->GetAbsOrigin());

	core->engineTrace->TraceRay(ray, 0x46004003, &traceFilter, &tr);

	if (you && tr.m_pEnt)
	{
		return(tr.m_pEnt == you || tr.fraction >= 1.0f);
	}
	*/
}

void hack::drawDebug() // myStruct uberStruct
{
	static wchar_t buff[512];
	/*
	if (myHack->traceray(myPlayer, player) && i < 32)
	{
	swprintf_s(buff, L"visible");
	core->g_pSurface->DrawSetTextFont(0);
	core->g_pSurface->DrawSetTextColor(255, 255, 255, 200);
	core->g_pSurface->DrawSetTextPos(screenPos.x, screenPos.y);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));
	}
	*/

	/*
	swprintf_s(buff, L"index(9):%d  dormant(8):%d", player->GetIndex(), player->IsDormant());
	core->g_pSurface->DrawSetTextFont(0);
	core->g_pSurface->DrawSetTextColor(255, 255, 255, 200);
	core->g_pSurface->DrawSetTextPos(screenPos.x, screenPos.y);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));
	*/

	QAngle vec1 = *(QAngle*)(myPlayer + m_local + m_vecPunchBase_Angle);
	QAngle vec2 = *(QAngle*)(myPlayer + m_local + m_vecPunchWeapon_Angle);
	QAngle vec3 = *(QAngle*)(myPlayer + m_local + m_vecPunchBase_AngleVel);
	QAngle vec4 = *(QAngle*)(myPlayer + m_local + m_vecPunchWeapon_AngleVel);

	core->g_pSurface->DrawSetTextFont(0);
	core->g_pSurface->DrawSetTextColor(255, 255, 255, 200);

	swprintf_s(buff, L"m_vecPunchBase_Angle: x:%.2f,y:%.2f,z:%.2f", vec1.x, vec1.y, vec1.z);
	core->g_pSurface->DrawSetTextPos(10, 200);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));

	swprintf_s(buff, L"m_vecPunchBase_AngleVel: x:%.2f,y:%.2f,z:%.2f", vec3.x, vec3.y, vec3.z);
	core->g_pSurface->DrawSetTextPos(10, 220);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));

	swprintf_s(buff, L"m_vecPunchWeapon_Angle: x:%.2f,y:%.2f,z:%.2f", vec2.x, vec2.y, vec2.z);
	core->g_pSurface->DrawSetTextPos(10, 240);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));

	swprintf_s(buff, L"m_vecPunchWeapon_AngleVel: x:%.2f,y:%.2f,z:%.2f", vec4.x, vec4.y, vec4.z);
	core->g_pSurface->DrawSetTextPos(10, 260);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));

	swprintf_s(buff, L"viewAngles: x:%.2f,y:%.2f,z:%.2f", uberStruct.viewAngles.x, uberStruct.viewAngles.y, uberStruct.viewAngles.z);
	core->g_pSurface->DrawSetTextPos(10, 280);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));

	swprintf_s(buff, L"aimAt: x:%.2f,y:%.2f,z:%.2f", aimAt[0], aimAt[1], aimAt[2]);
	core->g_pSurface->DrawSetTextPos(10, 300);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));
	// auto-eject: relatywnie od mojej pozycji...

	if (NORECOIL_SWITCH && RADAR_SWITCH)
	{
		//drawPunchedCrosshair(vec2.x, vec2.y);
	}
}

void hack::drawAllBones(CBaseEntity* player, int from, int to)
{
	const int VECTOR_SIZE_ADDON = 1;
	static matrix3x4 boneList[128];
	static Vector bone, boneScreen;
	static Vector boneTop, boneBottom, screenTop, screenBottom;
	static wchar_t buff[512];

	if (player->SetupBones(boneList, 128, 256, Plat_FloatTime()))
	{
		for (int i = from; i < to; i++)
		{
			bone = Vector(boneList[i][0][3], boneList[i][1][3], boneList[i][2][3]);
			boneTop = bone + Vector(0, 0, VECTOR_SIZE_ADDON);
			boneBottom = bone - Vector(0, 0, VECTOR_SIZE_ADDON);

			if (w2s(bone, boneScreen) && w2s(boneTop, screenTop) && w2s(boneBottom, screenBottom))
			{
				swprintf_s(buff, L"%d", i);
				core->g_pSurface->DrawOutlineRectangle(screenTop.x - VECTOR_SIZE_ADDON, screenTop.y, screenBottom.x + VECTOR_SIZE_ADDON, screenBottom.y);
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

	//bool MAIN_SWITCH = 0;
	//bool CROSSHAIR_SWITCH = 1;
	//bool RADAR_SWITCH = 1;
	//bool NORECOIL_SWITCH = 0;

	swprintf_s(buff, L"MAIN_SWITCH: %S", MAIN_SWITCH ? "true" : "false");
	core->g_pSurface->DrawSetTextPos(10, 10);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));

	swprintf_s(buff, L"CROSSHAIR_SWITCH: %S", CROSSHAIR_SWITCH ? "true" : "false");
	core->g_pSurface->DrawSetTextPos(10, 30);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));

	swprintf_s(buff, L"RADAR_SWITCH: %S", RADAR_SWITCH ? "true" : "false");
	core->g_pSurface->DrawSetTextPos(10, 50);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));

	swprintf_s(buff, L"NORECOIL_SWITCH: %S", NORECOIL_SWITCH ? "true" : "false");
	core->g_pSurface->DrawSetTextPos(10, 70);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));
}