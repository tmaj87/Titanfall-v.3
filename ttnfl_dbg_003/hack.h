#pragma once
#include "sdk.h"
#include "debug.h"

class hack
{
public:
	float get3dDist(Vector&, Vector&);
	bool worldToScreen(Vector&, Vector&);
	void getMatrix();
	void getHead(CBaseEntity*, Vector*);
	void getBonePos(CBaseEntity*, int, Vector*);
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

enum drawType {
	TITAN = 0x01,
	PLAYER,
	MINION
};

struct TargetList
{
	float distance3D;
	float distance2D;
	Vector aimAngle;

	TargetList()
	{
	}

	TargetList(Vector angle)
	{
		aimAngle = angle;
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