#pragma once
#include "header.h"

class Player
{
public:
	CBaseEntity* player;
	int lifeState = *(int*)(DWORD64(player) + m_lifeState);
	int team = *(int*)(DWORD64(player) + m_iTeamNum);
	int inventory = *(int*)(DWORD64(player) + m_inventory);
	Vector punchVec = *(Vector*)(player + m_local + m_vecPunchBase_Angle);
	Vector punchVecVel = *(Vector*)(player + m_local + m_vecPunchBase_AngleVel);

	bool enemy;

	Player(int);
	bool check();
	~Player();
};

