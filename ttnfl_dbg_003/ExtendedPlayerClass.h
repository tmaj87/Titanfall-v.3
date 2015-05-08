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

	//ExtendedPlayerClass(CBaseEntity*);
	ExtendedPlayerClass(CBaseEntity*, int);
	bool isEnemy();
private:
	CBaseEntity* player;
};

