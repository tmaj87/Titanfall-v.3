#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#define M_RADPI 57.295779513082
#define M_PI 3.14159265358979323846
#ifndef SHOW_DEBUG
	#define SHOW_DEBUG 1
#endif
#include "vmth64.h"
#include "hack.h"
#include "CoreHaxFunc.h"
#include "HackMechanics.h"
#include "DrawStuff.h"
#include "Player.h"

extern PaintTraverse oPaintTraverse;
extern CreateMove oCreateMove;
extern cvmth64* VMThook;
extern cvmth64* VMThook2;
extern hack* myHack;
extern CoreHaxFunc* core;
extern DrawStuff* draw;
extern Plat_FloatTime PlatFloatTime;
extern CBaseEntity* myPlayer;
extern Player myPlyr;
extern int myPlayerIdx;
extern myStruct uberStruct;

extern bool __DEBUG;
extern char __DEBUG_BUFF[512];
extern wchar_t __DEBUG_BUFF_W[512];
extern bool MAIN_SWITCH;
extern bool CROSSHAIR_SWITCH;
extern bool RADAR_SWITCH;
extern bool AIMBOT_SWITCH;

typedef struct _PEB_LDR_DATA {
	BYTE       Reserved1[8];
	PVOID      Reserved2[3];
	LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

struct TargetList
{
	float distance3D;
	float distance2D;
	float AimbotAngle[3];

	TargetList()
	{
	}

	TargetList(float aimbotAngle[], float myCoords[], float enemyCoords[])
	{
		AimbotAngle[0] = aimbotAngle[0];
		AimbotAngle[1] = aimbotAngle[1];
		AimbotAngle[2] = aimbotAngle[2];

		distance3D = Get3dDistance(myCoords[0], myCoords[1], myCoords[2], enemyCoords[0], enemyCoords[1], enemyCoords[2]);
	}

	float Get3dDistance(float myCoordsX, float myCoordsZ, float myCoordsY,
		float eNx, float eNz, float eNy)
	{
		return (float)sqrt(
			pow(double(eNx - myCoordsX), 2.0) +
			pow(double(eNy - myCoordsY), 2.0) +
			pow(double(eNz - myCoordsZ), 2.0));
	}
};

struct CompareTargetsIn2D
{
	bool operator() (TargetList & lhs, TargetList & rhs)
	{
		return lhs.distance2D < rhs.distance2D;
	}
};

struct CompareTargetsIn3D
{
	bool operator() (TargetList & lhs, TargetList & rhs)
	{
		return lhs.distance3D < rhs.distance3D;
	}
};