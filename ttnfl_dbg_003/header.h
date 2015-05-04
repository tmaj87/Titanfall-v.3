#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#define M_RADPI 57.295779513082
#include "vmth64.h"
#include "hack.h"
#include "CoreHaxFunc.h"
#include "HackMechanics.h"

extern tPaintTraverse oPaintTraverse;
extern tCreateMove oCreateMove;
extern cvmth64* VMThook;
extern cvmth64* VMThook2;
extern base* core;
extern hack* myHack;

extern bool __DEBUG;
extern char __DEBUG_BUFF[512];
extern bool MAIN_SWITCH;
extern bool CROSSHAIR_SWITCH;
extern bool RADAR_SWITCH;
extern bool NORECOIL_SWITCH;

typedef struct _PEB_LDR_DATA {
	BYTE       Reserved1[8];
	PVOID      Reserved2[3];
	LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

/*
for linker errors:
    add /FORCE:MULTIPLE to the linker command line options
*/


typedef struct hckStruct_t{
	float aimAt[3];

	void fillData()
	{
		//
	}
} hckStruct;




typedef struct pointAt_t{
	float pointaAt[3];
	float aimAt[3];
	float distanceAtIt;
	float viewAngles[3];
	pointAt_t()
	{
	}
	void updateAngles(float* fromAngles)
	{
		viewAngles[0] = fromAngles[0];
		viewAngles[1] = fromAngles[1];
		viewAngles[2] = fromAngles[2];
		pointaAt[0] = fromAngles[0];
		pointaAt[1] = fromAngles[1];
		pointaAt[2] = fromAngles[2];
		//pointAt[0] = *(float*)fromAngles[0];
	}
	void setEnemy(float* aim, float* dist)
	{
		for (int i = 0; i < 3; i++)
		{
			aimAt[i] = aim[i];
		}
		distanceAtIt = *(float*)dist;
	}
} pointAtStruct;



typedef struct TargetList_t
{
	float distance3D;
	float distance2D;
	float AimbotAngle[3];
	//float oEnemyCoords[3];
	//float oMyCoords[3];

	TargetList_t()
	{
	}

	TargetList_t(float aimbotAngle[], float myCoords[], float enemyCoords[])
	{
		//oEnemyCoords = enemyCoords;
		//oMyCoords = myCoords;

		AimbotAngle[0] = aimbotAngle[0];
		AimbotAngle[1] = aimbotAngle[1];
		AimbotAngle[2] = aimbotAngle[2];

		distance3D = Get3dDistance(myCoords[0], myCoords[1], myCoords[2], enemyCoords[0], enemyCoords[1], enemyCoords[2]);
		distance2D = Get2dDistance();
	}

	float Get3dDistance(float myCoordsX, float myCoordsZ, float myCoordsY,
		float eNx, float eNz, float eNy)
	{
		return sqrt(
			pow(double(eNx - myCoordsX), 2.0) +
			pow(double(eNy - myCoordsY), 2.0) +
			pow(double(eNz - myCoordsZ), 2.0));
	}

	float Get2dDistance()
	{
		return sqrt(
			pow(double(AimbotAngle[0] - uberStruct.viewAngles.x), 2.0) +
			pow(double(AimbotAngle[1] - uberStruct.viewAngles.y), 2.0));
	}
};




typedef struct CompareTargetEnArray2D
{
	bool operator() (TargetList_t & lhs, TargetList_t & rhs)
	{
		return lhs.distance2D < rhs.distance2D;
	}
};

typedef struct CompareTargetEnArray3D
{
	bool operator() (TargetList_t & lhs, TargetList_t & rhs)
	{
		return lhs.distance3D < rhs.distance3D;
	}
};