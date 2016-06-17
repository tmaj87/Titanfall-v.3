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
extern Player myPlayer;
extern int myPlayerIndex;
extern myStruct uberStruct;

extern bool __DEBUG;
extern char __DEBUG_BUFF[512];
extern wchar_t __DEBUG_BUFF_W[512];
extern bool MAIN_SWITCH;
extern bool CROSSHAIR_SWITCH;
extern bool RADAR_SWITCH;
extern bool AIMBOT_SWITCH;
