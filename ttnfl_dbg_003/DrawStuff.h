#pragma once
#include "header.h"

class DrawStuff
{
public:
	struct usedToDrawStruct {
		Vector playerPos;
		Vector screenPos;
		Vector boneTop;
		Vector boneBottom;
		Vector screenTop;
		Vector screenBottom;
		matrix3x4 boneList[128];
	};

	void inTheMiddle(VPANEL vguiPanel);
	void off();
	void pilot(CBaseEntity*, float, byte);
	void titan(CBaseEntity*);
	void minion(CBaseEntity*, float, byte);
	void onRadar(Vector& screenPos);
	void lineFromScreenCenter(int, int);
	void brackets(int, int, int);
	void titanSign(int, int, int);
	void cross(int, int, int);
	void debug();
	void stat();
	void statLn();
	void allBones(CBaseEntity*, int, int);
	void byType(CBaseEntity* player, byte type, float distFromMe, byte isEnemy, byte);
};