#pragma once
#include "header.h"

class Player
{
public:
	CBaseEntity* player;
	int lifeState;
	int team;
	int inventory;

	Player(CBaseEntity* player);
	~Player();
};

