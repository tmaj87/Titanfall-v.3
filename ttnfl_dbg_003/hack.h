#pragma once
#include "sdk.h"
#include "debug.h"

class hack
{
public:
	float getDist(Vector&, Vector&);
	bool worldToScreen(Vector&, Vector&);
	void getMatrix();
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
	float lastPunch[3];
	myStruct()
	{
		aimAt[0] = 0;
		aimAt[1] = 0;
		aimAt[2] = 0;
	}
};