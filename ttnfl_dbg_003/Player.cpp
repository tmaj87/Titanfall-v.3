#include "Player.h"


Player::Player(CBaseEntity* player)
{
	this.player = player;
	lifeState = *(int*)(DWORD64(player) + m_lifeState);
	team = *(int*)(DWORD64(player) + m_iTeamNum);
	inventory = *(int*)(DWORD64(player) + m_inventory);
}


Player::~Player()
{
}
