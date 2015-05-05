#pragma once
#include "sdk.h"
#include "debug.h"

class base
{
public:
	HINSTANCE hEngine;
	HINSTANCE hClient;
	HINSTANCE hVGui;
	HINSTANCE hVGui2;
	HINSTANCE hTier0;

	tCreateInterface g_pEngineFactory;
	tCreateInterface g_ClientFactory;
	tCreateInterface g_VGUIFactory;
	tCreateInterface g_VGUI2Factory;

	EngineClient* g_pEngine;
	CEntList* g_pEntList;
	ISurface* g_pSurface;
	IPanel* g_pIPanel;
	IBaseClientDLL* g_pClient;
	dbg* debug;
	EngineTraceClient004* engineTrace;

	base();
	void initFaces();
	void hook();
};

class hack
{
public:
	matrix3x4* m_vMatrix;

	struct usedToDrawStruct {
		Vector playerPos;
		Vector screenPos;
		Vector boneTop;
		Vector boneBottom;
		Vector screenTop;
		Vector screenBottom;
		matrix3x4 boneList[128];
	};

	float getDist(Vector& myCoords, Vector& enemyCoords);
	bool w2s(Vector &vOrigin, Vector &vScreen);
	void inTheMiddle(VPANEL vguiPanel);
	void drawOff();
	void getMatrix();
	void drawPlayer(CBaseEntity*, float, byte);
	void drawTitan(CBaseEntity*, float, byte);
	void drawMinion(CBaseEntity*, float, byte);
	void drawOnRadar(Vector& screenPos);
	void drawPunchedCrosshair(float, float);
	bool traceray(CBaseEntity* me, CBaseEntity* you);
	void drawLineFromScreenCenter(int, int);
	void drawBrackets(int, int, int);
	void drawTitanSign(int, int, int);
	void drawCross(int, int, int);
	void drawDebug();
	void drawStat();
	void drawStatLn();
	void getHead(CBaseEntity*, float*);
	void getBonePos(CBaseEntity*, int, float*);
	void drawAllBones(CBaseEntity*, int, int);
	void drawCrossAt(int, int);
	void getEyePosition(CBaseEntity* player, float* returnAngles);
};

struct myStruct {
	QAngle viewAngles;
	QAngle prefferedAimAngles;
	QAngle bufferedAngles;
	float aimAt[3];
	myStruct()
	{
		aimAt[0] = 0;
		aimAt[1] = 0;
		aimAt[2] = 0;
	}
};

extern tPlat_FloatTime Plat_FloatTime;
extern base* core;
extern CBaseEntity* myPlayer;
extern myStruct uberStruct;