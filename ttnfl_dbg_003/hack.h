#pragma once
#include "sdk.h"
#include "debug.h"

class hack
{
public:
	float getDist(Vector& myCoords, Vector& enemyCoords);
	bool w2s(Vector &vOrigin, Vector &vScreen);
	void getMatrix();
	bool traceray(CBaseEntity* me, CBaseEntity* you);
	void getHead(CBaseEntity*, float*);
	void getBonePos(CBaseEntity*, int, float*);
	void getEyePosition(CBaseEntity* player, float* returnAngles);
private:
	matrix3x4* m_vMatrix;
};

struct myStruct {
	QAngle viewAngles;
	QAngle bufferedAngles;
	float aimAt[3];
	float enemyDistance2D;
	myStruct()
	{
		aimAt[0] = 0;
		aimAt[1] = 0;
		aimAt[2] = 0;
	}
};