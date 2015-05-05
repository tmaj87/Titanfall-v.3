#pragma once
#include "header.h"

class CoreHaxFunc
{
public:
	//static void DoMeAFavour();
	static void keyManager();
	static void VectorAngles(const float *forward, float *angles);
	static void CalcAngle(float* src, float* dst, float* angles);
	static void CalcAngleV2(float* src, float* dst, float* angles);

};

