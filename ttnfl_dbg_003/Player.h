#pragma once
#include "header.h"

class Player
{
public:
	CBaseEntity* player;
	int lifeState = *(int*)(DWORD64(player) + m_lifeState);
	int team = *(int*)(DWORD64(player) + m_iTeamNum);
	int health = *(int*)(DWORD64(player) + m_iHealth);
	int inventory = *(int*)(DWORD64(player) + m_inventory);
	float viewOffset = *(float*)(player + m_vecViewOffset_z);
	Vector punchVec = *(Vector*)(player + m_local + m_vecPunchBase_Angle);
	Vector punchVecVel = *(Vector*)(player + m_local + m_vecPunchBase_AngleVel);
	Vector position = player->GetAbsOrigin();

	bool enemy;
	Vector eyesPositon;
	byte type;
	playerInfo pInfo;
	double distanceFromMe;

	Player();
	Player(int);
	bool check();
	void getEyes();
	bool isEnemy();
	void setType();
	~Player();
};

