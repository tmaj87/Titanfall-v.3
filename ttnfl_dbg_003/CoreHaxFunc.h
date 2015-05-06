#pragma once
class CoreHaxFunc
{
public:
	//static void DoMeAFavour();
	static void keyManager();
	static void VectorAngles(const float *forward, float *angles);
	static bool visibilityCheck(Vector &vecAbsStart, Vector &vecAbsEnd, Vector &posToWrite);
};

