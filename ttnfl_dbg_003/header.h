#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#define M_RADPI 57.295779513082
#define M_PI 3.14159265358979323846
#include "vmth64.h"
#include "hack.h"
#include "CoreHaxFunc.h"
#include "HackMechanics.h"
#include "ExtendedPlayerClass.h"

extern tPaintTraverse oPaintTraverse;
extern tCreateMove oCreateMove;
extern cvmth64* VMThook;
extern cvmth64* VMThook2;
extern base* core;
extern hack* myHack;

extern bool __DEBUG;
extern char __DEBUG_BUFF[512];
extern wchar_t __DEBUG_BUFF_W[512];
extern bool MAIN_SWITCH;
extern bool CROSSHAIR_SWITCH;
extern bool RADAR_SWITCH;
extern bool NORECOIL_SWITCH;

typedef struct _PEB_LDR_DATA {
	BYTE       Reserved1[8];
	PVOID      Reserved2[3];
	LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

struct TargetList_t
{
	float distance3D;
	float crosshairDistance;
	float AimbotAngle[3];

	TargetList_t()
	{
	}

	TargetList_t(float aimbotAngle[], float myCoords[], float enemyCoords[])
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

	void fill2Ddistance(float distance)
	{
		crosshairDistance = distance;
	}
};

struct CompareTargetEnArray3D
{
	bool operator() (TargetList_t & lhs, TargetList_t & rhs)
	{
		return lhs.distance3D < rhs.distance3D;
	}
};

struct CompareTargetEnArray2D
{
	bool operator() (TargetList_t & lhs, TargetList_t & rhs)
	{
		return lhs.crosshairDistance < rhs.crosshairDistance;
	}
};