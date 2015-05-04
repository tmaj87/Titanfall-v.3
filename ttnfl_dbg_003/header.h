#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include "vmth64.h"
#include "hack.h"
#include "CoreHaxFunc.h"

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


struct hckStruct_t {
	float aimAt[3];

	void fillData()
	{
		//
	}
} hckStruct;




struct pointAt_t {
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


