#pragma once
#include "header.h"


class ExtendedPlayerClass
{
public:
	int health;
	int maxHealth;
	int lifeState;
	int index;
	int teamNumber;
	bool enemy;
	float distance2D;
	float distance3D;

	//ExtendedPlayerClass(CBaseEntity*);
	ExtendedPlayerClass(CBaseEntity*, int);
private:
	CBaseEntity* player;
	bool isEnemy();
};

