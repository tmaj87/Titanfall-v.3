#include "header.h"

void DrawStuff::inTheMiddle(VPANEL vguiPanel)
{
	static int w, h;
	core->g_pIPanel->GetSize(vguiPanel, w, h);
	core->g_pSurface->DrawSetColor(255, 20, 147, 160);
	core->g_pSurface->DrawLine(w / 2 + 5, h / 2 - 2, w / 2 + 5, h / 2 + 2);
	core->g_pSurface->DrawLine(w / 2 - 5, h / 2 - 2, w / 2 - 5, h / 2 + 2);
	core->g_pSurface->DrawLine(w / 2 - 2, h / 2 - 5, w / 2 + 2, h / 2 - 5);
	core->g_pSurface->DrawLine(w / 2 - 2, h / 2 + 5, w / 2 + 2, h / 2 + 5);
}

void DrawStuff::off()
{
	const wchar_t offStr[] = L"off";
	core->g_pSurface->DrawSetTextFont(0);
	core->g_pSurface->DrawSetTextColor(255, 0, 0, 200);
	core->g_pSurface->DrawSetTextPos(10, 10);
	core->g_pSurface->DrawPrintText(offStr, wcslen(offStr));
}


void DrawStuff::pilot(CBaseEntity* player, float distance, byte isEnemy)
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

	if (player->SetupBones(s.boneList, 128, 256, PlatFloatTime()))
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

		if (myHack->worldToScreen(s.boneTop, s.screenTop) && myHack->worldToScreen(s.boneBottom, s.screenBottom) && myHack->worldToScreen(s.playerPos, s.screenPos))
		{
			iSize = (s.screenBottom.y - s.screenTop.y) / 2;

			// ENEMY, SPECIAL CARE
			core->g_pSurface->DrawSetColor(255, 0, 0, alpha < MIN_CROSSHAIR_ALPHA ? MIN_CROSSHAIR_ALPHA : alpha); // 210
			if (isEnemy)
			{
				cross(s.screenPos.x, s.screenPos.y, iSize > MIN_CROSSHAIR_SIZE ? MIN_CROSSHAIR_SIZE : iSize);
			}

			// BRACKET PART
			if (distance > 500) return;
			if (!isEnemy)
			{
				core->g_pSurface->DrawSetColor(0, 255, 0, alpha);
			}
			brackets(s.screenPos.x, s.screenPos.y, iSize);
		}
	}
}

void DrawStuff::drawMarkBasedOnType(CBaseEntity* player, byte type, float distFromMe, byte isEnemy, byte alpha)
{
	switch (type)
	{
	case PLAYER:
		pilot(player, distFromMe, isEnemy);
		break;
	case TITAN:
		if (isEnemy)
		{
			core->g_pSurface->DrawSetColor(255, 153, 0, alpha);
		}
		titan(player);
		break;
	case MINION:
		if (!isEnemy || *(int*)(DWORD64(player) + m_hGroundEntity) == -1) // remove ally and weapons on ground
		{
			return;
		}
		core->g_pSurface->DrawSetColor(255, 255, 0, alpha);
		minion(player, distFromMe, isEnemy);
		break;
	}
}

void DrawStuff::lineFromScreenCenter(int x, int y)
{
	static int width, height;
	core->g_pEngine->GetScreenSize(width, height);
	core->g_pSurface->DrawLine(x, y, width / 2, height / 2);
}

void DrawStuff::brackets(int x, int y, int margin)
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

void DrawStuff::cross(int x, int y, int size)
{
	core->g_pSurface->DrawLine(x - size / 2, y, x + size / 2, y); // width
	core->g_pSurface->DrawLine(x, y - size / 2, x, y + size / 2); // height
}

void DrawStuff::titanSign(int x, int y, int size)
{
	core->g_pSurface->DrawLine(x - size, y, x + size, y); // top line
	core->g_pSurface->DrawLine(x, y, x, y - size * 2); // connector
	core->g_pSurface->DrawLine(x - size / 2, y - size * 2, x + size / 2, y - size * 2); // bottom line
}

void DrawStuff::titan(CBaseEntity* player)
{
	const int VECTOR_SIZE_ADDON = 100;
	static usedToDrawStruct s;

	s.playerPos = player->GetAbsOrigin();
	s.boneTop = s.playerPos + Vector(0, 0, VECTOR_SIZE_ADDON);
	s.boneBottom = s.playerPos - Vector(0, 0, VECTOR_SIZE_ADDON);
	if (
		myHack->worldToScreen(s.boneTop, s.screenTop)
		&& myHack->worldToScreen(s.boneBottom, s.screenBottom)
		&& myHack->worldToScreen(s.playerPos, s.screenPos)
		)
	{
		titanSign(s.screenPos.x, s.screenPos.y, (s.screenBottom.y - s.screenTop.y) / 2);
	}
}

void DrawStuff::minion(CBaseEntity* player, float distance, byte isEnemy)
{
	const int VECTOR_SIZE_ADDON = 4;
	static usedToDrawStruct s;

	if (player->SetupBones(s.boneList, 128, 256, PlatFloatTime()))
	{
		s.playerPos = Vector(s.boneList[12][0][3], s.boneList[12][1][3], s.boneList[12][2][3]);
		s.boneTop = s.playerPos + Vector(0, 0, VECTOR_SIZE_ADDON);
		s.boneBottom = s.playerPos - Vector(0, 0, VECTOR_SIZE_ADDON);
		if (
			myHack->worldToScreen(s.boneTop, s.screenTop)
			&& myHack->worldToScreen(s.boneBottom, s.screenBottom)
			&& myHack->worldToScreen(s.playerPos, s.screenPos)
			)
		{
			cross(s.screenPos.x, s.screenPos.y, (s.screenBottom.y - s.screenTop.y) / 2);
			/*
			if (isEnemy && distance < 1000)
			{
			drawLineFromScreenCenter(s.screenPos.x, s.screenPos.y);
			}
			*/
		}
	}
}

void DrawStuff::onRadar(Vector& screenPos)
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

void DrawStuff::debug()
{
	static wchar_t buff[512];

	core->g_pSurface->DrawSetTextColor(255, 255, 255, 200);

	swprintf_s(buff, L"uberStruct.viewAngles.x: x:%.0f,y:%.0f", uberStruct.viewAngles.x, uberStruct.viewAngles.y);
	core->g_pSurface->DrawSetTextPos(10, 220);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));

	swprintf_s(buff, L"uberStruct.aimAt: x:%.0f,y:%.0f", uberStruct.aimAt[0], uberStruct.aimAt[1]);
	core->g_pSurface->DrawSetTextPos(10, 240);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));

	swprintf_s(buff, L"punchVec: x:%.2f,y:%.2f", myPlayer.punchVec.x, myPlayer.punchVec.y);
	core->g_pSurface->DrawSetTextPos(10, 260);
	core->g_pSurface->DrawPrintText(buff, wcslen(buff));
}


void DrawStuff::allBones(CBaseEntity* player, int from, int to)
{
	const int VECTOR_SIZE_ADDON = 1;
	static matrix3x4 boneList[128];
	static Vector bone, boneScreen;
	static Vector boneTop, boneBottom, screenTop, screenBottom;
	static wchar_t buff[512];
	static int boneSize;

	if (player->SetupBones(boneList, 128, 256, PlatFloatTime()))
	{
		for (int i = from; i < to; i++)
		{
			bone = Vector(boneList[i][0][3], boneList[i][1][3], boneList[i][2][3]);
			boneTop = bone + Vector(0, 0, VECTOR_SIZE_ADDON);
			boneBottom = bone - Vector(0, 0, VECTOR_SIZE_ADDON);

			if (myHack->worldToScreen(bone, boneScreen) && myHack->worldToScreen(boneTop, screenTop) && myHack->worldToScreen(boneBottom, screenBottom))
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

void DrawStuff::stat()
{
	const wchar_t offStr[] = L"off";
	core->g_pSurface->DrawSetTextFont(0);
	core->g_pSurface->DrawSetTextColor(255, 0, 0, 200);
	core->g_pSurface->DrawSetTextPos(10, 10);
	core->g_pSurface->DrawPrintText(offStr, wcslen(offStr));
}

void DrawStuff::statLn()
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